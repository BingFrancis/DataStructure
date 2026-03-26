# C++ 数据结构与算法学习项目

> 从基础到实战的完整 C++ 学习路径

## 📚 项目简介

本项目系统性地涵盖了 C++ 开发所需的核心知识，从基础数据结构到高级特性，从算法到底层原理，从多线程到设计模式，帮助你全面提升 C++ 编程能力。

## 🎯 学习路径

建议按照模块顺序学习，每个模块的知识点循序渐进：

```
01_fundamentals (基础)
    ↓
02_smart_pointers (智能指针)
    ↓
03_memory_management (内存管理)
    ↓
04_template_metaprogramming (模板编程)
    ↓
05_string_processing (字符串处理)
    ↓
06_multithreading (多线程)
    ↓
07_algorithms (算法)
    ↓
08_advanced_cpp (高级 C++)
    ↓
09_design_patterns (设计模式)
```

## 📁 模块说明

### 1️⃣ 01_fundamentals - 基础数据结构

**核心内容：**
- 链表（linked_list.cpp）
- 二叉树（binary_tree.cpp）
- 哈希表（hash_table.cpp）
- 栈（stack.cpp）
- 队列（queue.cpp）
- 堆（heap.cpp）
- **继承机制**（inheritance.cpp）⭐

**学习目标：**
- 掌握基本数据结构的实现原理
- 理解面向对象编程核心概念（继承、多态、虚函数）
- 学会使用 STL 容器

**推荐学习时间：** 8-12 小时

---

### 2️⃣ 02_smart_pointers - 智能指针

**核心内容：**
- unique_ptr 独占所有权
- shared_ptr 共享所有权
- weak_ptr 解决循环引用
- 自定义删除器

**学习目标：**
- 理解 RAII 原则
- 避免内存泄漏
- 正确管理资源生命周期

**推荐学习时间：** 3-5 小时

---

### 3️⃣ 03_memory_management - 内存管理

**核心内容：**
- 内存布局（memory_layout.cpp）
- 对象池（object_pool_demo.cpp）
- 内存池（memory_pool.cpp）
- 智能分配器（smart_allocator.cpp）

**学习目标：**
- 理解 C++ 内存模型
- 掌握高性能内存管理技术
- 优化内存分配效率

**推荐学习时间：** 4-6 小时

---

### 4️⃣ 04_template_metaprogramming - 模板编程

**核心内容：**
- 函数模板（template_fundamentals.cpp）
- 高级模板特性（function_templates_advanced.cpp）
- 模板实例化（template_instantiation.cpp）

**学习目标：**
- 掌握泛型编程思想
- 理解模板元编程基础
- 编写类型安全的通用代码

**推荐学习时间：** 4-6 小时

---

### 5️⃣ 05_string_processing - 字符串处理

**核心内容：**
- std::string 基础操作
- 字符串分割、拼接、查找
- 正则表达式应用

**学习目标：**
- 熟练使用 std::string
- 掌握常见字符串处理技巧

**推荐学习时间：** 2-3 小时

---

### 6️⃣ 06_multithreading - 多线程

**核心内容：**
- 线程基础（threading_demo.cpp）
- 线程同步（thread_guard.cpp）
- 线程池（thread_pool.cpp）
- 线程安全队列（safe_queue.cpp）
- Future/Promise（future.cpp）

**学习目标：**
- 理解并发编程模型
- 掌握线程同步机制
- 避免数据竞争和死锁

**推荐学习时间：** 8-12 小时

---

### 7️⃣ 07_algorithms - 算法

**核心内容：**
- 排序算法（sorting/）
  - 冒泡排序（bubble_sort.cpp）
  - 快速排序（quick_sort.cpp）
  - 归并排序（merge_sort.cpp）
- 查找算法（searching/）
  - 二分查找（binary_search.cpp）
- 动态规划（dynamic_programming/）
  - 斐波那契数列（fibonacci.cpp）

**学习目标：**
- 掌握常用排序和查找算法
- 理解算法复杂度
- 学会动态规划思想

**推荐学习时间：** 6-8 小时

---

### 8️⃣ 08_advanced_cpp - 高级 C++

**核心内容：**
- 移动语义（move_semantics.cpp）
- 完美转发（perfect_forwarding.cpp）
- 可变参数模板（variadic_templates.cpp）

**学习目标：**
- 理解左值和右值
- 掌握现代 C++ 性能优化技巧
- 编写高效的代码

**推荐学习时间：** 4-6 小时

---

### 9️⃣ 09_design_patterns - 设计模式

**核心内容：**

**创建型模式：**
- 单例模式（singleton.cpp）
- 工厂方法模式（factory_method.cpp）
- 抽象工厂模式（abstract_factory.cpp）
- 建造者模式（builder.cpp）

**结构型模式：**
- 适配器模式（adapter.cpp）
- 装饰器模式（decorator.cpp）
- 代理模式（proxy.cpp）
- 外观模式（facade.cpp）

**行为型模式：**
- 观察者模式（observer.cpp）
- 策略模式（strategy.cpp）
- 命令模式（command.cpp）
- 状态模式（state.cpp）

**学习目标：**
- 理解设计模式的原理和应用场景
- 掌握 23 种设计模式的核心思想
- 提高代码的可维护性和扩展性

**推荐学习时间：** 12-16 小时

---

## 🛠️ 编译运行

### 编译单个文件

```bash
g++ -std=c++17 filename.cpp -o output.exe
./output.exe
```

### 编译整个模块

```bash
# 以基础模块为例
cd 01_fundamentals
g++ -std=c++17 linked_list.cpp -o linked_list && ./linked_list
```

### 推荐编译器

- **GCC**: `g++` (推荐)
- **Clang**: `clang++`
- **MSVC**: Visual Studio

**C++ 标准：** C++17 或更高

---

## 📊 学习统计

| 模块 | 文件数 | 推荐时长 | 难度 |
|------|--------|----------|------|
| 01_fundamentals | 7 | 8-12h | ⭐⭐ |
| 02_smart_pointers | 2 | 3-5h | ⭐⭐ |
| 03_memory_management | 5 | 4-6h | ⭐⭐⭐ |
| 04_template_metaprogramming | 3 | 4-6h | ⭐⭐⭐ |
| 05_string_processing | 1 | 2-3h | ⭐ |
| 06_multithreading | 12 | 8-12h | ⭐⭐⭐⭐ |
| 07_algorithms | 4 | 6-8h | ⭐⭐⭐ |
| 08_advanced_cpp | 3 | 4-6h | ⭐⭐⭐⭐ |
| 09_design_patterns | 12 | 12-16h | ⭐⭐⭐⭐ |

**总学习时间：** 约 51-74 小时

---

## ✅ 学习检查清单

完成本项目学习后，你应该能够：

- [ ] 熟练实现常用数据结构（链表、树、哈希表等）
- [ ] 正确使用智能指针管理资源
- [ ] 理解并应用内存管理技术
- [ ] 编写泛型和模板代码
- [ ] 熟练处理字符串操作
- [ ] 掌握多线程编程和线程同步
- [ ] 实现常用算法
- [ ] 理解移动语义和完美转发
- [ ] 应用设计模式解决实际问题

---

## 💡 学习建议

1. **循序渐进**：按顺序学习，不要跳过基础模块
2. **动手实践**：每个示例都要亲自编译运行
3. **理解原理**：不仅要会写代码，还要理解背后的原理
4. **代码重构**：尝试用不同方式实现相同功能
5. **举一反三**：学习一个知识点后，思考应用场景

---

## 📖 参考资料

- **C++ Primer** - C++ 入门经典
- **Effective C++** - C++ 最佳实践
- **Inside the C++ Object Model** - C++ 对象模型
- **Design Patterns** - 设计模式经典

---

## 📝 版本说明

- **C++ 标准**: C++17
- **编译器**: GCC 7.0+ / Clang 5.0+ / MSVC 2017+
- **平台**: Windows / Linux / macOS

---

## 🙏 总结

完成这 9 个模块的学习后，你将掌握：

- ✅ 基础数据结构的实现和应用
- ✅ 现代智能指针的使用
- ✅ 高级内存管理技术
- ✅ 模板编程和元编程
- ✅ 字符串处理技巧
- ✅ 多线程并发编程
- ✅ 常用算法的原理和实现
- ✅ 现代 C++ 高级特性
- ✅ 常用设计模式

这些技能将让你成为更出色的 C++ 工程师。祝你学习愉快！

---

**Happy Coding! 🚀**
