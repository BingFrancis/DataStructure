# 03_memory_management - 内存管理

本模块学习 C++ 内存模型和高性能内存管理技术。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `memory_layout.cpp` | C++ 内存布局（栈、堆、全局区） | ⭐⭐⭐ |
| `memory_pool.cpp` | 内存池实现 | ⭐⭐⭐⭐ |
| `smart_allocator.cpp` | 智能分配器 | ⭐⭐⭐⭐ |
| `object_pool_demo.cpp` | 对象池实现 | ⭐⭐⭐⭐ |
| `object.h` | 对象定义 | ⭐⭐ |

## 🎯 学习目标

- 理解 C++ 内存模型和布局
- 掌握内存池和对象池的实现
- 学会自定义内存分配器
- 优化程序内存使用效率

## 📖 核心概念

### C++ 内存布局

```
┌─────────────────────┐  高地址
│     栈 (Stack)      │  ↓ 向下增长
│   局部变量、函数调用  │
├─────────────────────┤
│                     │
│   空闲区域 (Free)    │
│                     │
├─────────────────────┤
│     堆 (Heap)       │  ↑ 向上增长
│   new/malloc 分配    │
├─────────────────────┤
│   BSS 段 (未初始化)  │
│   全局变量(零初始化)  │
├─────────────────────┤
│   Data 段 (已初始化) │
│   全局/静态变量       │
├─────────────────────┤
│   Text 段 (代码)     │
│   程序指令           │
└─────────────────────┘  低地址
```

### 三种内存管理技术对比

| 技术 | 原理 | 优点 | 缺点 | 适用场景 |
|------|------|------|------|----------|
| 内存池 | 预分配大块内存 | 减少分配开销 | 内存碎片 | 频繁分配/释放 |
| 对象池 | 预创建对象复用 | 避免构造/析构 | 内存占用 | 固定大小对象 |
| 分配器 | 自定义分配策略 | 灵活可控 | 实现复杂 | 特殊需求 |

## ⚠️ 常见内存问题

### 1. 内存泄漏

```cpp
// ❌ 泄漏
void leak() {
    int* p = new int(42);
    // 忘记 delete p
}

// ✅ 解决：使用智能指针
void no_leak() {
    auto p = make_unique<int>(42);
}
```

### 2. 悬挂指针

```cpp
// ❌ 悬挂指针
int* p = new int(42);
delete p;
*p = 10;  // 未定义行为

// ✅ 解决
int* p = new int(42);
delete p;
p = nullptr;  // 置空
```

### 3. 重复释放

```cpp
// ❌ 重复释放
int* p = new int(42);
delete p;
delete p;  // 未定义行为

// ✅ 解决：使用 unique_ptr（自动管理）
auto p = make_unique<int>(42);
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 memory_layout.cpp -o memory_layout && ./memory_layout
g++ -std=c++17 memory_pool.cpp -o memory_pool && ./memory_pool
g++ -std=c++17 smart_allocator.cpp -o smart_allocator && ./smart_allocator
g++ -std=c++17 object_pool_demo.cpp -o object_pool_demo && ./object_pool_demo
```

## ✅ 学习检查清单

- [ ] 理解 C++ 程序的内存布局
- [ ] 知道栈和堆的区别
- [ ] 能实现简单的内存池
- [ ] 能实现对象池
- [ ] 理解自定义分配器的原理
- [ ] 能识别和避免内存泄漏

## 🔗 相关模块

- `02_smart_pointers` - 智能指针（自动内存管理）
- `06_multithreading` - 多线程中的内存管理

---

**理解内存管理 = 写出高性能代码的基础！** 🚀
