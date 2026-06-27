// ============================================================================
// 04 - Template Metaprogramming: 可变参数模板 (Variadic Templates)
// ============================================================================
// 演示 C++11+ 可变参数模板：参数包展开、折叠表达式、完美转发构造、
// 编译时类型检查等高级技巧。
// ============================================================================

#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
#include <string>
#include <type_traits>

using namespace std;

// ============================================================================
// 1. 基本可变参数模板 + 折叠表达式 (C++17)
// ============================================================================
template<typename... Args>
void print(Args... args) {
    (cout << ... << args) << endl;  // 二元左折叠
}

// ============================================================================
// 2. 递归方式打印（C++11/14 风格）
// ============================================================================
template<typename T>
void printRecursively(T&& arg) {
    cout << arg << endl;
}

template<typename T, typename... Args>
void printRecursively(T&& first, Args&&... args) {
    cout << first << " ";
    printRecursively(forward<Args>(args)...);
}

// ============================================================================
// 3. sizeof... 获取参数个数
// ============================================================================
template<typename... Args>
constexpr size_t countArgs(Args... args) {
    return sizeof...(args);
}

// ============================================================================
// 4. 类型安全的 printf
// ============================================================================
template<typename T>
void printfSafe(T&& arg) {
    cout << arg;
}

template<typename T, typename... Args>
void printfSafe(T&& first, Args&&... args) {
    cout << first;
    printfSafe(forward<Args>(args)...);
}

// ============================================================================
// 5. 折叠表达式：数学运算
// ============================================================================
template<typename... Args>
auto sum(Args... args) {
    return (args + ... + 0);  // 右折叠
}

template<typename... Args>
auto product(Args... args) {
    return (args * ... * 1);
}

// ============================================================================
// 6. 折叠表达式：逻辑运算
// ============================================================================
template<typename... Args>
bool allTrue(Args... args) {
    return (args && ...);
}

template<typename... Args>
bool anyTrue(Args... args) {
    return (args || ...);
}

// ============================================================================
// 7. 完美转发构造工厂
// ============================================================================
template<typename T, typename... Args>
T* create(Args&&... args) {
    return new T(forward<Args>(args)...);
}

// ============================================================================
// 8. 编译时类型检查：Contains
// ============================================================================
template<typename T, typename... Args>
struct Contains;

template<typename T>
struct Contains<T> {
    static constexpr bool value = false;
};

template<typename T, typename First, typename... Rest>
struct Contains<T, First, Rest...> {
    static constexpr bool value = is_same_v<T, First> || Contains<T, Rest...>::value;
};

// ============================================================================
// 9. makeTuple / makeVector 工厂函数
// ============================================================================
template<typename... Args>
auto makeTuple(Args&&... args) {
    return make_tuple(forward<Args>(args)...);
}

template<typename T, typename... Args>
vector<T> makeVector(Args&&... args) {
    return {forward<Args>(args)...};
}

// ============================================================================
// 10. 函数指针包装器
// ============================================================================
template<typename R, typename... Args>
class FunctionPtr {
private:
    R (*func)(Args...);
public:
    explicit FunctionPtr(R (*f)(Args...)) : func(f) {}
    R operator()(Args... args) const {
        return func(forward<Args>(args)...);
    }
};

int add(int a, int b) { return a + b; }

// ============================================================================
// 11. 约束的可变参数模板 (SFINAE)
// ============================================================================
template<typename T>
struct is_integral_or_float {
    static constexpr bool value = is_integral_v<T> || is_floating_point_v<T>;
};

template<typename... Args>
enable_if_t<conjunction_v<is_integral_or_float<Args>...>, void>
sumNumbers(Args... args) {
    cout << "Sum: " << sum(args...) << endl;
}

// ============================================================================
int main() {
    cout << "=== 1. 基本可变参数模板 ===" << endl;
    print(1, 2.5, "Hello", 'A');
    print();  // 零参数
    cout << endl;

    cout << "=== 2. 递归打印 ===" << endl;
    printRecursively(1, 2.5, "Hello", 'A');
    cout << endl;

    cout << "=== 3. 参数个数 ===" << endl;
    cout << "countArgs(1, 2, 3): " << countArgs(1, 2, 3) << endl;
    cout << "countArgs(): " << countArgs() << endl;
    cout << endl;

    cout << "=== 4. 类型安全的 printf ===" << endl;
    printfSafe("Hello, ", "World", "! ", 123, "\n");
    cout << endl;

    cout << "=== 5. 求和 ===" << endl;
    cout << "sum(1, 2, 3, 4): " << sum(1, 2, 3, 4) << endl;
    cout << "sum(1, 2, 3.5): " << sum(1, 2, 3.5) << endl;
    cout << endl;

    cout << "=== 6. 求乘积 ===" << endl;
    cout << "product(2, 3, 4): " << product(2, 3, 4) << endl;
    cout << endl;

    cout << "=== 7. 逻辑运算 ===" << endl;
    cout << "allTrue(true, true, true): " << boolalpha << allTrue(true, true, true) << endl;
    cout << "allTrue(true, false, true): " << allTrue(true, false, true) << endl;
    cout << "anyTrue(false, false, true): " << anyTrue(false, false, true) << endl;
    cout << "anyTrue(false, false, false): " << anyTrue(false, false, false) << endl;
    cout << endl;

    cout << "=== 8. 完美转发构造 ===" << endl;
    auto ptr = create<string>(5, 'A');
    cout << "Created string: " << *ptr << endl;
    delete ptr;
    cout << endl;

    cout << "=== 9. 类型检查 ===" << endl;
    cout << "Contains<int, int, double, char>: " << Contains<int, int, double, char>::value << endl;
    cout << "Contains<int, double, char, string>: " << Contains<int, double, char, string>::value << endl;
    cout << endl;

    cout << "=== 10. 创建 tuple ===" << endl;
    auto t = makeTuple(1, 2.5, "Hello");
    cout << "get<0>(t): " << get<0>(t) << endl;
    cout << "get<1>(t): " << get<1>(t) << endl;
    cout << "get<2>(t): " << get<2>(t) << endl;
    cout << endl;

    cout << "=== 11. 创建 vector ===" << endl;
    auto v = makeVector<int>(1, 2, 3, 4, 5);
    cout << "vector: ";
    for (int x : v) { cout << x << " "; }
    cout << endl << endl;

    cout << "=== 12. 函数指针包装 ===" << endl;
    FunctionPtr<int, int, int> funcPtr(add);
    cout << "funcPtr(3, 5): " << funcPtr(3, 5) << endl;
    cout << endl;

    cout << "=== 13. 约束的可变参数模板 ===" << endl;
    sumNumbers(1, 2, 3);
    // sumNumbers(1, "Hello");  // 编译错误：不是整数或浮点数
    cout << endl;

    return 0;
}
