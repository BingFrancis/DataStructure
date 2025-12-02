# C++ 进阶练习

本目录包含了多个C++进阶编程练习，涵盖了现代C++的重要特性和技术。

## 文件列表

1. **smart_pointers.cpp** - 智能指针练习
   - unique_ptr 的使用
   - shared_ptr 的使用
   - weak_ptr 的使用（解决循环引用问题）
   - 自定义删除器

2. **stl_containers.cpp** - STL容器和算法练习
   - 序列容器（vector, list, deque）
   - 关联容器（set, map, unordered_map）
   - 容器适配器（stack, queue, priority_queue）
   - STL常用算法
   - 函数对象和Lambda表达式

3. **template_programming.cpp** - 模板编程练习
   - 函数模板
   - 类模板
   - 模板特化和偏特化
   - 可变参数模板
   - 模板元编程
   - SFINAE
   - 类型特征

4. **multithreading.cpp** - 多线程编程练习
   - 基本线程操作
   - 互斥锁和条件变量
   - 生产者消费者模型
   - 原子操作
   - future和promise
   - 简单线程池实现

5. **modern_cpp_features.cpp** - 现代C++特性练习
   - auto关键字和范围for循环
   - nullptr和constexpr
   - 右值引用和移动语义
   - Lambda表达式
   - tuple和tie
   - optional（C++17）
   - variant（C++17）
   - any（C++17）
   - 结构化绑定（C++17）
   - if constexpr（C++17）

## C++11 专题练习

6. **cpp11_rvalue_forwarding.cpp** - C++11右值引用和完美转发
   - 右值引用的基本概念
   - 移动构造函数和移动赋值运算符
   - 完美转forward的使用
   - 引用折叠规则
   - 标准库容器中的移动语义

7. **cpp11_regex.cpp** - C++11正则表达式
   - 基本正则表达式匹配
   - 正则表达式搜索
   - 正则表达式替换
   - 使用捕获组提取信息
   - 输入格式验证
   - 自定义匹配标志

8. **cpp11_chrono_random.cpp** - C++11时间库和随机数
   - 时间点和时间间隔
   - 高精度计时
   - 日期时间格式化
   - 随机数引擎和分布
   - 不同类型的概率分布
   - 随机洗牌算法

9. **cpp11_enum_class.cpp** - C++11枚举类和委托构造函数
   - 强类型枚举（enum class）
   - 指定枚举底层类型
   - 枚举前向声明
   - 委托构造函数
   - 构造函数继承
   - 综合示例：状态机实现

10. **task_queue.cpp** - 任务队列实现
    - 基本任务队列
    - 带返回值的异步任务队列
    - 优先级任务队列
    - 延迟任务队列
    - 多线程任务处理

## C++11 高级并发专题

11. **enhanced_thread_pool.cpp** - 增强线程池实现
    - 支持优先级的任务队列
    - 延迟任务执行
    - 批量任务处理
    - 线程池统计信息
    - 线程局部存储
    - 完美转发和变参模板

12. **smart_ptr_threading.cpp** - 智能指针与线程高级用法
    - 线程安全的资源管理器
    - 观察者模式与weak_ptr
    - ThreadSafeSharedPtr实现
    - 异步任务队列与智能指针
    - 线程安全的对象池
    - RAII与智能指针句柄

13. **coroutine_scheduler.cpp** - 协程式任务调度器
    - 协程上下文管理
    - 协程状态机
    - 协程挂起与恢复
    - 协程异常处理
    - 协程辅助函数
    - 并发协程调度

14. **move_semantics_examples.cpp** - 移动语义实际应用
    - 资源所有权转移
    - 性能优化场景
    - 工厂模式应用
    - 容器操作示例
    - 线程间资源传递
    - 状态机实现
    - 性能对比测试

## 编译说明

这些练习使用了C++11/14/17的特性，建议使用支持这些标准的编译器。例如使用g++编译：

```bash
g++ -std=c++17 -pthread filename.cpp -o output
```

对于多线程相关的练习（multithreading.cpp、task_queue.cpp、enhanced_thread_pool.cpp、smart_ptr_threading.cpp、coroutine_scheduler.cpp），需要添加-pthread选项。

## 学习建议

1. 按顺序学习，从智能指针开始，逐步深入到更复杂的主题
2. 运行每个示例并观察输出，理解代码的工作原理
3. 尝试修改代码，添加自己的实现
4. 将学到的知识应用到实际项目中

## 参考资源

- 《C++ Primer》第5版
- 《Effective Modern C++》
- cppreference.com
- 现代 C++ 教程