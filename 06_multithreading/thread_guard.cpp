// ============================================================================
// 06 - Multithreading: 线程守卫 (Thread Guard)
// ============================================================================
// 演示 RAII 风格的线程管理：自动 join 或 detach，防止资源泄漏。
// ============================================================================

#include <thread>
#include <iostream>
#include <utility>

class ThreadGuard {
public:
    // 默认构造
    ThreadGuard() = default;
    
    // 从 thread 构造（接管所有权）
    explicit ThreadGuard(std::thread t) 
        : thread_(std::move(t))  // 移动构造 thread
    {
        if (!thread_.joinable()) {
            throw std::logic_error("Not a joinable thread");
        }
    }
    
    // ============================================
    // 【关键】移动构造
    // ============================================
    ThreadGuard(ThreadGuard&& other) noexcept
        : thread_(std::move(other.thread_))  // 移动底层 thread
    {
        // other.thread_ 现在为空（有效但未指定状态）
        std::cout << "ThreadGuard moved (construct)\n";
    }
    
    // ============================================
    // 【关键】移动赋值
    // ============================================
    ThreadGuard& operator=(ThreadGuard&& other) noexcept {
        std::cout << "ThreadGuard moved (assign)\n";
        
        if (this != &other) {
            // 1. 先清理当前资源（如果持有线程，先 join）
            if (thread_.joinable()) {
                thread_.join();
            }
            
            // 2. 窃取资源
            thread_ = std::move(other.thread_);
        }
        return *this;
    }
    
    // 禁用拷贝
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
    
    // 析构
    ~ThreadGuard() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    // 显式控制
    std::thread::id get_id() const {
        return thread_.get_id();
    }
    
    bool joinable() const {
        return thread_.joinable();
    }

private:
    std::thread thread_;  // 成员是 move-only 类型
};

// ============================================
// 使用移动语义管理线程
// ============================================
ThreadGuard createWorker() {
    return ThreadGuard(std::thread([]() {
        std::cout << "Worker running\n";
    }));
    // 这里发生：thread 移动构造 → ThreadGuard 移动构造
    // 全程 noexcept，高效
}

void moveDemo() {
    auto guard1 = createWorker();  // 移动语义链
    
    ThreadGuard guard2;  // 空 guard
    guard2 = std::move(guard1);  // 移动赋值
    
    // guard1 现在为空，guard2 持有线程
    
} // guard2 析构，join 线程；guard1 析构，无操作


int	main(int argc, char **argv)
{
    moveDemo();
    return 0;
}
