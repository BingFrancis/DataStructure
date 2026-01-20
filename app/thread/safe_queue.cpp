#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <functional>
#include <condition_variable>
#include <memory>
#include <future>
#include <atomic>
#include <utility>

using namespace std;

//任务类型
using Task = function<void()>;

template <typename T>
class SafeQueue {
public:
    SafeQueue() {}
    void push(T value){
        unique_lock<mutex> lock(mtx);
        mqueue.push(move(value));
        cond.notify_one();
    }

    T pop() {
        unique_lock<mutex> lock(mtx);
        cond.wait(lock, [this]{return !mqueue.empty(); });
        T tmp = move(mqueue.front());
        mqueue.pop();
        return tmp;
    }

    bool empty () const{
        unique_lock<mutex> lock(mtx);
        return mqueue.empty();
    }

    // 尝试从队列中弹出元素，如果队列为空则立即返回
    bool try_pop(T& value) {
        unique_lock<mutex> lock(mtx);
        if (mqueue.empty()) {
            return false;
        }
        value = move(mqueue.front());
        mqueue.pop();
        return true;
    }

    // 等待并弹出元素
    void wait_and_pop(T& value) {
        unique_lock<mutex> lock(mtx);
        cond.wait(lock, [this]{ return !mqueue.empty(); });
        value = move(mqueue.front());
        mqueue.pop();
    }

    size_t size() const {
        unique_lock<mutex> lock(mtx);
        return mqueue.size();
    }

private:
    queue<T> mqueue;
    mutable  mutex mtx;
    condition_variable cond;
};

// 工作线程池类
class ThreadPool {
public:
    ThreadPool(size_t num_threads = thread::hardware_concurrency()) : stop(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    Task task;
                    {
                        unique_lock<mutex> lock(queue_mutex);
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

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        future<return_type> res = task->get_future();
        {
            unique_lock<mutex> lock(queue_mutex);
            if (stop) {
                throw runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace([task](){ (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (thread &worker : workers) {
            worker.join();
        }
    }

private:
    vector<thread> workers;
    queue<Task> tasks;
    mutex queue_mutex;
    condition_variable condition;
    bool stop;
};






// 测试函数
void test_safe_queue() {
    cout << "=== 测试线程安全队列 ===" << endl;
    
    SafeQueue<int> queue;
    
    // 测试基本功能
    cout << "1. 测试基本push/pop操作..." << endl;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    
    cout << "队列大小: " << queue.size() << endl;
    
    int value;
    queue.wait_and_pop(value);
    cout << "弹出值: " << value << endl;
    
    queue.wait_and_pop(value);
    cout << "弹出值: " << value << endl;
    
    // 测试try_pop
    cout << "2. 测试try_pop操作..." << endl;
    bool success = queue.try_pop(value);
    if (success) {
        cout << "try_pop成功，值: " << value << endl;
    } else {
        cout << "try_pop失败" << endl;
    }
    
    // 测试空队列
    success = queue.try_pop(value);
    cout << "空队列try_pop结果: " << (success ? "成功" : "失败") << endl;
    
    cout << "队列是否为空: " << (queue.empty() ? "是" : "否") << endl;
}

// 多线程测试
void test_multithreaded() {
    cout << "\n=== 测试多线程环境 ===" << endl;
    
    SafeQueue<int> queue;
    atomic<int> counter{0};
    const int num_producers = 3;
    const int num_consumers = 2;
    const int items_per_producer = 100;
    
    vector<thread> producers;
    vector<thread> consumers;
    
    // 创建生产者线程
    for (int i = 0; i < num_producers; ++i) {
        producers.emplace_back([&queue, i] {
            for (int j = 0; j < items_per_producer; ++j) {
                queue.push(i * items_per_producer + j);
                this_thread::sleep_for(chrono::microseconds(10));
            }
        });
    }
    
    // 创建消费者线程
    for (int i = 0; i < num_consumers; ++i) {
        consumers.emplace_back([&queue, &counter]{
            int value;
            while (counter < num_producers * items_per_producer) {
                if (queue.try_pop(value)) {
                    ++counter;
                    this_thread::sleep_for(chrono::microseconds(5));
                }
            }
        });
    }
    
    // 等待所有线程完成
    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();
    
    cout << "生产总数: " << num_producers * items_per_producer << endl;
    cout << "消费总数: " << counter << endl;
    cout << "队列最终大小: " << queue.size() << endl;
    cout << "测试完成!" << endl;
}

// 线程池测试
void test_thread_pool() {
    cout << "\n=== 测试线程池 ===" << endl;
    
    ThreadPool pool(4);
    vector<future<int>> results;
    
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                cout << "任务 " << i << " 在线程 " << this_thread::get_id() << " 执行" << endl;
                this_thread::sleep_for(chrono::milliseconds(100));
                return i * i;
            })
        );
    }
    
    for (auto& result : results) {
        cout << "结果: " << result.get() << endl;
    }
    
    cout << "线程池测试完成!" << endl;
}

int main() {
    cout << "=== 线程安全队列测试程序 ===" << endl;
    
    try {
        // test_safe_queue();
        test_multithreaded();
        // test_thread_pool();
        
        cout << "\n=== 所有测试完成 ===" << endl;
    } catch (const exception& e) {
        cerr << "错误: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}