// ============================================================================
// 06 - Multithreading: 智能线程池 (Smart Thread Pool)
// ============================================================================
// 支持任务提交、future 返回值、RAII 线程管理的线程池实现。
// 使用 RAIIThread 包装器自动管理线程生命周期。
// ============================================================================

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <functional>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <future>

class SmartThreadPool {
private:
    // 自定义 RAII 线程包装器
    struct RAIIThread {
        std::thread thread;
        
        template<typename Function, typename... Args>
        RAIIThread(Function&& f, Args&&... args) 
            : thread(std::forward<Function>(f), std::forward<Args>(args)...) {}
        
        ~RAIIThread() {
            if (thread.joinable()) {
                thread.join();
            }
        }
        
        // 禁止拷贝
        RAIIThread(const RAIIThread&) = delete;
        RAIIThread& operator=(const RAIIThread&) = delete;
        
        // 允许移动
        RAIIThread(RAIIThread&&) = default;
        RAIIThread& operator=(RAIIThread&&) = default;
    };

    // 工作线程使用 RAII 包装器管理
    std::vector<RAIIThread> workers;

    // 任务队列和相关同步工具
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::mutex output_mutex; // 输出同步锁
    std::condition_variable condition;

    // 线程池状态控制
    std::atomic<bool> stop{false};
    std::atomic<int> active_tasks{0};

public:
    // 构造函数：创建指定数量的工作线程
    explicit SmartThreadPool(size_t num_threads = std::thread::hardware_concurrency()) {
        for (size_t i = 0; i < num_threads; ++i) {
            // 使用 RAIIThread 包装器，自动管理线程生命周期
            workers.emplace_back([this, i] {
                worker_loop(i);
            });
        }
        std::cout << "智能线程池已启动! 线程数: " << num_threads << std::endl;
    }

    // 添加任务到线程池（支持任意可调用对象）
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        
        // 使用 packaged_task 包装任务，支持返回值
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> result = task->get_future();
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            
            // 检查线程池是否已停止
            if (stop) {
                throw std::runtime_error("线程池已停止，无法添加新任务");
            }
            
            // 将任务包装成 void() 类型并加入队列
            tasks.emplace([task, this]() {
                ++active_tasks;
                try {
                    (*task)(); // 执行实际任务
                } catch (...) {
                    // 捕获任务中的异常，避免影响线程池
                    std::cout << "任务执行异常" << std::endl;
                }
                --active_tasks;
            });
        }
        
        // 通知一个等待的工作线程
        condition.notify_one();
        return result;
    }

    // 优雅关闭线程池
    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        
        // 等待所有活动任务完成
        while (active_tasks > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        // RAIIThread 析构时会自动 join 所有线程
        workers.clear();
        std::cout << "线程池已关闭" << std::endl;
    }

    // 析构函数：自动关闭线程池
    ~SmartThreadPool() {
        if (!stop) {
            shutdown();
        }
    }

private:
    void worker_loop(int worker_id) {
        {
            std::lock_guard<std::mutex> lock(output_mutex);
            std::cout << "工作线程 " << worker_id << " 启动" << std::endl;
        }
        
        while (true) {
            std::function<void()> task;
            
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                
                // 等待任务或停止信号
                condition.wait(lock, [this] {
                    return stop || !tasks.empty();
                });
                
                // 检查是否需要退出
                if (stop && tasks.empty()) {
                    {
                        std::lock_guard<std::mutex> lock(output_mutex);
                        std::cout << "工作线程 " << worker_id << " 退出" << std::endl;
                    }
                    return;
                }
                
                // 取出任务
                task = std::move(tasks.front());
                tasks.pop();
            }
            
            // 执行任务
            task();
        }
    }
};


// 简单的计算任务
int calculate_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return x * x;
}

int main() {
    SmartThreadPool smartthreadpool(4);

    // 测试：添加带返回值的任务
    std::cout << "\n测试带返回值的任务..." << std::endl;
    std::vector<std::future<int>> results;
    for (int i = 1; i <= 6; ++i) {
        results.push_back(smartthreadpool.enqueue(calculate_square, i));
    }
    
    // 获取结果
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "任务" << (i+1) << " 结果: " << results[i].get() << std::endl;
    }
    return 0;
}
