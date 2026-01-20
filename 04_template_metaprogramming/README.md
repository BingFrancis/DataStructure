# 04 - 模板与元编程

## 📚 模块概述

模板是 C++ 最强大的特性之一，它不仅支持泛型编程，还能进行编译期计算和类型推导。本模块涵盖从模板基础到高级元编程技术，是写出通用、高效 C++ 代码的关键。

---

## 📁 文件列表

| 文件名 | 描述 | 学习时长 |
|--------|------|----------|
| `template_fundamentals.cpp` | 模板基础与函数模板 | 2-3 小时 |
| `function_templates_advanced.cpp` | 高级函数模板技术 | 2-3 小时 |
| `template_instantiation.cpp` | 模板实例化与 SFINAE | 2-3 小时 |

---

## 🎯 学习目标

- 理解模板的基本语法和使用
- 掌握模板参数推导规则
- 学会使用完美转发和可变参数模板
- 理解 SFINAE 原则
- 掌握类型萃取和编译期条件判断
- 了解 C++20 Concepts

---

## 📖 学习顺序建议

### 第一步：模板基础 (template_fundamentals.cpp)

#### 1. 函数模板
**基本语法：**
```cpp
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// 使用
int x = max(10, 20);      // T = int
double y = max(1.5, 2.5); // T = double
```

#### 2. 类模板
**基本语法：**
```cpp
template<typename T>
class Vector {
private:
    T* data;
    size_t size;
public:
    Vector() : data(nullptr), size(0) {}
    void push_back(const T& value);
};

// 使用
Vector<int> intVec;
Vector<std::string> strVec;
```

#### 3. 模板特化
**全特化：**
```cpp
template<>
class Vector<bool> {
    // 特化版本，优化 bool 存储
};
```

**偏特化（仅类模板支持）：**
```cpp
template<typename T, typename U = int>
class Pair {};

template<typename T>
class Pair<T, int> {  // 偏特化
    // 特化版本
};
```

**编译运行：**
```bash
g++ -std=c++17 template_fundamentals.cpp -o template_fundamentals && ./template_fundamentals
```

---

### 第二步：高级函数模板 (function_templates_advanced.cpp)

#### 1. 万能引用（Universal Reference）
**左值 vs 右值：**
```cpp
template<typename T>
void func(T&& arg) {
    // T&& 可能是左值引用，也可能是右值引用
    // 取决于传入的实参
}
```

#### 2. 完美转发（Perfect Forwarding）
**std::forward 保留值的类型：**
```cpp
template<typename T>
void wrapper(T&& arg) {
    target(std::forward<T>(arg));  // 完美转发
}
```

#### 3. 可变参数模板（Variadic Templates）
**基本语法：**
```cpp
template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args) << '\n';
}

// 使用
print(1, "hello", 3.14, 'a');  // 1hello3.14a
```

**递归展开：**
```cpp
template<typename T>
void print(T first) {
    std::cout << first << '\n';
}

template<typename T, typename... Args>
void print(T first, Args... args) {
    std::cout << first << ' ';
    print(args...);  // 递归调用
}
```

#### 4. 折叠表达式（C++17）
```cpp
// 左折叠
template<typename... Args>
auto sum_left(Args... args) {
    return (args + ...);  // (((a1 + a2) + a3) + ...)
}

// 右折叠
template<typename... Args>
auto sum_right(Args... args) {
    return (... + args);  // (a1 + (a2 + (a3 + ...)))
}
```

**编译运行：**
```bash
g++ -std=c++17 function_templates_advanced.cpp -o function_templates_advanced && ./function_templates_advanced
```

---

### 第三步：模板实例化与 SFINAE (template_instantiation.cpp)

#### 1. 模板实例化
**隐式实例化：**
```cpp
std::vector<int> vec;  // 编译器自动实例化 vector<int>
```

**显式实例化：**
```cpp
template class Vector<int>;  // 显式实例化
```

**显式特化声明：**
```cpp
extern template class Vector<int>;  // 声明，在其他文件中实例化
```

#### 2. SFINAE（替换失败并非错误）
**核心思想：**
- 在模板参数替换失败时，不会报错
- 编译器会尝试其他重载或特化版本

**示例：**
```cpp
template<typename T>
auto func(T t) -> decltype(t.size()) {
    // 只有 T 有 size() 成员时才匹配
    return t.size();
}

template<typename T>
void func(T t) {
    // 没有成员函数的类型
    std::cout << t << '\n';
}
```

#### 3. std::enable_if
**编译期条件判断：**
```cpp
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
process(T value) {
    std::cout << "Integral: " << value << '\n';
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
process(T value) {
    std::cout << "Floating: " << value << '\n';
}
```

#### 4. 类型萃取（Type Traits）
**常用类型萃取：**
```cpp
std::is_integral<T>::value        // 是否为整数类型
std::is_pointer<T>::value         // 是否为指针类型
std::is_const<T>::value           // 是否为 const
std::remove_reference<T>::type    // 移除引用
std::decay<T>::type               // 衰退（数组转指针，函数转指针）
std::common_type<T, U>::type      // 公共类型
```

#### 5. 编译期计算（Compile-time Computation）
**模板元编程经典示例：斐波那契数列：**
```cpp
template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template<>
struct Fibonacci<0> { static constexpr int value = 0; };

template<>
struct Fibonacci<1> { static constexpr int value = 1; };

// 使用
constexpr int fib10 = Fibonacci<10>::value;  // 编译期计算
```

#### 6. C++20 Concepts（概念）
**简化约束声明：**
```cpp
template<typename T>
concept Integral = std::is_integral_v<T>;

template<Integral T>
T add(T a, T b) {
    return a + b;
}

// 等价于旧写法：
template<typename T>
requires std::is_integral_v<T>
T add(T a, T b) {
    return a + b;
}
```

**编译运行：**
```bash
g++ -std=c++20 template_instantiation.cpp -o template_instantiation && ./template_instantiation
```

---

## 🔗 知识图谱

```
模板编程
├── 基础
│   ├── 函数模板
│   ├── 类模板
│   ├── 模板参数推导
│   └── 模板特化
├── 高级技术
│   ├── 万能引用
│   ├── 完美转发 (std::forward)
│   ├── 可变参数模板
│   └── 折叠表达式 (C++17)
├── 元编程
│   ├── SFINAE 原则
│   ├── 类型萃取 (Type Traits)
│   ├── std::enable_if
│   └── 编译期计算
└── C++20
    ├── Concepts (概念)
    ├── Requires 子句
    └── 简化模板约束
```

---

## 💡 最佳实践

### ✅ 推荐做法

1. **优先使用 `auto` 和 `decltype`**：
   ```cpp
   auto lambda = [](auto x) { return x * 2; };  // 泛型 lambda
   ```

2. **使用 constexpr 编译期计算**：
   ```cpp
   constexpr int fib = Fibonacci<10>::value;
   ```

3. **完美转发保留类型信息**：
   ```cpp
   template<typename T>
   void wrapper(T&& arg) {
       func(std::forward<T>(arg));
   }
   ```

4. **使用 type_traits 简化元编程**：
   ```cpp
   template<typename T>
   std::enable_if_t<std::is_integral_v<T>>
   process(T value) { /* ... */ }
   ```

### ❌ 避免的做法

1. **过度复杂的模板元编程**：
   ```cpp
   // 可读性差的嵌套模板
   template<typename T, template<typename> class U, typename V>
   class Complex;
   ```

2. **不必要的模板实例化**：
   ```cpp
   // 在头文件中实例化模板，导致编译时间增加
   ```

3. **忽略 SFINAE 导致的隐式错误**：
   ```cpp
   // 可能匹配到不期望的重载
   ```

---

## 📚 推荐资源

- **书籍**：《C++ Templates: The Complete Guide》- Nicolai M. Josuttis
- **书籍**：《Template Metaprogramming in C++》- Marius Bancila
- **文档**：[cppreference - Type Traits](https://en.cppreference.com/w/cpp/types)
- **视频**：C++ Template Metaprogramming - CppCon

---

## ✅ 学习检查清单

- [ ] 理解模板的基本语法和用法
- [ ] 掌握模板参数推导规则
- [ ] 理解左值引用和右值引用
- [ ] 掌握完美转发技术
- [ ] 掌握可变参数模板的使用
- [ ] 理解 SFINAE 原则和应用
- [ ] 掌握常用类型萃取
- [ ] 了解 C++20 Concepts
- [ ] 能够编写简单的编译期计算代码

---

## 🚀 下一步

完成本模块后，建议进入 **05_string_processing** 模块，学习字符串处理技术，然后再进入 **06_multithreading** 模块，学习多线程编程。
