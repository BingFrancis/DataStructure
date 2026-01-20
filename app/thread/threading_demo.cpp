#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <future>
#include <atomic>

std::mutex mutex;
void worker(int id) {
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "Worker " << id << " is running" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // std::lock_guard<std::mutex> lock2(mutex);
    std::cout << "Worker " << id << " is done" << std::endl;
}


int main() {
    // std::vector<std::thread> threads;
    // for (int i = 0; i < 20; ++i) {
    //     threads.emplace_back(worker,i);
    // }

    // for (auto& thread : threads) {
    //     thread.join();
    // }


    std::atomic<int> counter(0);
    std::vector<std::thread> atomic_threads;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) {
        atomic_threads.emplace_back([&counter]() {
            for (int j = 0; j < 100; ++j) {
                counter++;
            }
        });
    }

    for (auto& thread : atomic_threads) {
        thread.join();
    }
    std::cout << "Counter value: " << counter << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}