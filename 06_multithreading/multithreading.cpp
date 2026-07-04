// ============================================================================
// 06 - Multithreading: 线程管理综合示例
// ============================================================================
// 1. 线程创建与 join/detach
// 2. 线程对象的所有权和转移
// 3. RAII 线程管理
// 4. 线程局部存储
// ============================================================================

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>

// 1. 基础线程创建
void task(int id, const std::string& msg) {
    std::cout << "Thread " << id << ": " << msg << std::endl;
}

// 2. 线程对象所有权转移
class ScopedThread {
    std::thread t;
public:
    explicit ScopedThread(std::thread t_) : t(std::move(t_)) {
        if (!t.joinable())
            throw std::logic_error("No thread");
    }
    ~ScopedThread() { t.join(); }
    ScopedThread(const ScopedThread&) = delete;
    ScopedThread& operator=(const ScopedThread&) = delete;
    ScopedThread(ScopedThread&&) = default;
    ScopedThread& operator=(ScopedThread&&) = default;
};

// 3. 线程局部存储 (thread_local)
thread_local int tls_counter = 0;

void increment_tls() {
    ++tls_counter;
    std::cout << "Thread " << std::this_thread::get_id()
              << ": tls_counter = " << tls_counter << std::endl;
}

// 4. 批量线程管理
void batch_thread_demo() {
    std::cout << "\n=== 批量线程管理 ===" << std::endl;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            std::cout << "Worker " << i << " running on thread "
                      << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
}

// 5. 线程休眠与时间管理
void timing_demo() {
    std::cout << "\n=== 时间管理 ===" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "休眠 100ms，实际用时: " << duration.count() << "ms" << std::endl;
}

// 6. 线程转移所有权
void ownership_transfer_demo() {
    std::cout << "\n=== 线程所有权转移 ===" << std::endl;
    
    std::thread t1([]() {
        std::cout << "线程 t1 开始执行" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "线程 t1 执行完毕" << std::endl;
    });
    
    // 转移所有权
    std::thread t2 = std::move(t1);
    std::cout << "t1 是否可 join: " << (t1.joinable() ? "是" : "否") << std::endl;
    std::cout << "t2 是否可 join: " << (t2.joinable() ? "是" : "否") << std::endl;
    
    t2.join();
}

// 7. RAII 线程守卫
void raii_demo() {
    std::cout << "\n=== RAII 线程守卫 ===" << std::endl;
    
    ScopedThread st(std::thread([]() {
        std::cout << "RAII 线程开始执行" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "RAII 线程执行完毕" << std::endl;
    }));
    // 离开作用域时自动 join
    std::cout << "ScopedThread 即将析构..." << std::endl;
}

// 8. 线程局部存储演示
void tls_demo() {
    std::cout << "\n=== 线程局部存储 (thread_local) ===" << std::endl;
    
    std::thread t1([]() {
        increment_tls();
        increment_tls();
    });
    
    std::thread t2([]() {
        increment_tls();
        increment_tls();
        increment_tls();
    });
    
    t1.join();
    t2.join();
    
    std::cout << "每个线程有自己独立的 tls_counter" << std::endl;
}

// 9. 线程标识与硬件并发数
void info_demo() {
    std::cout << "\n=== 系统信息 ===" << std::endl;
    std::cout << "硬件并发数: " << std::thread::hardware_concurrency() << std::endl;
    std::cout << "主线程 ID: " << std::this_thread::get_id() << std::endl;
}

// 10. detach 风险演示
void detach_demo() {
    std::cout << "\n=== detach 风险演示 ===" << std::endl;
    
    // detach 后线程独立运行，但主线程结束可能看不到输出
    std::thread t([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "Detached 线程仍然存活!" << std::endl;
    });
    t.detach();
    
    // 等待 detach 线程完成（实际项目中用其他同步机制）
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "主线程等待 detach 线程完成" << std::endl;
}

int main() {
    std::cout << "=== C++ 多线程编程综合示例 ===" << std::endl;
    
    // 基础
    std::thread t1(task, 1, "Hello from thread 1");
    std::thread t2(task, 2, "Hello from thread 2");
    t1.join();
    t2.join();
    
    // 进阶演示
    info_demo();
    batch_thread_demo();
    timing_demo();
    ownership_transfer_demo();
    raii_demo();
    tls_demo();
    detach_demo();
    
    std::cout << "\n所有演示完成!" << std::endl;
    return 0;
}
