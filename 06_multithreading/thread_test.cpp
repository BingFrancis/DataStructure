// ============================================================================
// 06 - Multithreading: 线程安全任务队列 (Thread-Safe Task Queue)
// ============================================================================
// 演示使用 mutex + condition_variable 实现生产者-消费者模型。
// 支持 FIFO 队列行为、优雅停止 (graceful shutdown)。
// ============================================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <condition_variable>
#include <deque>
#include <atomic>

using namespace std;
using Task = function<void()>;

// 线程安全的任务队列 (FIFO)
class TaskQueue {
public:
    TaskQueue() : m_stop(false) {}

    void push(Task task) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.push_back(std::move(task));
        m_cond.notify_one();
    }

    Task pop() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this] { return m_stop || !m_tasks.empty(); });

        if (m_stop && m_tasks.empty()) {
            return nullptr;
        }
        if (m_tasks.empty()) {
            return nullptr;
        }

        // FIFO: 取队首元素
        Task task = std::move(m_tasks.front());
        m_tasks.pop_front();
        return task;
    }

    bool empty() {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_tasks.empty();
    }

    void stop() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = true;
        m_cond.notify_all();
    }

private:
    deque<Task> m_tasks;         // deque 支持高效的 pop_front
    mutex m_mutex;
    condition_variable m_cond;
    bool m_stop;
};

int main() {
    TaskQueue task_queue;

    // 生产者：持续生成任务
    thread producer([&task_queue] {
        for (int i = 0; i < 10; i++) {
            task_queue.push([i] {
                int res = i * i;
                cout << "[Consumer] Task " << i << " → result: " << res << endl;
            });

            cout << "[Producer] Pushed task " << i << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }

        // 生产完毕后通知消费者停止
        std::this_thread::sleep_for(std::chrono::seconds(1));
        task_queue.stop();
        cout << "[Producer] Stopped." << endl;
    });

    // 消费者：持续消费任务
    thread consumer([&task_queue] {
        while (true) {
            Task task = task_queue.pop();
            if (!task) {
                cout << "[Consumer] No more tasks, exiting." << endl;
                break;
            }
            task();
        }
    });

    producer.join();
    consumer.join();

    cout << "\n=== 生产者-消费者模型演示完成 ===" << endl;
    return 0;
}