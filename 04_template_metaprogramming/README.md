# 04_template_metaprogramming - 模板编程

本模块学习 C++ 模板编程，从函数模板到可变参数模板，覆盖泛型编程的核心技术。

## 📁 文件列表

| 文件 | 描述 | 难度 | 学习顺序 |
|------|------|------|----------|
| `template_fundamentals.cpp` | 函数模板、类模板、全特化/偏特化、CTAD | ⭐⭐⭐ | ① |
| `template_instantiation.cpp` | 隐式/显式实例化、成员函数模板 | ⭐⭐⭐⭐ | ② |
| `function_templates_advanced.cpp` | SFINAE、类型推导、类型萃取 | ⭐⭐⭐⭐ | ③ |
| `variadic_templates.cpp` | 可变参数模板、折叠表达式、完美转发构造 | ⭐⭐⭐⭐⭐ | ④ |

## 🎯 学习目标

- 掌握函数模板和类模板
- 理解模板参数推导与 CTAD (C++17)
- 学会模板特化和偏特化
- 了解 SFINAE 和 type_traits
- **掌握可变参数模板和折叠表达式**

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

// 全特化版本
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

### 5. 可变参数模板 + 折叠表达式 (C++17)

```cpp
// 折叠表达式：求和
template<typename... Args>
auto sum(Args... args) {
    return (args + ... + 0);  // 右折叠
}

// 折叠表达式：打印
template<typename... Args>
void print(Args... args) {
    (cout << ... << args) << endl;  // 二元左折叠
}
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 template_fundamentals.cpp -o tf && ./tf
g++ -std=c++17 template_instantiation.cpp -o ti && ./ti
g++ -std=c++17 function_templates_advanced.cpp -o fta && ./fta
g++ -std=c++17 variadic_templates.cpp -o vt && ./vt
```

## ✅ 学习检查清单

- [ ] 能编写函数模板和类模板
- [ ] 理解模板参数推导
- [ ] 掌握模板特化和偏特化
- [ ] 了解 SFINAE 的原理
- [ ] 能使用 type_traits
- [ ] **能编写可变参数模板**
- [ ] **理解折叠表达式**

## 🔗 相关模块

- `08_advanced_cpp` — 值类别和完美转发是变参模板的前提
- `07_algorithms` — 泛型算法大量使用模板
- `06_multithreading` — 线程池 enqueue 使用变参模板 + 完美转发

---

**模板 = C++ 泛型编程的核心！** 🚀
