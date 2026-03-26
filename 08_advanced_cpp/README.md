# 08_advanced_cpp - 高级 C++ 特性

本模块学习现代 C++ 的高级特性，包括移动语义、完美转发和可变参数模板。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `move_semantics.cpp` | 移动语义（&&、std::move） | ⭐⭐⭐⭐ |
| `perfect_forwarding.cpp` | 完美转发（std::forward） | ⭐⭐⭐⭐ |
| `variadic_templates.cpp` | 可变参数模板 | ⭐⭐⭐⭐⭐ |

## 🎯 学习目标

- 理解左值、右值和移动语义
- 掌握 std::move 的正确用法
- 学会完美转发
- 理解可变参数模板

## 📖 核心概念

### 1. 左值与右值

```cpp
int a = 42;       // a 是左值，42 是右值
int& r = a;       // 左值引用
int&& rr = 42;    // 右值引用（绑定到右值）
int&& rr2 = a;    // ❌ 错误：右值引用不能绑定左值

string s = "hello";
string s2 = s;            // 拷贝构造
string s3 = move(s);      // 移动构造（s 变为空）
```

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
// 万能引用 + 完美转发
template<typename T>
void wrapper(T&& arg) {
    process(forward<T>(arg));  // 保持参数的值类别
}

// 实际应用：工厂函数
template<typename T, typename... Args>
unique_ptr<T> create(Args&&... args) {
    return make_unique<T>(forward<Args>(args)...);
}
```

### 4. 可变参数模板

```cpp
// 递归展开
void print() {}  // 终止条件

template<typename T, typename... Args>
void print(T first, Args... rest) {
    cout << first << " ";
    print(rest...);
}

print(1, "hello", 3.14);  // 1 hello 3.14

// 折叠表达式 (C++17)
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // 一元右折叠
}
```

## ⚠️ 注意事项

### std::move 不会移动，只是转换

```cpp
string s = "hello";
string s2 = move(s);  // s2 调用移动构造函数
// move(s) 只是将 s 转换为右值引用
// s 本身并没有被"移动"，只是它的资源被转移了
```

### 移动后对象的状态

```cpp
vector<int> v1 = {1, 2, 3};
vector<int> v2 = move(v1);
// v1 现在处于有效但未指定的状态
// 只能对 v1 进行析构或赋值
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 move_semantics.cpp -o move && ./move
g++ -std=c++17 perfect_forwarding.cpp -o forward && ./forward
g++ -std=c++17 variadic_templates.cpp -o variadic && ./variadic
```

## ✅ 学习检查清单

- [ ] 理解左值和右值的区别
- [ ] 能正确实现移动构造函数
- [ ] 能正确实现移动赋值运算符
- [ ] 理解 std::move 的原理
- [ ] 能正确使用完美转发
- [ ] 能编写可变参数模板
- [ ] 理解折叠表达式

## 🔗 相关模块

- `02_smart_pointers` - 智能指针的移动语义
- `04_template_metaprogramming` - 模板基础
- `06_multithreading` - 完美转发在多线程中的应用

---

**移动语义 = 现代 C++ 性能提升的关键！** 🚀
