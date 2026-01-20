# 06 - 多线程并发

## 📚 模块概述

多线程编程是现代软件开发的核心技能之一。本模块涵盖 C++ 多线程的基础知识、同步机制、任务调度、线程池等高级技术。掌握这些技术可以让你编写高效、安全、可扩展的并发程序。

---

## 📁 文件列表

| 文件名 | 描述 | 学习时长 |
|--------|------|----------|
| `threading_demo.cpp` | 多线程基础演示 | 1-2 小时 |
| `thread_guard.cpp` | RAII 线程管理 | 1 小时 |
| `thread_test.cpp` | 线程测试示例 | 1 小时 |
| `multithreading.cpp` | 多线程综合应用 | 2-3 小时 |
| `safe_queue.cpp` | 线程安全队列 | 2-3 小时 |
| `My_Safe_Queue.cpp` | 自定义安全队列 | 2-3 小时 |
| `task_queue.cpp` | 任务队列模式 | 2-3 小时 |
| `future.cpp` | std::future 和 std::promise | 2-3 小时 |
| `smartThreadPool.cpp` | 智能线程池 | 3-4 小时 |
| `enhanced_thread_pool.cpp` | 增强型线程池 | 3-4 小时 |
| `smart_ptr_threading.cpp` | 智能指针与多线程 | 2-3 小时 |

---

## 🎯 学习目标

- 理解多线程的基本概念和 C++11 线程支持
- 掌握线程同步机制（互斥锁、条件变量）
- 学会使用 std::future 和 std::promise 实现异步编程
- 掌握线程安全的数据结构和容器
- 学会设计和实现线程池
- 理解智能指针在多线程中的应用
- 掌握避免死锁和竞争条件的技巧

---

## 📖 学习顺序建议

### 第一步：多线程基础

#### 1. 创建线程 (threading_demo.cpp)
```cpp
#include <thread>
#include <iostream>

void hello() {
    std::cout << "Hello from thread!\n";
}

int main() {
    std::thread t(hello);  // 创建线程
    t.join();              // 等待线程结束

    // 或者
    std::thread t2(hello);
    t2.detach();           // 分离线程（不等待）
}
```

**关键概念：**
- `std::thread`：线程类
- `join()`：等待线程完成
- `detach()`：分离线程，独立运行
- 线程不可复制，只能移动

#### 2. RAII 线程管理 (thread_guard.cpp)
```cpp
class thread_guard {
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_) : t(t_) {}
    ~thread_guard() {
        if (t.joinable()) {
            t.join();  // 析构时自动 join
        }
    }
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;
};
```

**优势：**
- 自动管理线程生命周期
- 异常安全
- 避免忘记 join 或 detach

---

### 第二步：线程同步

#### 1. 互斥锁 (std::mutex)
```cpp
#include <mutex>

std::mutex mtx;
int counter = 0;

void increment() {
    std::lock_guard<std::mutex> lock(mtx);  // RAII 锁
    ++counter;
}
```

**锁类型：**
- `std::mutex`：基本互斥锁
- `std::recursive_mutex`：可递归锁
- `std::timed_mutex`：带超时的锁
- `std::shared_mutex`：读写锁 (C++17)

#### 2. 条件变量 (std::condition_variable)
```cpp
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });  // 等待条件
    // 执行工作
}

void signal() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();  // 或 notify_all()
}
```

#### 3. 原子操作 (std::atomic)
```cpp
#include <atomic>

std::atomic<int> counter{0};

void increment() {
    ++counter;  // 原子操作，无需锁
}
```

---

### 第三步：线程安全容器

#### 1. 线程安全队列 (safe_queue.cpp)
```cpp
template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex mtx;
    std::queue<T> queue;
    std::condition_variable cv;

public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(value));
        cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{ return !queue.empty(); });
        T value = std::move(queue.front());
        queue.pop();
        return value;
    }
};
```

#### 2. 自定义安全队列 (My_Safe_Queue.cpp)
- 更丰富的 API
- 支持 try_pop、wait_and_pop 等多种操作
- 支持容量限制和超时

---

### 第四步：异步编程

#### 1. std::future 和 std::promise (future.cpp)
```cpp
#include <future>

void calculate(std::promise<int> prom) {
    int result = 42;
    prom.set_value(result);  // 设置结果
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(calculate, std::move(prom));

    int value = fut.get();  // 获取结果（阻塞）
    t.join();

    // 或者使用 std::async
    auto future2 = std::async(std::launch::async, []{
        return 100;
    });
    std::cout << future2.get() << '\n';
}
```

#### 2. std::packaged_task
```cpp
std::packaged_task<int(int, int)> task([](int a, int b){
    return a + b;
});

std::future<int> result = task.get_future();

std::thread t(std::move(task), 10, 20);
t.join();

int sum = result.get();  // 30
```

---

### 第五步：线程池

#### 1. 基本线程池 (smartThreadPool.cpp)
```cpp
class ThreadPool {
private:
    std::vector<std::thread> workers;
    ThreadSafeQueue<std::function<void()>> tasks;
    bool stop;

public:
    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this]{
                while (true) {
                    auto task = tasks.pop();
                    if (!task) break;
                    task();
                }
            });
        }
    }

    template<typename F>
    void enqueue(F&& f) {
        tasks.push(std::forward<F>(f));
    }

    ~ThreadPool() {
        tasks.stop();
        for (auto& worker : workers) {
            worker.join();
        }
    }
};
```

#### 2. 增强型线程池 (enhanced_thread_pool.cpp)
- 支持返回值
- 支持任务优先级
- 支持超时和取消
- 更好的异常处理

---

### 第六步：智能指针与多线程 (smart_ptr_threading.cpp)

#### 1. std::shared_ptr 的线程安全
```cpp
// 读操作是线程安全的
std::shared_ptr<T> copy = ptr;  // 原子操作

// 写操作需要同步
std::mutex mtx;
std::shared_ptr<T> ptr;

void update(std::shared_ptr<T> new_ptr) {
    std::lock_guard<std::mutex> lock(mtx);
    ptr = std::move(new_ptr);
}
```

#### 2. std::atomic_shared_ptr (C++20)
```cpp
std::atomic_shared_ptr<T> ptr;
auto copy = ptr.load();  // 原子操作
ptr.store(new_ptr);
```

---

## 🔗 知识图谱

```
多线程并发
├── 基础
│   ├── std::thread
│   ├── 创建和销毁线程
│   └── RAII 线程管理
├── 同步机制
│   ├── 互斥锁 (std::mutex)
│   ├── 条件变量 (condition_variable)
│   ├── 读写锁 (shared_mutex)
│   └── 原子操作 (std::atomic)
├── 线程安全容器
│   ├── 线程安全队列
│   ├── 并发容器
│   └── 自定义安全数据结构
├── 异步编程
│   ├── std::future
│   ├── std::promise
│   ├── std::packaged_task
│   └── std::async
├── 线程池
│   ├── 基本线程池
│   ├── 任务队列
│   ├── 增强型线程池
│   └── 任务调度策略
└── 智能指针与并发
    ├── shared_ptr 的线程安全性
    ├── atomic_shared_ptr
    └── 无锁数据结构
```

---

## 💡 最佳实践

### ✅ 推荐做法

1. **使用 RAII 管理锁**：
   ```cpp
   std::lock_guard<std::mutex> lock(mtx);  // 自动释放
   ```

2. **最小化锁的粒度**：
   ```cpp
   // 好：锁范围小
   {
       std::lock_guard<std::mutex> lock(mtx);
       // 仅保护关键区
   }
   ```

3. **使用条件变量避免忙等待**：
   ```cpp
   cv.wait(lock, []{ return ready; });  // 等待通知
   ```

4. **优先使用高级抽象（线程池、future）**：
   ```cpp
   auto result = std::async([]{ return compute(); });
   ```

### ❌ 避免的做法

1. **忘记 join 或 detach**：
   ```cpp
   std::thread t(func);
   // 忘记 join/detach，导致 std::terminate
   ```

2. **嵌套锁导致死锁**：
   ```cpp
   std::lock_guard<std::mutex> lock1(mtx1);
   std::lock_guard<std::mutex> lock2(mtx2);  // 死锁风险
   ```

3. **在锁内执行耗时操作**：
   ```cpp
   std::lock_guard<std::mutex> lock(mtx);
   do_slow_operation();  // 阻塞其他线程
   ```

4. **共享指针的误用**：
   ```cpp
   // 多线程中只修改 shared_ptr 本身需要同步
   // 通过 shared_ptr 访问对象需要额外的同步
   ```

---

## 🔧 编译运行

```bash
# 编译（需要链接 pthread）
g++ -std=c++17 -pthread threading_demo.cpp -o threading_demo && ./threading_demo
g++ -std=c++17 -pthread smartThreadPool.cpp -o thread_pool && ./thread_pool
```

---

## 📚 推荐资源

- **书籍**：《C++ Concurrency in Action》- Anthony Williams
- **书籍**：《Effective Modern C++》- Scott Meyers（第 7 章）
- **文档**：[cppreference - Thread support library](https://en.cppreference.com/w/cpp/thread)
- **视频**：C++ Concurrency - CppCon 演讲

---

## ✅ 学习检查清单

- [ ] 理解线程的基本概念和创建方法
- [ ] 掌握互斥锁、条件变量等同步机制
- [ ] 理解 RAII 在线程管理中的应用
- [ ] 掌握线程安全队列的实现
- [ ] 理解 std::future 和 std::promise 的使用
- [ ] 能够设计和实现基本的线程池
- [ ] 了解智能指针在多线程中的应用
- [ ] 掌握避免死锁和竞争条件的技巧
- [ ] 能够编写简单的并发程序

---

## 🚀 完成整个项目

恭喜你完成了所有 6 个模块的学习！现在你已经掌握了：
1. 基础数据结构
2. 智能指针
3. 内存管理
4. 模板元编程
5. 字符串处理
6. 多线程并发

这些技能将让你能够编写高效、安全、可扩展的 C++ 程序。继续实践，深入探索，成为更优秀的 C++ 开发者！
