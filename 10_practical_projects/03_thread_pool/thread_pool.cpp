/**
 * 线程池 (Thread Pool) 实现
 *
 * 线程池是一种并发设计模式，它维护多个工作线程，等待任务队列中的任务
 *
 * 功能：
 * - 固定数量的工作线程
 * - 任务队列管理
 * - 支持任意可调用对象（函数、lambda、函数对象）
 * - 线程安全
 * - 优雅关闭
 *
 * 编译：g++ -std=c++17 thread_pool.cpp -o thread_pool.exe -pthread
 */

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <chrono>

using namespace std;

// ========================================
// 线程池实现
// ========================================

class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queueMutex;
    condition_variable condition;
    atomic<bool> stop;

public:
    explicit ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;

                    {
                        unique_lock<mutex> lock(queueMutex);
                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    // 添加任务（无返回值）
    template<typename F>
    void enqueue(F&& f) {
        {
            unique_lock<mutex> lock(queueMutex);
            tasks.emplace(forward<F>(f));
        }
        condition.notify_one();
    }

    // 添加任务（有返回值，返回 future）
    template<typename F, typename... Args>
    auto enqueueWithResult(F&& f, Args&&... args)
        -> future<typename result_of<F(Args...)>::type> {

        using ReturnType = typename result_of<F(Args...)>::type;

        auto task = make_shared<packaged_task<ReturnType()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );

        future<ReturnType> result = task->get_future();

        {
            unique_lock<mutex> lock(queueMutex);
            if (stop) {
                throw runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace([task](){ (*task)(); });
        }
        condition.notify_one();

        return result;
    }

    ~ThreadPool() {
        stop = true;
        condition.notify_all();

        for (thread& worker : workers) {
            worker.join();
        }
    }

    size_t getQueueSize() const {
        unique_lock<mutex> lock(queueMutex);
        return tasks.size();
    }
};

// ========================================
// 工作量类（模拟耗时的计算任务）
// ========================================

class Workload {
private:
    string name;

public:
    explicit Workload(string n) : name(move(n)) {}

    void operator()() const {
        cout << "开始执行任务: " << name << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "完成执行任务: " << name << endl;
    }

    int compute(int n) {
        this_thread::sleep_for(chrono::milliseconds(100));
        int result = 0;
        for (int i = 0; i <= n; i++) {
            result += i;
        }
        cout << name << " 计算结果: " << result << endl;
        return result;
    }

    const string& getName() const { return name; }
};

// ========================================
// 示例1：基本使用
// ========================================

void example1_BasicUsage() {
    cout << "=== 示例1：基本使用 ===" << endl;

    ThreadPool pool(4);
    vector<future<int>> results;

    // 提交 8 个任务
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueueWithResult([i] {
                cout << "任务 " << i << " 正在执行，线程 ID: "
                     << this_thread::get_id() << endl;
                this_thread::sleep_for(chrono::milliseconds(100));
                return i * i;
            })
        );
    }

    // 等待所有任务完成并获取结果
    for (auto&& result : results) {
        cout << "结果: " << result.get() << endl;
    }

    cout << endl;
}

// ========================================
// 示例2：批量处理
// ========================================

void example2_BulkProcessing() {
    cout << "=== 示例2：批量处理 ===" << endl;

    ThreadPool pool(3);

    vector<Workload> workloads = {
        Workload("任务A"),
        Workload("任务B"),
        Workload("任务C"),
        Workload("任务D"),
        Workload("任务E")
    };

    for (auto& workload : workloads) {
        pool.enqueue(workload);
    }

    // 等待所有任务完成
    this_thread::sleep_for(chrono::seconds(1));
    cout << endl;
}

// ========================================
// 示例3：带返回值的任务
// ========================================

void example3_TaskWithReturn() {
    cout << "=== 示例3：带返回值的任务 ===" << endl;

    ThreadPool pool(4);
    vector<future<int>> results;

    for (int i = 1; i <= 5; ++i) {
        Workload workload("任务" + to_string(i));
        results.emplace_back(
            pool.enqueueWithResult(&Workload::compute, &workload, i * 10)
        );
    }

    // 获取所有结果
    int sum = 0;
    for (auto& result : results) {
        sum += result.get();
    }
    cout << "所有任务结果之和: " << sum << endl;
    cout << endl;
}

// ========================================
// 示例4：计算密集型任务
// ========================================

void example4_ComputeIntensive() {
    cout << "=== 示例4：计算密集型任务 ===" << endl;

    ThreadPool pool(4);
    vector<future<long long>> results;

    auto startTime = chrono::high_resolution_clock::now();

    // 并行计算斐波那契数列
    for (int i = 30; i <= 40; ++i) {
        results.emplace_back(
            pool.enqueueWithResult([i] {
                // 递归计算斐波那契数列（计算量大）
                function<long long(int)> fib = [&](int n) {
                    if (n <= 1) return (long long)n;
                    return fib(n - 1) + fib(n - 2);
                };
                auto start = chrono::high_resolution_clock::now();
                long long result = fib(i);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
                cout << "fib(" << i << ") = " << result
                     << ", 耗时: " << duration.count() << "ms" << endl;
                return result;
            })
        );
    }

    // 等待所有任务完成
    for (auto& result : results) {
        result.get();
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto totalDuration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    cout << "总耗时: " << totalDuration.count() << "ms" << endl;
    cout << endl;
}

// ========================================
// 示例5：生产者-消费者模式
// ========================================

atomic<bool> producerDone(false);

void producer(ThreadPool& pool, int count) {
    cout << "生产者开始生产 " << count << " 个任务" << endl;

    for (int i = 0; i < count; ++i) {
        pool.enqueue([i] {
            cout << "消费者处理任务 " << i << endl;
            this_thread::sleep_for(chrono::milliseconds(50));
        });

        // 模拟生产间隔
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    producerDone = true;
    cout << "生产者完成生产" << endl;
}

void consumer(ThreadPool& pool) {
    while (!producerDone || pool.getQueueSize() > 0) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "消费者完成消费" << endl;
}

void example5_ProducerConsumer() {
    cout << "=== 示例5：生产者-消费者模式 ===" << endl;

    ThreadPool pool(3);

    thread producerThread(producer, ref(pool), 10);
    thread consumerThread(consumer, ref(pool));

    producerThread.join();
    consumerThread.join();

    cout << endl;
}

// ========================================
// 主函数
// ========================================

int main() {
    example1_BasicUsage();
    example2_BulkProcessing();
    example3_TaskWithReturn();
    example4_ComputeIntensive();
    example5_ProducerConsumer();

    cout << "所有示例执行完成" << endl;

    return 0;
}
