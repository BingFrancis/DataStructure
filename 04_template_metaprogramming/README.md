# 04_template_metaprogramming - 模板编程

本模块学习 C++ 模板编程，包括函数模板、类模板和模板元编程基础。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `template_fundamentals.cpp` | 模板基础（函数模板、类模板） | ⭐⭐⭐ |
| `function_templates_advanced.cpp` | 高级函数模板（SFINAE、完美转发） | ⭐⭐⭐⭐ |
| `template_instantiation.cpp` | 模板实例化与特化 | ⭐⭐⭐⭐ |

## 🎯 学习目标

- 掌握函数模板和类模板
- 理解模板参数推导
- 学会模板特化和偏特化
- 了解 SFINAE 和 concepts

## 📖 核心概念

### 1. 函数模板

```cpp
// 基础函数模板
template<typename T>
T add(T a, T b) {
    return a + b;
}

// 多类型参数
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// 模板参数推导 (C++17)
template<typename T>
void print(T value) {
    cout << value << endl;
}

print(42);        // T = int
print(3.14);      // T = double
print("hello");   // T = const char*
```

### 2. 类模板

```cpp
template<typename T, int Size>
class Stack {
private:
    T data[Size];
    int top = -1;
public:
    void push(const T& val) { data[++top] = val; }
    T pop() { return data[top--]; }
};

Stack<int, 100> intStack;
Stack<string, 50> strStack;
```

### 3. 模板特化

```cpp
// 通用版本
template<typename T>
struct TypeInfo {
    static string name() { return "unknown"; }
};

// 特化版本
template<>
struct TypeInfo<int> {
    static string name() { return "int"; }
};

template<>
struct TypeInfo<double> {
    static string name() { return "double"; }
};
```

### 4. SFINAE（替换失败不是错误）

```cpp
// enable_if 限制模板参数
template<typename T>
typename enable_if<is_integral<T>::value, T>::type
process(T val) {
    return val * 2;
}

template<typename T>
typename enable_if<is_floating_point<T>::value, T>::type
process(T val) {
    return val + 0.5;
}
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 template_fundamentals.cpp -o template_fundamentals && ./template_fundamentals
g++ -std=c++17 function_templates_advanced.cpp -o function_templates_advanced && ./function_templates_advanced
g++ -std=c++17 template_instantiation.cpp -o template_instantiation && ./template_instantiation
```

## ✅ 学习检查清单

- [ ] 能编写函数模板和类模板
- [ ] 理解模板参数推导
- [ ] 掌握模板特化和偏特化
- [ ] 了解 SFINAE 的原理
- [ ] 能使用 type_traits
- [ ] 了解 C++20 Concepts

## 🔗 相关模块

- `05_string_processing` - 模板在字符串处理中的应用
- `08_advanced_cpp` - 可变参数模板

---

**模板 = C++ 泛型编程的核心！** 🚀
