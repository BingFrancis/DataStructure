#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
#include <thread>
#include <condition_variable>
#include <memory>
#include <future>
#include <utility>
#include <atomic>

using namespace std;

using Task = function<void()>;

mutex cout_mutex;
mutex cout_mutex1;

template<typename T> 
class SafeQueue
{
private:
    queue<T> mqueue;
    mutable mutex mtx;
    condition_variable cond;
public:
    SafeQueue();
    void push(T value) {
        unique_lock<mutex> lock(mtx);
        mqueue.push(move(value));
        cond.notify_one();
    }

    T pop (){
        unique_lock<mutex> lock(mtx);
        cond.wait(lock, [this]{ return !mqueue.empty(); });
        T tmp = move(mqueue.front());
        mqueue.pop();
        return tmp;
    }

    bool empty () const {
        unique_lock<mutex> lock(mtx);
        return mqueue.empty();
    }


    bool try_pop (T& value) {
        unique_lock<mutex> lock(mtx);
        if (mqueue.empty()) {
            return false;
        }
        value = move(mqueue.front());
        mqueue.pop();
        return true;
    }

    void wait_and_pop(T& value) {
        unique_lock<mutex> lock(mtx);
        cond.wait(lock, [this]{ return !mqueue.empty(); });
        value = move(mqueue.front());
        mqueue.pop();
    }
    size_t size () {
        unique_lock<mutex> lock(mtx);
        return mqueue.size();
    }

    ~SafeQueue();
};


class ThreadPool {
private:
    vector<thread> workers;
    queue<Task> tasks;
    mutex queue_mutex;
    condition_variable condition;
    bool stop;
public:
    ThreadPool(size_t num_threads = thread::hardware_concurrency()):stop(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this]{
                while (true) {
                    Task task;
                    {
                        unique_lock<mutex> lock(queue_mutex);
                        condition.wait(lock, [this]{ 
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
            try{
                tasks.emplace([task](){ (*task)(); });
            }catch() {
                throw std::runtime_error("内存分配失败: " + std::string(e.what()));
            }
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
};

void test_thread_pool() {
    cout << "\n====== 测试线程池 ======" << endl;
    ThreadPool pool(4);
    vector<future<int>> results;
    for (int i = 0; i < 8; ++i){
        results.emplace_back(
            pool.enqueue([i]{
                {
                unique_lock<mutex> lock(cout_mutex); 
                cout << "任务 " << i << "在线程 " << this_thread::get_id() << " 执行" << endl;
                }
                this_thread::sleep_for(chrono::milliseconds(100));
                return i * i; 
            })
        );
    }
}

int	main(int argc, char **argv)
{
    test_thread_pool();
    return 0;
}
