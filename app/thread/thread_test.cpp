#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <condition_variable>
#include <queue>
using namespace std;
using Task = function<void()>;
class TaskQueue {
public:
    TaskQueue() {}
    void push(Task task) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.push_back(task);
        m_cond.notify_one();
    }

    Task pop() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this] { return !m_tasks.empty(); } );
        if (m_tasks.empty()) {
            return nullptr;
        }

        Task task = m_tasks.back();
        m_tasks.pop_back();
        return task;
    }
    bool empty()  {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_tasks.empty();
    }
    
private:
    vector<Task> m_tasks;
    mutex m_mutex;
    condition_variable m_cond;
};



int main() {
    TaskQueue task_queue;
    for (int i = 0; i < 10; i++) {
        task_queue.push([i] {
            int res = i * i;
            cout << "Task " << i << " result: " << res  << endl;
        });
    }

    thread pro_thread([&task_queue]{
        while (true){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if(task_queue.empty()) {
                cout << "task queue is empty" << endl;
                continue;
            }
            Task task = task_queue.pop();
            if (task) {
                task();
            }
        }
    });

    thread con_thread([&task_queue]{
        while (true) {
            task_queue.push([]{
                std::this_thread::sleep_for(std::chrono::seconds(1));
                cout << "new task" << endl;
            });
        }
    });

    pro_thread.join();
    con_thread.join ();
    return 0;
}