# C++ 数据结构与进阶编程

本目录包含多个 C++ 进阶编程练习，涵盖现代 C++ 的重要特性、数据结构实现、内存管理、多线程编程等核心技术。

---

## 📁 项目结构（按学习路径组织）

```
DataStructure/
├── 01_fundamentals/              # 基础数据结构与 C++ 基础
│   ├── README.md
│   ├── linked_list.cpp
│   ├── binary_tree.cpp
│   └── hash_table.cpp
├── 02_smart_pointers/            # 智能指针
│   ├── README.md
│   ├── smart_pointers.cpp
│   └── modern_cpp_project.cpp
├── 03_memory_management/         # 内存管理
│   ├── README.md
│   ├── memory_layout.cpp
│   ├── memory_pool.cpp
│   ├── smart_allocator.cpp
│   ├── object_pool_demo.cpp
│   └── object.h
├── 04_template_metaprogramming/   # 模板与元编程
│   ├── README.md
│   ├── template_fundamentals.cpp
│   ├── function_templates_advanced.cpp
│   └── template_instantiation.cpp
├── 05_string_processing/         # 字符串处理
│   ├── README.md
│   └── string.cpp
├── 06_multithreading/            # 多线程并发
│   ├── README.md
│   ├── threading_demo.cpp
│   ├── thread_guard.cpp
│   ├── thread_test.cpp
│   ├── multithreading.cpp
│   ├── safe_queue.cpp
│   ├── My_Safe_Queue.cpp
│   ├── task_queue.cpp
│   ├── future.cpp
│   ├── smartThreadPool.cpp
│   ├── enhanced_thread_pool.cpp
│   └── smart_ptr_threading.cpp
├── app/                          # 原始代码目录（保留）
└── README.md                     # 本文件
```

---

## 🎯 学习路径

本项目的 6 个模块按照由浅入深的学习路径组织，建议按顺序学习：

### 1️⃣ 01_fundamentals - 基础数据结构
**学习时间**：5-8 小时

**核心内容**：
- 链表（单向、双向）
- 二叉树（遍历、操作）
- 哈希表（冲突处理、扩容）

**学习目标**：
- 掌握基本数据结构的实现
- 理解指针和动态内存
- 熟练使用 STL 容器

👉 [查看详细文档](./01_fundamentals/README.md)

---

### 2️⃣ 02_smart_pointers - 智能指针
**学习时间**：3-5 小时

**核心内容**：
- `std::unique_ptr`（独占所有权）
- `std::shared_ptr`（共享所有权）
- `std::weak_ptr`（解决循环引用）
- 现代 C++ 特性（auto、Lambda、nullptr 等）

**学习目标**：
- 理解 RAII 原则
- 掌握智能指针的使用
- 避免内存泄漏和悬空指针

👉 [查看详细文档](./02_smart_pointers/README.md)

---

### 3️⃣ 03_memory_management - 内存管理
**学习时间**：9-13 小时

**核心内容**：
- C++ 内存布局（栈、堆、代码段、数据段）
- 内存池实现
- 5 种高级分配器（Arena、Stack、FreeList、Proxy、STL 包装）
- 对象池设计

**学习目标**：
- 理解内存管理机制
- 掌握高性能内存分配技术
- 了解内存优化策略

👉 [查看详细文档](./03_memory_management/README.md)

---

### 4️⃣ 04_template_metaprogramming - 模板与元编程
**学习时间**：6-9 小时

**核心内容**：
- 函数模板和类模板
- 完美转发和可变参数模板
- SFINAE 原则
- 类型萃取和编译期计算
- C++20 Concepts

**学习目标**：
- 掌握模板编程
- 理解泛型编程思想
- 能够编写通用的、类型安全的代码

👉 [查看详细文档](./04_template_metaprogramming/README.md)

---

### 5️⃣ 05_string_processing - 字符串处理
**学习时间**：2-3 小时

**核心内容**：
- C 风格字符串 vs std::string
- 字符串与数值类型转换
- 字符串查找、替换、分割
- 正则表达式
- std::string_view（零拷贝）

**学习目标**：
- 熟练使用 std::string
- 掌握字符串高级操作
- 了解性能优化技巧

👉 [查看详细文档](./05_string_processing/README.md)

---

### 6️⃣ 06_multithreading - 多线程并发
**学习时间**：18-27 小时

**核心内容**：
- std::thread 基础
- 同步机制（互斥锁、条件变量、原子操作）
- 线程安全队列
- std::future 和 std::promise（异步编程）
- 线程池（基础版和增强版）
- 智能指针与多线程

**学习目标**：
- 理解多线程编程模型
- 掌握线程同步技术
- 能够设计并发程序
- 避免死锁和竞争条件

👉 [查看详细文档](./06_multithreading/README.md)

---

## 🔧 编译说明

### 基础编译（非多线程）
```bash
g++ -std=c++17 filename.cpp -o output && ./output
```

### 多线程项目编译
```bash
g++ -std=c++17 -pthread filename.cpp -o output && ./output
```

### 优化编译
```bash
g++ -std=c++17 -O2 -Wall filename.cpp -o output && ./output
```

### 编译参数说明

| 参数 | 说明 |
|------|------|
| `-std=c++17` | 使用 C++17 标准（也可用 c++11、c++14、c++20） |
| `-pthread` | 链接 POSIX 线程库（多线程项目必需） |
| `-O2` | 优化等级 2（平衡编译时间和性能） |
| `-O3` | 最高优化等级（更激进优化） |
| `-Wall` | 显示所有警告 |
| `-Wextra` | 额外的警告 |
| `-g` | 生成调试信息（用于 gdb） |

---

## 📊 知识图谱

```
┌─────────────────────────────────────────────────────────────┐
│                   C++ 进阶编程学习路径                       │
└─────────────────────────────────────────────────────────────┘
                              │
        ┌─────────────────────┼─────────────────────┐
        ▼                     ▼                     ▼
   01_fundamentals      02_smart_pointers     03_memory_management
   基础数据结构            智能指针              内存管理
        │                     │                     │
   ┌────┴────┐           ┌────┴────┐           ┌────┴────┐
   │ 链表    │           │ unique │           │ 内存布局│
   │ 二叉树  │           │ shared │           │ 内存池  │
   │ 哈希表  │           │ weak   │           │ 分配器  │
   └─────────┘           └─────────┘           └─────────┘
        │                     │                     │
        └─────────────────────┼─────────────────────┘
                              │
                    ┌─────────┼─────────┐
                    ▼         ▼         ▼
         04_template_  05_string_  06_multi_
         metaprogramming  processing  threading
              │              │              │
        ┌─────┴─────┐  ┌────┴────┐  ┌─────┴─────┐
        │ 完美转发  │  │ 查找替换 │  │ 线程同步  │
        │ SFINAE   │  │ 正则表达式│  │ 线程池   │
        │ 类型萃取  │  │ string_view│ │ 异步编程  │
        └───────────┘  └─────────┘  └───────────┘
```

---

## 💡 学习建议

### 学习方法
1. **循序渐进**：严格按照模块顺序学习，不要跳跃
2. **动手实践**：每个文件都亲自编译运行，修改参数观察结果
3. **画图理解**：数据结构和算法最好画图辅助理解
4. **对比学习**：对比不同实现的性能差异
5. **应用实践**：将学到的技术应用到实际项目中

### 实践练习
1. 运行示例，观察输出结果
2. 使用调试器（gdb/lldb）跟踪执行流程
3. 编写单元测试验证功能
4. 尝试改进现有实现
5. 设计新场景应用所学技术

### 进阶方向
- **游戏开发**：对象池、Arena 分配器、线程池（模块 3、6）
- **服务器开发**：内存池、线程安全队列、高并发处理（模块 3、6）
- **系统编程**：内存布局、自定义分配器、底层优化（模块 3）
- **库开发**：模板编程、完美转发、泛型设计（模块 4）

---

## 📚 推荐资源

### 书籍
- 《C++ Primer》第 5 版 - 入门到精通
- 《Effective Modern C++》- Scott Meyers（现代 C++ 最佳实践）
- 《C++ Templates: The Complete Guide》- 模板编程权威指南
- 《C++ Concurrency in Action》- Anthony Williams（多线程编程）
- 《Game Engine Architecture》- Jason Gregory（游戏开发中的内存管理）

### 在线资源
- [cppreference.com](https://en.cppreference.com) - C++ 官方参考文档
- [Compiler Explorer](https://godbolt.org) - 在线编译器探索
- [Modern C++ Tutorial](https://github.com/changkun/modern-cpp-tutorial) - 现代C++教程

### 工具
- `gdb` / `lldb` - 调试器
- `valgrind` - 内存泄漏检测
- `perf` - 性能分析
- `clang-tidy` - 代码检查
- `AddressSanitizer` - 内存错误检测（`-fsanitize=address`）

---

## 🏗️ 技术要点总结

### 内存管理
- RAII（资源获取即初始化）
- 智能指针（`unique_ptr`, `shared_ptr`, `weak_ptr`）
- 内存对齐优化
- 自定义分配器（Arena、Stack、FreeList、Proxy）
- 对象池和内存池

### 并发编程
- 线程安全队列
- 条件变量与生产者-消费者
- 原子操作与内存序
- 线程池设计（基础版、增强版）
- std::future 和 std::promise（异步编程）

### 模板编程
- 类型推导规则
- 完美转发（`std::forward`）
- 可变参数模板
- SFINAE 与 `std::enable_if`
- C++20 Concepts

### 性能优化
- 移动语义
- 内存池与对象池
- 缓存友好设计
- string_view（零拷贝）
- 编译期计算

---

## 🎓 适用场景

本项目的学习成果适用于以下场景：

- 📚 **学习 C++**：系统学习 C++ 高级特性
- 💼 **求职面试**：准备 C++ 技术面试
- 🚀 **性能优化**：提升程序性能
- 🎮 **游戏开发**：学习游戏引擎中的内存管理和并发技术
- 🖥️ **服务器开发**：高并发、高性能服务器开发
- 🔧 **系统编程**：理解底层原理和优化技巧
- 📦 **库开发**：编写通用的、高效的 C++ 库

---

## 📝 许可证

本项目为学习项目，代码仅供学习参考。

---

## 🙏 总结

完成这 6 个模块的学习后，你将掌握：
- ✅ 基础数据结构的实现和应用
- ✅ 现代智能指针的使用
- ✅ 高级内存管理技术
- ✅ 模板编程和元编程
- ✅ 字符串处理技巧
- ✅ 多线程并发编程

这些技能将让你成为更出色的 C++ 开发者。祝你学习愉快！
