# C++ 数据结构与算法学习项目

> 从语法基础到工程实战的完整学习路径，覆盖 C++11/14/17/20/23

## 📚 项目简介

本项目系统性地涵盖 C++ 开发所需的核心知识体系。按**「语法 → 机制 → 抽象 → 工程 → 未来」**五阶段递进设计，每阶段建立在前一阶段之上。

## 🎯 学习路径

```
阶段一：语法与数据（C++ 基本功）
    01_fundamentals → 02_smart_pointers → 03_memory_management
         │
         │  学完数据结构、智能指针、内存管理后，
         │  需要理解底层的「值语义」机制
         ↓
阶段二：现代 C++ 核心机制（写对代码的前提）
    08_advanced_cpp
    (值类别 → 移动语义 → 完美转发 → Rule of 5)
         │
         │  有了移动语义基础，才能理解泛型容器的优化
         │  和线程池中的资源转移
         ↓
阶段三：泛型与算法（抽象能力）
    04_template_metaprogramming → 07_algorithms → 05_string_processing
         │
         │  模板 + 算法 = 写出高效、通用的代码
         │
         ↓
阶段四：并发与工程（实战能力）
    06_multithreading → 09_design_patterns
         │
         │  线程池、设计模式都依赖移动语义和模板
         │
         ↓
阶段五：紧跟标准（面向未来）
    10_cpp_modern
```

### 为什么这样排序？

| 关键依赖 | 说明 |
|----------|------|
| 多线程依赖移动语义 | 线程池、`future`、安全队列中大量使用 `std::move`/`std::forward` |
| 模板依赖值类别 | 转发引用 `T&&`、完美转发都需要理解 lvalue/rvalue |
| 算法依赖模板 | 泛型排序、比较器、迭代器都是模板的典型应用 |
| 设计模式依赖智能指针 | Observer、Proxy 等模式大量使用 `shared_ptr`/`weak_ptr` |

---

## 📁 模块说明

### 1️⃣ 01_fundamentals — 基础数据结构 ⭐⭐

| 文件 | 描述 |
|------|------|
| `linked_list.cpp` | 链表（单向、双向） |
| `binary_tree.cpp` | 二叉树遍历与操作 |
| `hash_table.cpp` | 哈希表实现 |
| `stack.cpp` | 栈（LIFO） |
| `queue.cpp` | 队列（FIFO） |
| `heap.cpp` | 堆（优先队列） |
| `inheritance.cpp` | 继承与多态（虚函数、抽象类、多重继承） |
| `inheritance_guide.md` | 继承学习指南 |

**推荐时间：** 8-12h ｜ **标准：** C++17

---

### 2️⃣ 02_smart_pointers — 智能指针 ⭐⭐

| 文件 | 描述 |
|------|------|
| `smart_pointers.cpp` | unique_ptr / shared_ptr / weak_ptr 详解 |
| `modern_cpp_project.cpp` | 现代项目中的智能指针应用 |

**推荐时间：** 3-5h ｜ **标准：** C++17

---

### 3️⃣ 03_memory_management — 内存管理 ⭐⭐⭐

| 文件 | 描述 |
|------|------|
| `memory_layout.cpp` | C++ 内存布局（栈、堆、全局区） |
| `memory_pool.cpp` | 内存池实现 |
| `object_pool_demo.cpp` | 对象池实现 |
| `smart_allocator.cpp` | 智能分配器（Arena、栈分配器） |
| `object.h` | 对象池模板头文件 |

**推荐时间：** 4-6h ｜ **标准：** C++17

---

### 4️⃣ 08_advanced_cpp — 现代 C++ 核心机制 ⭐⭐⭐⭐

> **⚠️ 关键模块：** 本模块是理解后续所有内容的基石，务必在学多线程和模板进阶之前完成。

| 文件 | 描述 | 学习顺序 |
|------|------|----------|
| `value_categories.cpp` | **值类别**（lvalue/rvalue/xvalue/prvalue） | ① 先学 |
| `move_semantics.cpp` | **移动语义**（&&、std::move） | ② 再学 |
| `perfect_forwarding.cpp` | **完美转发**（std::forward、转发引用） | ③ 然后 |
| `rule_of_five.cpp` | **Rule of 5 实战**（MyString 完整演示） | ④ 最后 |

**推荐时间：** 6-8h ｜ **标准：** C++17

---

### 5️⃣ 04_template_metaprogramming — 模板编程 ⭐⭐⭐⭐

| 文件 | 描述 |
|------|------|
| `template_fundamentals.cpp` | 函数模板、类模板、全特化/偏特化、CTAD |
| `function_templates_advanced.cpp` | SFINAE、类型推导、类型萃取 |
| `template_instantiation.cpp` | 隐式/显式实例化、成员函数模板 |
| `variadic_templates.cpp` | 可变参数模板、折叠表达式、完美转发构造 |

**推荐时间：** 6-8h ｜ **标准：** C++17

---

### 6️⃣ 07_algorithms — 算法 ⭐⭐⭐

| 文件 | 描述 |
|------|------|
| `sorting/bubble_sort.cpp` | 冒泡排序 |
| `sorting/quick_sort.cpp` | 快速排序 |
| `sorting/merge_sort.cpp` | 归并排序 |
| `searching/binary_search.cpp` | 二分查找 |
| `dynamic_programming/fibonacci.cpp` | 动态规划入门 |

**推荐时间：** 6-8h ｜ **标准：** C++17

---

### 7️⃣ 05_string_processing — 字符串处理 ⭐⭐

| 文件 | 描述 |
|------|------|
| `string_operations.cpp` | std::string 实战（查找/替换/分割/正则/string_view） |

**推荐时间：** 2-3h ｜ **标准：** C++17

---

### 8️⃣ 06_multithreading — 多线程并发 ⭐⭐⭐⭐

> **依赖：** 需要先学完 `08_advanced_cpp`（移动语义/完美转发），否则看不懂线程池中的 `std::move` 和 `std::forward`。

| 文件 | 描述 | 学习顺序 |
|------|------|----------|
| `threading_demo.cpp` | 线程基础（创建、join/detach、mutex） | ① |
| `thread_guard.cpp` | RAII 线程守卫 | ② |
| `multithreading.cpp` | 线程管理综合示例 | ③ |
| `thread_test.cpp` | 生产者-消费者模型 | ④ |
| `safe_queue.cpp` | 线程安全队列 | ⑤ |
| `my_safe_queue.cpp` | 安全队列 + 线程池 | ⑥ |
| `task_queue.cpp` | 任务队列 | ⑦ |
| `future.cpp` | Future/Promise 异步编程 | ⑧ |
| `smart_thread_pool.cpp` | 智能线程池（RAII 管理） | ⑨ |
| `enhanced_thread_pool.cpp` | 增强线程池 | ⑩ |
| `smart_ptr_threading.cpp` | 智能指针与多线程 | ⑪ |
| `smart_ptr_thread_pool_demo.cpp` | 线程池完整演示 | ⑫ |

**推荐时间：** 8-12h ｜ **标准：** C++17（需 `-pthread`）

---

### 9️⃣ 09_design_patterns — 设计模式 ⭐⭐⭐⭐

| 分类 | 模式 |
|------|------|
| **创建型** | singleton / factory_method / abstract_factory / builder |
| **结构型** | adapter / decorator / proxy / facade |
| **行为型** | observer / strategy / command / state |

**推荐时间：** 12-16h ｜ **标准：** C++17

---

### 🔟 10_cpp_modern — 现代 C++ 新特性 ⭐⭐⭐⭐

| 文件 | 描述 |
|------|------|
| `cpp20_features.cpp` | Concepts、Ranges、span、`<=>`、format |
| `cpp23_features.cpp` | expected、print/println、consteval、flat_map |

**推荐时间：** 4-6h ｜ **标准：** C++20 / C++23

---

## 📊 学习统计

| 阶段 | 模块 | 难度 | 推荐时长 |
|------|------|------|----------|
| 语法与数据 | 01_fundamentals | ⭐⭐ | 8-12h |
| | 02_smart_pointers | ⭐⭐ | 3-5h |
| | 03_memory_management | ⭐⭐⭐ | 4-6h |
| 核心机制 | 08_advanced_cpp | ⭐⭐⭐⭐ | 6-8h |
| 泛型与算法 | 04_template_metaprogramming | ⭐⭐⭐⭐ | 6-8h |
| | 07_algorithms | ⭐⭐⭐ | 6-8h |
| | 05_string_processing | ⭐⭐ | 2-3h |
| 并发与工程 | 06_multithreading | ⭐⭐⭐⭐ | 8-12h |
| | 09_design_patterns | ⭐⭐⭐⭐ | 12-16h |
| 紧跟标准 | 10_cpp_modern | ⭐⭐⭐⭐ | 4-6h |

**总学习时间：** 约 59-84 小时

---

## 🛠️ 编译运行

```bash
# 单文件编译
g++ -std=c++17 filename.cpp -o output && ./output

# 多线程需要链接 pthread
g++ -std=c++17 threading.cpp -o app -pthread && ./app

# C++20/23 需要更新编译器
g++ -std=c++20 cpp20_features.cpp -o app && ./app
g++ -std=c++23 cpp23_features.cpp -o app && ./app
```

### 编译器要求

| 标准 | GCC | Clang | MSVC |
|------|-----|-------|------|
| C++17 | 7+ | 5+ | 2017 15.7+ |
| C++20 | 10+ | 10+ | 2019 16.10+ |
| C++23 | 13+ | 17+ | 2022 17.4+ |

---

## ✅ 学习检查清单

- [ ] 熟练实现常用数据结构（链表、树、哈希表、堆）
- [ ] 正确使用三种智能指针管理资源
- [ ] 理解内存布局和内存池/对象池技术
- [ ] **理解值类别（lvalue/rvalue/xvalue）** ← 新增
- [ ] **掌握移动语义和完美转发** ← 前置
- [ ] 编写泛型模板代码（含变参模板）
- [ ] 实现常用排序和查找算法
- [ ] 熟练处理字符串操作（含正则）
- [ ] 掌握多线程编程（线程池、Future/Promise）
- [ ] 应用 12 种设计模式
- [ ] 掌握 C++20/23 核心新特性

---

## 💡 学习建议

1. **严格按阶段顺序**：不要跳阶段，每一阶段都是下一阶段的基础
2. **先理解再使用**：`std::move` 不是"移动"，是"允许移动的转型"
3. **动手实践**：每个示例都要亲自编译运行，修改参数观察变化
4. **建立知识网络**：学完一个知识点后，思考它和已学内容的关联
5. **画图辅助**：内存布局、值类别关系、继承链 — 画出来比看代码更清晰

---

## 📖 参考资料

| 书籍 | 适用阶段 |
|------|----------|
| **C++ Primer** | 阶段一 |
| **Effective Modern C++** | 阶段二、三 |
| **C++ Concurrency in Action** | 阶段四 |
| **Design Patterns (GoF)** | 阶段四 |
| **cppreference.com** | 全程 |

---

**Happy Coding! 🚀**
