#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <future>
#include <memory>
#include <vector>
#include <chrono>

using namespace std;

// 简单任务队列实现
class TaskQueue {
private:
    queue<function<void()>> tasks;
    mutable mutex queue_mutex;  // 添加mutable使其可以在const方法中使用
    condition_variable condition;
    atomic<bool> stopped;
    
public:
    TaskQueue() : stopped(false) {}
    
    // 添加任务到队列
    template<typename F>
    void enqueue(F&& f) {
        {
            unique_lock<mutex> lock(queue_mutex);
            tasks.emplace(forward<F>(f));
        }
        condition.notify_one();
    }
    
    // 从队列中取出任务（阻塞直到有任务）
    bool dequeue(function<void()>& task) {
        unique_lock<mutex> lock(queue_mutex);
        
        condition.wait(lock, [this] { return stopped || !tasks.empty(); });
        
        if (stopped && tasks.empty()) {
            return false;
        }
        
        task = move(tasks.front());
        tasks.pop();
        return true;
    }
    
    // 停止任务队列
    void stop() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stopped = true;
        }
        condition.notify_all();
    }
    
    // 检查队列是否为空
    bool empty() const {
        unique_lock<mutex> lock(queue_mutex);
        return tasks.empty();
    }
    
    // 获取队列大小
    size_t size() const {
        unique_lock<mutex> lock(queue_mutex);
        return tasks.size();
    }
};

// 简单工作线程类
class Worker {
private:
    TaskQueue& queue;
    thread worker_thread;
    atomic<bool> running;
    
public:
    Worker(TaskQueue& q) : queue(q), running(true) {
        worker_thread = thread([this] {
            while (running) {
                function<void()> task;
                if (queue.dequeue(task)) {
                    task();
                } else {
                    break;
                }
            }
        });
    }
    
    ~Worker() {
        if (worker_thread.joinable()) {
            worker_thread.join();
        }
    }
    
    void stop() {
        running = false;
    }
    
    void join() {
        if (worker_thread.joinable()) {
            worker_thread.join();
        }
    }
};

// 带返回值的任务队列实现
template<typename T>
class AsyncTaskQueue {
private:
    struct AsyncTask {
        function<T()> func;
        promise<T> taskPromise;
    };
    
    queue<shared_ptr<AsyncTask>> tasks;
    mutable mutex queue_mutex;
    condition_variable condition;
    atomic<bool> stopped;
    
public:
    AsyncTaskQueue() : stopped(false) {}
    
    // 添加任务并返回future
    template<typename F>
    future<T> enqueue(F&& f) {
        auto task = make_shared<AsyncTask>();
        task->func = forward<F>(f);
        auto result = task->taskPromise.get_future();
        
        {
            unique_lock<mutex> lock(queue_mutex);
            tasks.push(task);
        }
        
        condition.notify_one();
        return result;
    }
    
    // 从队列中取出任务（阻塞直到有任务）
    bool dequeue(shared_ptr<AsyncTask>& task) {
        unique_lock<mutex> lock(queue_mutex);
        
        condition.wait(lock, [this] { return stopped || !tasks.empty(); });
        
        if (stopped && tasks.empty()) {
            return false;
        }
        
        task = tasks.front();
        tasks.pop();
        return true;
    }
    
    // 停止任务队列
    void stop() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stopped = true;
        }
        condition.notify_all();
    }
    
    // 执行任务并设置结果
    void execute_tasks() {
        while (true) {
            shared_ptr<AsyncTask> task;
            if (!dequeue(task)) {
                break;
            }
            
            try {
                if (task->func) {
                    task->taskPromise.set_value(task->func());
                }
            } catch (...) {
                task->taskPromise.set_exception(current_exception());
            }
        }
    }
};

// 优先级任务队列
template<typename T>
class PriorityQueue {
private:
    struct PriorityTask {
        int priority;
        function<void()> task;
        
        // 重载小于运算符，priority_queue使用它来比较元素
        bool operator<(const PriorityTask& other) const {
            // 注意：priority_queue默认是最大堆，所以这里反转比较
            return priority < other.priority;
        }
    };
    
    priority_queue<PriorityTask> tasks;
    mutable mutex queue_mutex;
    condition_variable condition;
    atomic<bool> stopped;
    
public:
    PriorityQueue() : stopped(false) {}
    
    // 添加带优先级的任务（数字越大优先级越高）
    template<typename F>
    void enqueue(F&& f, int priority) {
        {
            unique_lock<mutex> lock(queue_mutex);
            tasks.emplace(PriorityTask{priority, forward<F>(f)});
        }
        condition.notify_one();
    }
    
    // 从队列中取出优先级最高的任务（阻塞直到有任务）
    bool dequeue(function<void()>& task) {
        unique_lock<mutex> lock(queue_mutex);
        
        condition.wait(lock, [this] { return stopped || !tasks.empty(); });
        
        if (stopped && tasks.empty()) {
            return false;
        }
        
        task = move(tasks.top().task);
        tasks.pop();
        return true;
    }
    
    // 停止任务队列
    void stop() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stopped = true;
        }
        condition.notify_all();
    }
};

// 延迟任务队列
class DelayedTaskQueue {
private:
    struct DelayedTask {
        chrono::steady_clock::time_point execute_time;
        function<void()> task;
        
        bool operator>(const DelayedTask& other) const {
            return execute_time > other.execute_time;
        }
    };
    
    priority_queue<DelayedTask, vector<DelayedTask>, greater<DelayedTask>> tasks;
    mutable mutex queue_mutex;
    condition_variable condition;
    atomic<bool> stopped;
    thread worker_thread;
    
    void worker_loop() {
        while (true) {
            shared_ptr<DelayedTask> task;
            {
                unique_lock<mutex> lock(queue_mutex);
                
                // 等待任务或停止信号
                condition.wait(lock, [this] { 
                    return stopped || !tasks.empty(); 
                });
                
                if (stopped && tasks.empty()) {
                    break;
                }
                
                // 检查最早的任务是否到了执行时间
                if (tasks.top().execute_time > chrono::steady_clock::now()) {
                    // 还没到时间，等待到执行时间或新任务
                    condition.wait_until(lock, tasks.top().execute_time);
                    
                    // 重新检查
                    if (stopped && tasks.empty()) {
                        break;
                    }
                    
                    if (tasks.empty() || tasks.top().execute_time > chrono::steady_clock::now()) {
                        continue;
                    }
                }
                
                // 复制任务并从队列中移除
                task = make_shared<DelayedTask>(move(const_cast<DelayedTask&>(tasks.top())));
                tasks.pop();
            }
            
            // 执行任务（在锁外执行，避免阻塞其他任务添加）
            if (task && task->task) {
                task->task();
            }
        }
    }
    
public:
    DelayedTaskQueue() : stopped(false) {
        worker_thread = thread(&DelayedTaskQueue::worker_loop, this);
    }
    
    ~DelayedTaskQueue() {
        stop();
    }
    
    // 添加延迟执行的任务
    template<typename F>
    void enqueue(F&& f, chrono::milliseconds delay) {
        {
            unique_lock<mutex> lock(queue_mutex);
            tasks.emplace(DelayedTask{
                chrono::steady_clock::now() + delay, 
                forward<F>(f)
            });
        }
        condition.notify_one();
    }
    
    // 添加在指定时间点执行的任务
    template<typename F>
    void enqueue_at(F&& f, chrono::steady_clock::time_point time_point) {
        {
            unique_lock<mutex> lock(queue_mutex);
            tasks.emplace(DelayedTask{time_point, forward<F>(f)});
        }
        condition.notify_one();
    }
    
    // 停止任务队列
    void stop() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stopped = true;
        }
        condition.notify_all();
        
        if (worker_thread.joinable()) {
            worker_thread.join();
        }
    }
};

// 演示函数
void basic_task_queue_demo() {
    cout << "\n=== 基本任务队列演示 ===" << endl;
    
    TaskQueue queue;
    Worker worker(queue);
    
    // 添加一些任务
    for (int i = 1; i <= 5; ++i) {
        queue.enqueue([i] {
            cout << "执行任务 " << i << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        });
    }
    
    // 等待所有任务完成
    this_thread::sleep_for(chrono::seconds(1));
    worker.stop();
    worker.join();
}

void async_task_queue_demo() {
    cout << "\n=== 异步任务队列演示 ===" << endl;
    
    AsyncTaskQueue<int> queue;
    thread worker([&queue] { queue.execute_tasks(); });
    
    vector<future<int>> results;
    
    // 添加一些计算任务
    for (int i = 1; i <= 5; ++i) {
        results.push_back(queue.enqueue([i] {
            cout << "计算 " << i << " 的平方" << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
            return i * i;
        }));
    }
    
    // 停止队列并等待工作线程结束
    queue.stop();
    worker.join();
    
    // 获取结果
    cout << "计算结果:" << endl;
    for (auto& result : results) {
        cout << result.get() << " ";
    }
    cout << endl;
}

void priority_queue_demo() {
    cout << "\n=== 优先级任务队列演示 ===" << endl;
    
    PriorityQueue<string> queue;
    thread worker([&queue] {
        function<void()> task;
        while (queue.dequeue(task)) {
            task();
        }
    });
    
    // 添加不同优先级的任务
    queue.enqueue([] { cout << "低优先级任务" << endl; }, 1);
    queue.enqueue([] { cout << "高优先级任务" << endl; }, 5);
    queue.enqueue([] { cout << "中优先级任务" << endl; }, 3);
    queue.enqueue([] { cout << "最高优先级任务" << endl; }, 10);
    queue.enqueue([] { cout << "中低优先级任务" << endl; }, 2);
    
    // 等待所有任务完成
    this_thread::sleep_for(chrono::seconds(1));
    queue.stop();
    worker.join();
}

void delayed_task_queue_demo() {
    cout << "\n=== 延迟任务队列演示 ===" << endl;
    
    DelayedTaskQueue queue;
    
    auto start_time = chrono::steady_clock::now();
    
    // 添加延迟任务
    queue.enqueue([start_time] { 
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
        cout << "任务1 (200ms后执行), 已经过时间: " << elapsed << "ms" << endl; 
    }, chrono::milliseconds(200));
    
    queue.enqueue([start_time] { 
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
        cout << "任务2 (100ms后执行), 已经过时间: " << elapsed << "ms" << endl; 
    }, chrono::milliseconds(100));
    
    queue.enqueue([start_time] { 
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
        cout << "任务3 (300ms后执行), 已经过时间: " << elapsed << "ms" << endl; 
    }, chrono::milliseconds(300));
    
    // 等待所有任务完成
    this_thread::sleep_for(chrono::seconds(1));
}

int main() {
    cout << "C++ 任务队列实现练习" << endl;
    
    basic_task_queue_demo();
    async_task_queue_demo();
    priority_queue_demo();
    delayed_task_queue_demo();
    
    cout << "\n程序结束" << endl;
    return 0;
}