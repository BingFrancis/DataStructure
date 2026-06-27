// ============================================================================
// 10 - C++ Modern: C++20 核心新特性
// ============================================================================
// 1. Concepts - 概念约束
// 2. Ranges - 范围库  
// 3. std::span - 安全数组视图
// 4. Three-way comparison (<=>)
// 5. std::format - 类型安全格式化
// 6. constexpr 增强
// 7. Designated initializers
// ============================================================================

#include <iostream>
#include <vector>
#include <span>
#include <string>
#include <compare>
#include <concepts>
#include <algorithm>
#include <ranges>
#include <format>

// 1. Concepts
template<typename T>
concept Printable = requires(T t, std::ostream& os) {
    { os << t } -> std::convertible_to<std::ostream&>;
};

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Printable T>
void safe_print(const T& value) { std::cout << value << std::endl; }

template<Numeric T>
T safe_add(T a, T b) { return a + b; }

void test_concepts() {
    std::cout << "=== 1. Concepts ===" << std::endl;
    safe_print(42);
    safe_print(std::string("Hello Concepts!"));
    std::cout << "safe_add(3, 5) = " << safe_add(3, 5) << std::endl;
}

// 2. Ranges
void test_ranges() {
    std::cout << "\n=== 2. Ranges ===" << std::endl;
    using namespace std::views;
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto result = numbers 
                | filter([](int n) { return n % 2 == 0; })
                | transform([](int n) { return n * n; })
                | take(3);
    
    std::cout << "偶数平方(前3): ";
    for (int n : result) std::cout << n << " ";
    std::cout << std::endl;
}

// 3. std::span
void process_data(std::span<int> data) {
    for (auto& val : data) val *= 2;
}

void test_span() {
    std::cout << "\n=== 3. std::span ===" << std::endl;
    int arr[] = {1, 2, 3, 4, 5};
    process_data(arr);
    std::cout << "修改后: ";
    for (auto x : arr) std::cout << x << " ";
    std::cout << std::endl;
}

// 4. Three-way comparison
struct Point {
    int x, y;
    auto operator<=>(const Point&) const = default;
};

void test_spaceship() {
    std::cout << "\n=== 4. Three-way comparison (<=>) ===" << std::endl;
    Point p1{1, 2}, p2{1, 3};
    std::cout << "p1 < p2: " << (p1 < p2) << std::endl;
    std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
}

// 5. std::format
void test_format() {
    std::cout << "\n=== 5. std::format ===" << std::endl;
    std::cout << std::format("Hello, {}!", "World") << std::endl;
    std::cout << std::format("整数: {}, 浮点: {:.2f}", 42, 3.14159) << std::endl;
    std::cout << std::format("十六进制: {:#x}", 255) << std::endl;
}

// 6. constexpr 增强
constexpr int factorial(int n) {
    if (n < 0) return -1;
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

void test_constexpr() {
    std::cout << "\n=== 6. constexpr 增强 ===" << std::endl;
    constexpr int f5 = factorial(5);
    std::cout << "编译期 5! = " << f5 << std::endl;
    std::cout << "运行期 6! = " << factorial(6) << std::endl;
}

// 7. Designated Initializers
struct Config {
    std::string host = "localhost";
    int port = 8080;
    bool debug = false;
    int timeout = 30;
};

void test_designated() {
    std::cout << "\n=== 7. Designated Initializers ===" << std::endl;
    Config c1{.host = "example.com", .port = 443, .debug = true};
    Config c2{.host = "localhost", .timeout = 60};
    std::cout << std::format("c1: {}:{}, debug={}", c1.host, c1.port, c1.debug) << std::endl;
    std::cout << std::format("c2: {}:{}, timeout={}", c2.host, c2.port, c2.timeout) << std::endl;
}

int main() {
    std::cout << "=== C++20 核心新特性 ===" << std::endl;
    test_concepts();
    test_ranges();
    test_span();
    test_spaceship();
    test_format();
    test_constexpr();
    test_designated();
    std::cout << "\n所有 C++20 特性演示完成!" << std::endl;
    return 0;
}
