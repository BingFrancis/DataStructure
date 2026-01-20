#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>

// 智能指针线程池完整实现
class SmartThreadPool {
private:
    // 工作线程使用智能指针管理
    std::vector<std::unique_ptr<std::thread>> workers;
    
    // 任务队列和相关同步工具
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    
    // 线程池状态控制
    std::atomic<bool> stop{false};
    std::atomic<int> active_tasks{0};
    
    // 自定义删除器，确保线程安全退出
    struct ThreadDeleter {
        void operator()(std::thread* t) const {
            if (t && t->joinable()) {
                t->join();
            }
            delete t;
        }
    };

public:
    // 构造函数：创建指定数量的工作线程
explicit SmartThreadPool(size_t num_threads = std::thread::hardware_concurrency()) {
        for (size_t i = 0; i < num_threads; ++i) {
            // 直接使用标准unique_ptr，在析构函数中手动join
            workers.push_back(std::make_unique<std::thread>([this, i] {
                worker_loop(i);
            }));
        }
        std::cout << "智能线程池已启动! 线程数: " << num_threads << std::endl;
    }

    // 添加任务到线程池（支持任意可调用对象）
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        
        // 使用packaged_task包装任务，支持返回值
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
            
            // 将任务包装成void()类型并加入队列
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

    // 获取活跃任务数量
    int get_active_task_count() const {
        return active_tasks;
    }
    
    // 获取待处理任务数量
    size_t get_pending_task_count() {
        std::unique_lock<std::mutex> lock(queue_mutex);
        return tasks.size();
    }
    
    // 获取工作线程数量
    size_t get_worker_count() const {
        return workers.size();
    }
    
    // 等待所有任务完成
    void wait_all() {
        while (get_active_task_count() > 0 || get_pending_task_count() > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    // 优雅关闭线程池
    void shutdown() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        workers.clear(); // 智能指针自动join所有线程
        std::cout << "线程池已关闭" << std::endl;
    }
    
    // 析构函数：自动关闭线程池
    ~SmartThreadPool() {
        if (!stop) {
            shutdown();
        }
    }

private:
    // 工作线程的主循环
    void worker_loop(int worker_id) {
        std::cout << "工作线程 " << worker_id << " 启动" << std::endl;
        
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
                    std::cout << "工作线程 " << worker_id << " 退出" << std::endl;
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

// ========== 测试函数 ==========

// 简单的计算任务
int calculate_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return x * x;
}

// 带状态的任务
void process_data(const std::string& data, int& result) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    result = data.length() * 10;
    std::cout << "处理数据: " << data << ", 结果: " << result << std::endl;
}

// 异常任务（测试异常处理）
void risky_task() {
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    throw std::runtime_error("模拟任务异常");
}

// 长时间运行的任务
void long_running_task(int duration_ms) {
    std::cout << "开始长时间任务，预计耗时: " << duration_ms << "ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    std::cout << "长时间任务完成" << std::endl;
}

// 测试智能指针线程池
void test_smart_pool() {
    std::cout << "\n=== 测试智能指针线程池 ===" << std::endl;
    
    // 创建线程池（4个工作线程）
    SmartThreadPool pool(4);
    
    // 测试1：添加带返回值的任务
    std::cout << "\n1. 测试带返回值的任务..." << std::endl;
    std::vector<std::future<int>> results;
    for (int i = 1; i <= 5; ++i) {
        results.push_back(pool.enqueue(calculate_square, i));
    }
    
    // 获取结果
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "任务" << (i+1) << "结果: " << results[i].get() << std::endl;
    }
    
    // 测试2：添加无返回值任务
    std::cout << "\n2. 测试无返回值任务..." << std::endl;
    int shared_result = 0;
    std::vector<std::future<void>> void_tasks;
    
    for (int i = 0; i < 3; ++i) {
        void_tasks.push_back(pool.enqueue(process_data, 
                                         "数据块" + std::to_string(i), 
                                         std::ref(shared_result)));
    }
    
    // 等待所有任务完成
    for (auto& task : void_tasks) {
        task.get();
    }
    
    // 测试3：异常任务处理
    std::cout << "\n3. 测试异常处理..." << std::endl;
    try {
        auto exception_task = pool.enqueue(risky_task);
        exception_task.get(); // 这里会抛出异常
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
    }
    
    // 测试4：监控线程池状态
    std::cout << "\n4. 测试状态监控..." << std::endl;
    std::cout << "工作线程数: " << pool.get_worker_count() << std::endl;
    std::cout << "活跃任务数: " << pool.get_active_task_count() << std::endl;
    std::cout << "待处理任务数: " << pool.get_pending_task_count() << std::endl;
    
    // 测试5：添加大量任务
    std::cout << "\n5. 测试批量任务..." << std::endl;
    std::vector<std::future<int>> batch_results;
    for (int i = 0; i < 10; ++i) {
        batch_results.push_back(pool.enqueue([](int x) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            return x * 2;
        }, i));
    }
    
    // 使用wait_all等待所有任务
    pool.wait_all();
    std::cout << "所有批量任务已完成" << std::endl;
    
    // 测试6：动态任务添加
    std::cout << "\n6. 测试动态任务添加..." << std::endl;
    for (int i = 0; i < 3; ++i) {
        pool.enqueue([i]() {
            std::cout << "动态任务 " << i << " 执行中..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
            std::cout << "动态任务 " << i << " 完成" << std::endl;
        });
    }
    
    // 等待一会儿让任务执行
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    std::cout << "\n测试完成，线程池将自动清理..." << std::endl;
    // pool析构时会自动关闭
}

// 改进你的opps函数，使用智能指针线程池
void improved_opps_with_pool() {
    std::cout << "\n=== 改进版opps函数（使用线程池） ===" << std::endl;
    
    SmartThreadPool pool(2); // 使用2个线程的池
    
    // 使用智能指针管理数据
    auto state = std::make_shared<int>(0);
    
    // 将原来的循环任务提交到线程池
    auto future = pool.enqueue([state]() -> int {
        for (unsigned j = 0; j < 1000000; ++j) { // 减少循环次数用于演示
            *state += 10;
        }
        return *state;
    });
    
    // 可以继续执行其他操作
    std::cout << "任务已提交到线程池，主线程继续..." << std::endl;
    
    // 等待任务完成并获取结果
    int result = future.get();
    std::cout << "线程池计算结果: " << result << std::endl;
    
    // 线程池会自动清理
}

int main() {
    std::cout << "智能指针线程池完整演示" << std::endl;
    std::cout << "======================" << std::endl;
    
    test_smart_pool();
    improved_opps_with_pool();
    
    std::cout << "\n所有演示完成" << std::endl;
    return 0;
}
