# 06_multithreading - 多线程并发

本模块学习 C++ 多线程编程，包括线程管理、同步机制和线程池。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `threading_demo.cpp` | 线程基础（创建、等待、分离） | ⭐⭐ |
| `thread_guard.cpp` | RAII 线程守卫 | ⭐⭐⭐ |
| `multithreading.cpp` | 多线程综合示例 | ⭐⭐⭐ |
| `thread_test.cpp` | 生产者-消费者模型 | ⭐⭐⭐ |
| `safe_queue.cpp` | 线程安全队列 | ⭐⭐⭐⭐ |
| `my_safe_queue.cpp` | 安全队列 + 线程池实现 | ⭐⭐⭐⭐ |
| `task_queue.cpp` | 任务队列 | ⭐⭐⭐⭐ |
| `future.cpp` | Future/Promise 异步编程 | ⭐⭐⭐⭐ |
| `smart_thread_pool.cpp` | 智能线程池（RAII） | ⭐⭐⭐⭐ |
| `enhanced_thread_pool.cpp` | 增强线程池 | ⭐⭐⭐⭐⭐ |
| `smart_ptr_threading.cpp` | 智能指针与多线程 | ⭐⭐⭐⭐ |
| `smart_ptr_thread_pool_demo.cpp` | 智能指针线程池演示 | ⭐⭐⭐⭐ |

## 🎯 学习目标

- 掌握线程的创建和管理
- 理解线程同步机制（互斥锁、条件变量）
- 实现线程安全的数据结构
- 掌握线程池的设计和实现
- 学会使用 Future/Promise

## 📖 核心概念

### 1. 线程基础

```cpp
#include <thread>

void task(int id) {
    cout << "Thread " << id << endl;
}

thread t1(task, 1);
thread t2(task, 2);
t1.join();
t2.join();
```

### 2. 互斥锁

```cpp
mutex mtx;

void safe_increment(int& counter) {
    lock_guard<mutex> lock(mtx);  // 自动加锁/解锁
    counter++;
}
```

### 3. 条件变量

```cpp
mutex mtx;
condition_variable cv;
bool ready = false;

// 等待方
unique_lock<mutex> lock(mtx);
cv.wait(lock, [] { return ready; });

// 通知方
{
    lock_guard<mutex> lock(mtx);
    ready = true;
}
cv.notify_one();
```

### 4. Future/Promise

```cpp
future<int> f = async(launch::async, [](int a, int b) {
    return a + b;
});
int result = f.get();  // 阻塞等待结果
```

### 5. 原子操作

```cpp
atomic<int> counter{0};
counter++;              // 原子自增
counter.fetch_add(10);  // 原子加法
```

## ⚠️ 常见问题

### 数据竞争

```cpp
// ❌ 数据竞争
int counter = 0;

// ✅ 解决：使用原子操作
atomic<int> counter{0};
```

### 死锁预防

```cpp
// 一次性锁定多个互斥量
lock(mtx1, mtx2);
lock_guard<mutex> lk1(mtx1, adopt_lock);
lock_guard<mutex> lk2(mtx2, adopt_lock);
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 safe_queue.cpp -o sq -pthread && ./sq
g++ -std=c++17 future.cpp -o future -pthread && ./future
g++ -std=c++17 smart_thread_pool.cpp -o pool -pthread && ./pool
```

## 📖 学习建议顺序

1. `threading_demo.cpp` - 线程基础
2. `thread_guard.cpp` - RAII 管理
3. `thread_test.cpp` - 生产者-消费者
4. `safe_queue.cpp` - 线程安全队列
5. `future.cpp` - 异步编程
6. `smart_thread_pool.cpp` - 线程池实现

## ✅ 学习检查清单

- [ ] 能创建和管理线程
- [ ] 理解互斥锁和条件变量
- [ ] 能实现线程安全的数据结构
- [ ] 理解原子操作
- [ ] 能使用 Future/Promise
- [ ] 能设计和实现线程池
- [ ] 能识别和避免死锁

## 🔗 相关模块

- `02_smart_pointers` - 智能指针的线程安全性
- `03_memory_management` - 多线程中的内存管理

---

**多线程 = C++ 工程师的必备技能！** 🚀
