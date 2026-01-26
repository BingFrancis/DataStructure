# 08 - Advanced C++（高级 C++ 特性）

本模块涵盖现代 C++ 的高级特性和最佳实践。

## 目录结构

```
08_advanced_cpp/
├── move_semantics.cpp       # 移动语义和右值引用
├── rvalue_references.cpp    # 右值引用详解
├── perfect_forwarding.cpp   # 完美转发
├── variadic_templates.cpp  # 可变参数模板
├── lambda_expressions.cpp   # Lambda 表达式进阶
├── coroutines.cpp           # C++20 协程
├── concepts.cpp             # C++20 概念约束
├── ranges.cpp               # C++20 范围库
├── modules.cpp              # C++20 模块系统
├── chrono.cpp               # 时间库
└── README.md                # 本文件
```

## 学习目标

- 理解移动语义和右值引用
- 掌握完美转发技术
- 学习可变参数模板
- 熟悉 C++20 新特性（协程、概念、范围、模块）
- 掌握现代 C++ 最佳实践

## 核心概念

### 1. 移动语义 (Move Semantics)

移动语义允许在不复制数据的情况下转移资源所有权，提高性能。

```cpp
class MyClass {
    unique_ptr<int[]> data;
    size_t size;

public:
    // 移动构造函数
    MyClass(MyClass&& other) noexcept
        : data(std::move(other.data)), size(other.size) {
        other.size = 0;
    }

    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            size = other.size;
            other.size = 0;
        }
        return *this;
    }
};
```

### 2. 右值引用 (Rvalue References)

```cpp
void foo(int& x);        // 只能绑定左值
void foo(const int& x);  // 可以绑定左值或右值
void foo(int&& x);       // 只能绑定右值

int a = 10;
foo(a);      // 调用 foo(int&)
foo(10);     // 调用 foo(int&&)
foo(a + 5);  // 调用 foo(int&&)
```

### 3. 完美转发 (Perfect Forwarding)

```cpp
template<typename T>
void wrapper(T&& arg) {
    foo(std::forward<T>(arg));  // 完美转发
}
```

### 4. C++20 新特性

**概念约束 (Concepts):**
```cpp
template<typename T>
concept Integral = std::is_integral_v<T>;

template<Integral T>
T add(T a, T b) {
    return a + b;
}
```

**范围 (Ranges):**
```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto result = v | std::views::filter([](int x) { return x % 2 == 0; })
                 | std::views::transform([](int x) { return x * 2; });
```

**协程 (Coroutines):**
```cpp
generator<int> sequence(int start, int end) {
    for (int i = start; i <= end; ++i) {
        co_yield i;
    }
}
```

## 编译要求

### C++17/20 编译
```bash
g++ -std=c++20 filename.cpp -o filename.exe
```

## 推荐学习顺序

1. 移动语义和右值引用
2. 完美转发
3. 可变参数模板
4. C++20 新特性（概念、范围、协程、模块）

## 相关模块

- `02_smart_pointers` - 智能指针（与移动语义密切相关）
- `03_memory_management` - 内存管理
- `04_template_metaprogramming` - 模板编程

## 参考资源

- 《Effective Modern C++》- Scott Meyers
- 《C++20 - The Complete Guide》- Nicolai M. Josuttis
- cppreference.com - C++ 标准库参考
