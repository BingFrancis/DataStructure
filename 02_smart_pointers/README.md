# 02 - 智能指针

## 📚 模块概述

智能指针是现代 C++ 的核心特性，用于自动管理动态内存，避免内存泄漏和悬空指针。掌握智能指针是写出安全、高效 C++ 代码的关键。

---

## 📁 文件列表

| 文件名 | 描述 | 学习时长 |
|--------|------|----------|
| `smart_pointers.cpp` | unique_ptr、shared_ptr、weak_ptr 的使用 | 2-3 小时 |
| `modern_cpp_project.cpp` | 现代 C++ 特性综合应用 | 1-2 小时 |

---

## 🎯 学习目标

- 理解内存管理的 RAII 机制
- 掌握三种智能指针的使用场景
- 学会使用 `make_unique` 和 `make_shared`
- 理解引用计数和循环引用问题
- 熟悉现代 C++ 的其他特性

---

## 📖 学习顺序建议

### 第一步：智能指针基础 (smart_pointers.cpp)

#### 1. `std::unique_ptr`
**特点：**
- 独占所有权，不可复制，只能移动
- 性能开销最小
- 适合单一所有权的场景

**使用示例：**
```cpp
#include <memory>

std::unique_ptr<int> ptr1 = std::make_unique<int>(42);
std::unique_ptr<int> ptr2 = std::move(ptr1);  // ptr1 现在为 nullptr
```

**适用场景：**
- 工厂函数返回对象
- PIMPL 模式
- 资源独占管理

---

#### 2. `std::shared_ptr`
**特点：**
- 共享所有权，可复制
- 使用引用计数管理生命周期
- 性能比 `unique_ptr` 稍高

**使用示例：**
```cpp
std::shared_ptr<int> ptr1 = std::make_shared<int>(42);
std::shared_ptr<int> ptr2 = ptr1;  // 引用计数 = 2
```

**适用场景：**
- 多个对象共享同一个资源
- 缓存管理
- 观察者模式

---

#### 3. `std::weak_ptr`
**特点：**
- 不增加引用计数
- 用于解决循环引用问题
- 需要通过 `lock()` 获取 `shared_ptr`

**使用示例：**
```cpp
std::weak_ptr<int> weak = ptr1;
if (auto locked = weak.lock()) {
    // 使用 locked
}
```

**适用场景：**
- 观察者模式
- 缓存
- 防止循环引用

---

### 第二步：现代 C++ 特性 (modern_cpp_project.cpp)

**关键特性：**
- `auto` 类型推导
- `nullptr` 替代 NULL
- `constexpr` 编译期常量
- Lambda 表达式
- 范围 for 循环
- 结构化绑定

**使用示例：**
```cpp
auto num = 42;                          // 类型推导
auto func = [](int x) { return x * 2; }; // Lambda
std::pair<int, std::string> p{1, "hello"};
auto [key, value] = p;                  // 结构化绑定
```

---

## 🔗 知识图谱

```
智能指针
├── unique_ptr (独占所有权)
│   ├── make_unique
│   └── 自定义删除器
├── shared_ptr (共享所有权)
│   ├── make_shared
│   └── 引用计数
└── weak_ptr (观察所有权)
    └── lock() 获取 shared_ptr

现代 C++ 特性
├── 类型推导 (auto, decltype)
├── 空指针 (nullptr)
├── Lambda 表达式
├── 范围 for 循环
├── 结构化绑定
└── constexpr (编译期常量)
```

---

## 💡 最佳实践

### ✅ 推荐做法

1. **优先使用智能指针**：
   ```cpp
   // 好
   auto ptr = std::make_unique<MyClass>();

   // 避免
   MyClass* ptr = new MyClass();
   ```

2. **使用 make 函数**：
   ```cpp
   // 好 - 一次内存分配
   auto ptr = std::make_shared<MyClass>();

   // 差 - 两次内存分配
   auto ptr = std::shared_ptr<MyClass>(new MyClass());
   ```

3. **传递所有权**：
   ```cpp
   // 按值传递 unique_ptr（移动语义）
   void takeOwnership(std::unique_ptr<MyClass> ptr);

   // 按值传递 shared_ptr（增加引用计数）
   void shareOwnership(std::shared_ptr<MyClass> ptr);
   ```

### ❌ 避免的做法

1. **循环引用**：
   ```cpp
   // 危险！导致内存泄漏
   struct Node {
       std::shared_ptr<Node> next;
       std::shared_ptr<Node> prev;  // 应该用 weak_ptr
   };
   ```

2. **混用裸指针和智能指针**：
   ```cpp
   // 危险！可能导致悬空指针
   MyClass* raw = ptr.get();
   delete raw;  // 错误！智能指针已经管理了内存
   ```

---

## 🔧 编译运行

```bash
# 编译智能指针示例
g++ -std=c++17 smart_pointers.cpp -o smart_pointers && ./smart_pointers

# 编译现代 C++ 特性示例
g++ -std=c++17 modern_cpp_project.cpp -o modern_cpp && ./modern_cpp
```

---

## 📚 推荐资源

- **文档**：[cppreference - Smart Pointers](https://en.cppreference.com/w/cpp/memory)
- **书籍**：《Effective Modern C++》- Scott Meyers
- **视频**：C++ Smart Pointers - Jason Turner (C++ Weekly)

---

## ✅ 学习检查清单

- [ ] 理解 RAII 原则
- [ ] 掌握 unique_ptr 的使用
- [ ] 掌握 shared_ptr 的使用和引用计数
- [ ] 掌握 weak_ptr 解决循环引用
- [ ] 熟练使用 make_unique 和 make_shared
- [ ] 了解现代 C++ 的常用特性
- [ ] 能够设计安全的内存管理策略

---

## 🚀 下一步

完成本模块后，建议进入 **03_memory_management** 模块，学习更高级的内存管理技术，包括内存池、对象池等。
