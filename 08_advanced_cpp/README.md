# 08_advanced_cpp — 现代 C++ 核心机制

> **⚠️ 关键模块：** 本模块是理解智能指针优化、多线程资源转移、模板完美转发的前提。
> 务必在进入阶段三（模板进阶）和阶段四（多线程/设计模式）之前完成。

本模块深入讲解 C++ 的值类别体系和资源管理机制。

## 📁 文件列表

| 文件 | 描述 | 难度 | 学习顺序 |
|------|------|------|----------|
| `value_categories.cpp` | **值类别**（lvalue/rvalue/xvalue/prvalue/引用折叠） | ⭐⭐⭐ | ① 基础 |
| `move_semantics.cpp` | **移动语义**（&&、std::move、移动构造/赋值） | ⭐⭐⭐⭐ | ② 核心 |
| `perfect_forwarding.cpp` | **完美转发**（std::forward、转发引用 T&&） | ⭐⭐⭐⭐ | ③ 进阶 |
| `rule_of_five.cpp` | **Rule of 5 实战**（MyString 完整演示拷贝/移动） | ⭐⭐⭐ | ④ 实战 |

## 🎯 学习目标

- **理解值类别体系**（lvalue/rvalue/xvalue/prvalue/glvalue）
- 掌握 std::move 的正确用法（move 不是移动，是转型）
- 学会完美转发和转发引用
- 能正确实现 Rule of 5（五大特殊成员函数）
- 理解移动后对象的状态

## 📖 核心概念

### 1. 值类别全景图

```
         expression
        /          \
    glvalue        rvalue
    /     \        /     \
lvalue    xvalue  prvalue
```

| 表达式 | 值类别 | 可绑定到 |
|--------|--------|----------|
| 变量名 `x` | lvalue | `T&`, `const T&` |
| 字面量 `42` | prvalue | `T&&`, `const T&` |
| `std::move(x)` | xvalue | `T&&`, `const T&` |
| 函数返回 `T` (非引用) | prvalue | `T&&`, `const T&` |

### 2. 移动语义

```cpp
class Buffer {
    int* data;
    size_t size;
public:
    // 移动构造函数
    Buffer(Buffer&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    // 移动赋值运算符
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
```

### 3. 完美转发

```cpp
// 转发引用 + 完美转发
template<typename T>
void wrapper(T&& arg) {                    // T&& 是转发引用
    process(std::forward<T>(arg));          // 保留原始值类别
}

// 实际应用：工厂函数
template<typename T, typename... Args>
unique_ptr<T> create(Args&&... args) {
    return make_unique<T>(std::forward<Args>(args)...);
}
```

### 4. Rule of 5

```cpp
class MyClass {
public:
    ~MyClass();                              // 1. 析构函数
    MyClass(const MyClass&);                 // 2. 拷贝构造
    MyClass& operator=(const MyClass&);      // 3. 拷贝赋值
    MyClass(MyClass&&) noexcept;             // 4. 移动构造
    MyClass& operator=(MyClass&&) noexcept;  // 5. 移动赋值
};
```

## ⚠️ 注意事项

### std::move 不会移动，只是转型

```cpp
string s = "hello";
string s2 = move(s);  // move(s) 只是将 s 转型为右值引用
// 真正的"移动"发生在 string 的移动构造函数中
```

### 移动后对象的状态

```cpp
vector<int> v1 = {1, 2, 3};
vector<int> v2 = move(v1);
// v1 处于「有效但未指定」状态
// 只能对 v1 进行：析构、赋值（不含访问元素的操作）
```

### 转发引用不是右值引用

```cpp
template<typename T>
void f(T&& arg) {       // T&& 是转发引用（不是右值引用！）
    // arg 是左值（有名字）
    g(std::forward<T>(arg));  // 完美转发
}
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 value_categories.cpp -o vc && ./vc
g++ -std=c++17 move_semantics.cpp -o move && ./move
g++ -std=c++17 perfect_forwarding.cpp -o forward && ./forward
g++ -std=c++17 rule_of_five.cpp -o rof && ./rof
```

## ✅ 学习检查清单

- [ ] 能区分 lvalue / rvalue / xvalue / prvalue
- [ ] 理解引用折叠规则（`T& &` → `T&`）
- [ ] 能正确实现移动构造函数和移动赋值运算符
- [ ] 理解 `std::move` 只是转型，不是移动
- [ ] 能正确使用 `std::forward` 进行完美转发
- [ ] 理解转发引用 `T&&` 和右值引用 `int&&` 的区别
- [ ] 能独立实现 Rule of 5

## 🔗 相关模块

- `02_smart_pointers` — `unique_ptr` 只移动不拷贝，依赖移动语义
- `04_template_metaprogramming` — 变参模板中的完美转发构造
- `06_multithreading` — 线程池中大量使用 `std::move`/`std::forward`

---

**移动语义 = 现代 C++ 性能提升的关键！** 🚀
