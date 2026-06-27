# 10_cpp_modern - 现代 C++ 新特性 (C++20/C++23)

本模块学习 C++20 和 C++23 的重要新特性，保持技术栈与时俱进。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `cpp20_features.cpp` | C++20 核心新特性 | ⭐⭐⭐⭐ |
| `cpp23_features.cpp` | C++23 精选新特性 | ⭐⭐⭐⭐ |

## 🎯 学习目标

- 掌握 C++20 的四大核心特性（Concepts、Ranges、Coroutines、Modules）
- 理解 C++23 的重要改进
- 学会在实际项目中应用现代特性

## 📖 C++20 核心特性

### 1. Concepts - 概念约束

```cpp
// 定义概念：要求类型支持 + 运算
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

// 使用概念约束模板参数
template<Addable T>
T add(T a, T b) { return a + b; }
```

### 2. Ranges - 范围库

```cpp
#include <ranges>
using namespace std::views;

vector<int> v = {1, 2, 3, 4, 5, 6};
// 管道语法：过滤 + 转换
auto result = v | filter([](int x) { return x % 2 == 0; })
                | transform([](int x) { return x * x; });
// 结果: {4, 16, 36}
```

### 3. std::span - 安全数组视图

```cpp
void process(std::span<int> data) {
    // 零拷贝，带边界检查的数组视图
    for (int& val : data) { val *= 2; }
}

int arr[] = {1, 2, 3, 4};
process(arr);  // 安全传递数组
```

### 4. Three-way comparison (太空船操作符)

```cpp
struct Point {
    int x, y;
    auto operator<=>(const Point&) const = default;
    // 自动生成所有比较运算符!
};
```

## 📖 C++23 精选特性

### 1. std::expected - 错误处理

```cpp
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) return std::unexpected("除以零");
    return a / b;
}
```

### 2. std::print (C++23)

```cpp
std::print("Hello {}! The answer is {}\n", "World", 42);
```

### 3. if consteval

```cpp
constexpr int compute() {
    if consteval {
        return compile_time_calc();  // 编译期执行
    } else {
        return run_time_calc();      // 运行期执行
    }
}
```

## 🛠️ 编译运行

```bash
# C++20 需要 GCC 10+ / Clang 10+ / MSVC 2019 16.10+
g++ -std=c++20 cpp20_features.cpp -o cpp20 && ./cpp20

# C++23 需要 GCC 13+ / Clang 17+ / MSVC 2022 17.4+
g++ -std=c++23 cpp23_features.cpp -o cpp23 && ./cpp23
```

## ✅ 学习检查清单

- [ ] 理解 Concepts 的概念约束
- [ ] 能使用 Ranges 进行管道式数据处理
- [ ] 掌握 std::span 的使用
- [ ] 了解 std::expected 错误处理
- [ ] 知道何时使用 coroutines
- [ ] 了解 std::format/std::print

## 🔗 相关模块

- `04_template_metaprogramming` - 模板基础
- `08_advanced_cpp` - C++11/14/17 特性

---

**现代 C++ = 更安全、更简洁、更高效！** 🚀
