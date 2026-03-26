/**
 * 可变参数模板 (Variadic Templates) 示例
 *
 * 可变参数模板允许模板接受任意数量的参数
 *
 * 编译：g++ -std=c++17 variadic_templates.cpp -o variadic_templates.exe
 */

#include <iostream>
#include <tuple>
#include <utility>

using namespace std;

// 基本的可变参数模板
template<typename... Args>
void print(Args... args) {
    (cout << ... << args) << endl;  // C++17 折叠表达式
}

// 递归方式打印（C++11/14）
template<typename T>
void printRecursively(T&& arg) {
    cout << arg << endl;
}

template<typename T, typename... Args>
void printRecursively(T&& first, Args&&... args) {
    cout << first << " ";
    printRecursively(forward<Args>(args)...);
}

// 计算参数个数
template<typename... Args>
constexpr size_t countArgs(Args... args) {
    return sizeof...(args);
}

// 类型安全的 printf
template<typename T>
void printfSafe(T&& arg) {
    cout << arg;
}

template<typename T, typename... Args>
void printfSafe(T&& first, Args&&... args) {
    cout << first;
    printfSafe(forward<Args>(args)...);
}

// 求和（C++17 折叠表达式）
template<typename... Args>
auto sum(Args... args) {
    return (args + ... + 0);  // 右折叠
}

// 求乘积
template<typename... Args>
auto product(Args... args) {
    return (args * ... * 1);
}

// 检查所有参数是否都为 true
template<typename... Args>
bool allTrue(Args... args) {
    return (args && ...);
}

// 检查是否有参数为 true
template<typename... Args>
bool anyTrue(Args... args) {
    return (args || ...);
}

// 完美转发到构造函数
template<typename T, typename... Args>
T* create(Args&&... args) {
    return new T(forward<Args>(args)...);
}

// 检查类型是否在参数包中
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

// 创建 tuple
template<typename... Args>
auto makeTuple(Args&&... args) {
    return make_tuple(forward<Args>(args)...);
}

// 展开到 initializer_list
template<typename T, typename... Args>
vector<T> makeVector(Args&&... args) {
    return {forward<Args>(args)...};
}

// 函数指针包装器
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

// 示例函数
int add(int a, int b) {
    return a + b;
}

// 编译时检查
template<typename T>
struct is_integral_or_float {
    static constexpr bool value = is_integral_v<T> || is_floating_point_v<T>;
};

// 约束的可变参数模板
template<typename... Args>
enable_if_t<conjunction_v<is_integral_or_float<Args>...>, void>
sumNumbers(Args... args) {
    cout << "Sum: " << sum(args...) << endl;
}

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
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    cout << endl;

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
