#include <iostream>
#include <future>
#include <chrono>
#include <thread>


//移动语义与完美转发
class StringWrapper {
private:
    std::mutex mutex;
    std::string data;

public:
    StringWrapper(const std::string &data_) : data(data_) {
        std::cout << "拷贝构造函数" << std::endl;
    };

    StringWrapper(StringWrapper && other) noexcept :data(std::move(other.data)){
        std::cout << "移动构造" << std::endl;
    };
};

void promise_future_example () {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread worker([&prom](){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        prom.set_value(42);
    });

    try
    {
        int result = fut.get();
        std::cout << "Received: " << result << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    worker.join();
}

void packaged_task_example () {
     // 创建packaged_task
     std::packaged_task<int(int, int)> task([](int a, int b) {
        return a + b;
     });

     auto fut = task.get_future();

     std::thread task_thread(std::move(task), 10, 20);

     std::cout << "packaged_task get data :" << fut.get() << std::endl;

     task_thread.join();
}



int main() {
    // promise_future_example();
    packaged_task_example();
    return 0;
}