#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <functional>
#include <memory>
#include <chrono>
#include <random>
#include <algorithm>
#include <type_traits>

using namespace std;

// 任务包装器，支持优先级和延迟执行
class TaskWrapper {
public:
    using TaskFunction = function<void()>;
    
    TaskWrapper(TaskFunction func, int priority = 0) 
        : taskFunc(move(func)), 
          taskPriority(priority),
          executeTime(chrono::steady_clock::now()),
          taskId(generateId()) {}
          
    TaskWrapper(TaskFunction func, chrono::milliseconds delay, int priority = 0) 
        : taskFunc(move(func)), 
          taskPriority(priority),
          executeTime(chrono::steady_clock::now() + delay),
          taskId(generateId()) {}
    
    void execute() {
        if (taskFunc) {
            taskFunc();
        }
    }
    
    bool isReady() const {
        return chrono::steady_clock::now() >= executeTime;
    }
    
    chrono::steady_clock::time_point getExecuteTime() const {
        return executeTime;
    }
    
    int getPriority() const {
        return taskPriority;
    }
    
    size_t getId() const {
        return taskId;
    }
    
    // 用于优先级队列的比较函数
    bool operator<(const TaskWrapper& other) const {
        if (executeTime != other.executeTime) {
            return executeTime > other.executeTime; // 时间早的优先
        }
        return taskPriority < other.taskPriority; // 优先级高的优先
    }

private:
    TaskFunction taskFunc;
    int taskPriority;
    chrono::steady_clock::time_point executeTime;
    size_t taskId;
    
    static atomic<size_t> idCounter;
    static size_t generateId() {
        return idCounter.fetch_add(1);
    }
};

atomic<size_t> TaskWrapper::idCounter(0);

// 增强的线程池，支持优先级和延迟执行
class EnhancedThreadPool {
private:
    // 工作线程
    vector<thread> workers;
    
    // 任务优先级队列
    priority_queue<TaskWrapper> taskQueue;
    
    // 线程安全
    mutable mutex queueMutex;
    condition_variable condition;
    
    // 线程池状态
    atomic<bool> stop;
    atomic<size_t> activeThreads;
    atomic<size_t> finishedTasks;
    
    // 统计信息
    mutable mutex statsMutex;
    chrono::steady_clock::time_point startTime;
    
    // 工作线程函数
    void workerThread() {
        while (true) {
            unique_lock<mutex> lock(queueMutex);
            
            // 等待条件：线程池停止或有任务可执行
            condition.wait(lock, [this] {
                return stop || (!taskQueue.empty() && taskQueue.top().isReady());
            });
            
            // 如果线程池停止且没有任务，退出线程
            if (stop && taskQueue.empty()) {
                break;
            }
            
            // 检查是否有任务可执行
            if (!taskQueue.empty() && taskQueue.top().isReady()) {
                TaskWrapper task = move(const_cast<TaskWrapper&>(taskQueue.top()));
                taskQueue.pop();
                lock.unlock();
                
                // 执行任务
                activeThreads++;
                try {
                    task.execute();
                    finishedTasks++;
                } catch (const exception& e) {
                    cerr << "任务执行异常: " << e.what() << endl;
                }
                activeThreads--;
            }
        }
    }
    
    // 检查最早任务的等待时间
    chrono::milliseconds getWaitTime() const {
        if (taskQueue.empty()) {
            return chrono::milliseconds(100); // 默认等待时间
        }
        
        auto now = chrono::steady_clock::now();
        auto nextExecuteTime = taskQueue.top().getExecuteTime();
        
        if (nextExecuteTime <= now) {
            return chrono::milliseconds(0); // 立即可执行
        }
        
        return chrono::duration_cast<chrono::milliseconds>(nextExecuteTime - now);
    }

public:
    explicit EnhancedThreadPool(size_t threads = thread::hardware_concurrency()) 
        : stop(false), activeThreads(0), finishedTasks(0) {
        
        startTime = chrono::steady_clock::now();
        
        // 创建工作线程
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back(&EnhancedThreadPool::workerThread, this);
        }
    }
    
    ~EnhancedThreadPool() {
        shutdown();
    }
    
    // 添加普通任务
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> future<typename result_of<F(Args...)>::type> {
        
        using ReturnType = typename result_of<F(Args...)>::type;
        
        auto task = make_shared<packaged_task<ReturnType()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        
        future<ReturnType> result = task->get_future();
        
        {
            unique_lock<mutex> lock(queueMutex);
            
            if (stop) {
                throw runtime_error("线程池已停止，无法添加任务");
            }
            
            taskQueue.emplace([task]() { (*task)(); });
        }
        
        condition.notify_one();
        return result;
    }
    
    // 添加带优先级的任务
    template<typename F, typename... Args>
    auto enqueuePriority(F&& f, int priority, Args&&... args) 
        -> future<typename result_of<F(Args...)>::type> {
        
        using ReturnType = typename result_of<F(Args...)>::type;
        
        auto task = make_shared<packaged_task<ReturnType()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        
        future<ReturnType> result = task->get_future();
        
        {
            unique_lock<mutex> lock(queueMutex);
            
            if (stop) {
                throw runtime_error("线程池已停止，无法添加任务");
            }
            
            taskQueue.emplace([task]() { (*task)(); }, priority);
        }
        
        condition.notify_one();
        return result;
    }
    
    // 添加延迟任务
    template<typename F, typename... Args>
    auto enqueueDelayed(F&& f, chrono::milliseconds delay, Args&&... args) 
        -> future<typename result_of<F(Args...)>::type> {
        
        using ReturnType = typename result_of<F(Args...)>::type;
        
        auto task = make_shared<packaged_task<ReturnType()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        
        future<ReturnType> result = task->get_future();
        
        {
            unique_lock<mutex> lock(queueMutex);
            
            if (stop) {
                throw runtime_error("线程池已停止，无法添加任务");
            }
            
            taskQueue.emplace([task]() { (*task)(); }, delay);
        }
        
        condition.notify_one();
        return result;
    }
    
    // 批量添加任务
    template<typename Iterator>
    vector<future<void>> enqueueBatch(Iterator first, Iterator last) {
        vector<future<void>> results;
        
        for (auto it = first; it != last; ++it) {
            results.push_back(enqueue(*it));
        }
        
        return results;
    }
    
    // 等待所有任务完成
    void waitForAllTasks() {
        while (true) {
            {
                unique_lock<mutex> lock(queueMutex);
                if (taskQueue.empty() && activeThreads == 0) {
                    break;
                }
            }
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
    
    // 关闭线程池
    void shutdown() {
        {
            unique_lock<mutex> lock(queueMutex);
            stop = true;
        }
        
        condition.notify_all();
        
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        
        workers.clear();
    }
    
    // 获取线程池统计信息
    struct ThreadPoolStats {
        size_t totalThreads;
        size_t activeThreads;
        size_t finishedTasks;
        size_t pendingTasks;
        double runTimeSeconds;
    };
    
    ThreadPoolStats getStats() const {
        unique_lock<mutex> lock(queueMutex);
        unique_lock<mutex> statsLock(statsMutex);
        
        auto now = chrono::steady_clock::now();
        auto runTime = chrono::duration_cast<chrono::seconds>(now - startTime);
        
        return {
            workers.size(),
            activeThreads.load(),
            finishedTasks.load(),
            taskQueue.size(),
            runTime.count()
        };
    }
    
    // 打印统计信息
    void printStats() const {
        ThreadPoolStats stats = getStats();
        cout << "线程池统计信息:" << endl;
        cout << "  总线程数: " << stats.totalThreads << endl;
        cout << "  活动线程数: " << stats.activeThreads << endl;
        cout << "  已完成任务数: " << stats.finishedTasks << endl;
        cout << "  待处理任务数: " << stats.pendingTasks << endl;
        cout << "  运行时间: " << stats.runTimeSeconds << " 秒" << endl;
    }
};

// 线程局部存储示例
thread_local random_device rd;
thread_local mt19937 gen(rd());
thread_local uniform_int_distribution<> dis(1, 100);

// 演示函数
void basicTaskExample() {
    cout << "\n=== 基本任务示例 ===" << endl;
    
    EnhancedThreadPool pool(4);
    
    // 添加一些基本任务
    vector<future<int>> results;
    
    for (int i = 0; i < 10; ++i) {
        results.push_back(pool.enqueue([i] {
            this_thread::sleep_for(chrono::milliseconds(dis(gen) * 10));
            cout << "任务 " << i << " 执行完成" << endl;
            return i * i;
        }));
    }
    
    // 获取结果
    cout << "任务结果: ";
    for (auto& result : results) {
        cout << result.get() << " ";
    }
    cout << endl;
}

void priorityTaskExample() {
    cout << "\n=== 优先级任务示例 ===" << endl;
    
    EnhancedThreadPool pool(2);
    
    // 添加不同优先级的任务
    for (int i = 0; i < 10; ++i) {
        int priority = i % 3 + 1; // 优先级 1-3
        pool.enqueuePriority([i, priority] {
            this_thread::sleep_for(chrono::milliseconds(50));
            cout << "优先级 " << priority << " 任务 " << i << " 执行完成" << endl;
        }, priority);
    }
    
    this_thread::sleep_for(chrono::seconds(2));
}

void delayedTaskExample() {
    cout << "\n=== 延迟任务示例 ===" << endl;
    
    EnhancedThreadPool pool(2);
    
    auto start = chrono::steady_clock::now();
    
    // 添加延迟任务
    pool.enqueueDelayed([start] {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start);
        cout << "延迟100ms任务执行，实际经过时间: " << elapsed.count() << "ms" << endl;
    }, chrono::milliseconds(100));
    
    pool.enqueueDelayed([start] {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start);
        cout << "延迟200ms任务执行，实际经过时间: " << elapsed.count() << "ms" << endl;
    }, chrono::milliseconds(200));
    
    pool.enqueueDelayed([start] {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start);
        cout << "延迟300ms任务执行，实际经过时间: " << elapsed.count() << "ms" << endl;
    }, chrono::milliseconds(300));
    
    this_thread::sleep_for(chrono::seconds(1));
}

void batchTaskExample() {
    cout << "\n=== 批量任务示例 ===" << endl;
    
    EnhancedThreadPool pool(4);
    
    // 准备一批任务
    vector<function<void()>> tasks;
    for (int i = 0; i < 20; ++i) {
        tasks.push_back([i] {
            this_thread::sleep_for(chrono::milliseconds(20 + dis(gen)));
            cout << "批量任务 " << i << " 执行完成" << endl;
        });
    }
    
    // 批量添加任务
    auto results = pool.enqueueBatch(tasks.begin(), tasks.end());
    
    // 等待所有任务完成
    for (auto& result : results) {
        result.get();
    }
}

void statsExample() {
    cout << "\n=== 统计信息示例 ===" << endl;
    
    EnhancedThreadPool pool(4);
    
    // 添加一些任务
    for (int i = 0; i < 15; ++i) {
        pool.enqueue([i] {
            this_thread::sleep_for(chrono::milliseconds(dis(gen)));
        });
    }
    
    // 等待一段时间
    this_thread::sleep_for(chrono::milliseconds(500));
    
    // 打印统计信息
    pool.printStats();
    
    // 等待所有任务完成
    pool.waitForAllTasks();
    
    // 打印最终统计信息
    cout << "\n最终统计信息:" << endl;
    pool.printStats();
}

int main() {
    cout << "C++11 增强线程池演示" << endl;
    
    try {
        basicTaskExample();
        priorityTaskExample();
        delayedTaskExample();
        batchTaskExample();
        statsExample();
        
        cout << "\n程序结束" << endl;
    } catch (const exception& e) {
        cerr << "异常: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}