// ============================================================================
// 06 - Multithreading: Future/Promise 异步编程
// ============================================================================
// 演示 std::future、std::promise、std::packaged_task 的核心用法。
// ============================================================================

#include <iostream>
#include <future>
#include <chrono>
#include <thread>

void promise_future_example() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread worker([&prom](){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        prom.set_value(42);
    });

    try
    {
        int result = fut.get();
        std::cout << "promise_future received: " << result << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    worker.join();
}

void packaged_task_example() {
     // 创建 packaged_task
     std::packaged_task<int(int, int)> task([](int a, int b) {
        return a + b;
     });

     auto fut = task.get_future();

     std::thread task_thread(std::move(task), 10, 20);

     std::cout << "packaged_task result: " << fut.get() << std::endl;

     task_thread.join();
}


int main() {
    promise_future_example();
    packaged_task_example();
    return 0;
}