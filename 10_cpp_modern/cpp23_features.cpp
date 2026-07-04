// ============================================================================
// 10 - C++ Modern: C++23 精选新特性
// ============================================================================
// 1. std::expected - 现代错误处理
// 2. std::print / std::println
// 3. if consteval
// 4. std::flat_map
// 5. expected 链式调用 (and_then)
// ============================================================================

#include <iostream>
#include <expected>
#include <string>
#include <print>
#include <vector>
#include <flat_map>

// 1. std::expected: 现代错误处理
std::expected<int, std::string> safe_divide(int a, int b) {
    if (b == 0) return std::unexpected(std::string("除数不能为零"));
    return a / b;
}

void test_expected() {
    std::println("=== 1. std::expected ===");
    
    auto r1 = safe_divide(10, 2);
    if (r1) std::println("10/2 = {}", *r1);
    else std::println("错误: {}", r1.error());
    
    auto r2 = safe_divide(10, 0);
    if (r2) std::println("10/0 = {}", *r2);
    else std::println("错误: {}", r2.error());
    
    // or_else: 错误恢复
    auto recovered = r2.or_else([](const std::string&) -> std::expected<int, std::string> {
        return -1;
    });
    std::println("恢复后默认值: {}", *recovered);
}

// 2. std::print / std::println
void test_print() {
    std::println("\n=== 2. std::print / std::println ===");
    
    std::print("Hello, {}!\n", "World");
    std::println("答案: {}, PI: {:.3f}", 42, 3.14159);
    std::println("十六进制: {:#x}, 二进制: {:#b}", 255, 15);
    std::println("|{:<10}|{:^10}|{:>10}|", "左对齐", "居中", "右对齐");
}

// 3. if consteval
constexpr double get_value() {
    if consteval {
        return 3.14159265358979323846;
    } else {
        return 3.14;
    }
}

void test_consteval() {
    std::println("\n=== 3. if consteval ===");
    
    constexpr double compile_val = get_value();
    double runtime_val = get_value();
    
    std::println("编译期值: {:.10f}", compile_val);
    std::println("运行期值: {:.2f}", runtime_val);
}

// 4. std::flat_map: 排序的扁平化映射
void test_flat_map() {
    std::println("\n=== 4. std::flat_map ===");
    
    std::flat_map<std::string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    scores["Diana"] = 98;
    
    std::println("成绩单:");
    for (const auto& [name, score] : scores) {
        std::println("  {}: {}", name, score);
    }
    
    if (auto it = scores.find("Charlie"); it != scores.end()) {
        std::println("Charlie 的成绩: {}", it->second);
    }
}

// 5. Expected 链式调用
std::expected<std::string, std::string> get_user(int id) {
    if (id <= 0) return std::unexpected("无效用户ID");
    return std::format("用户_{}", id);
}

std::expected<std::string, std::string> get_email(const std::string& user) {
    return std::format("{}@example.com", user);
}

void test_chaining() {
    std::println("\n=== 5. Expected 链式调用 ===");
    
    // and_then: 成功时继续处理
    auto email = get_user(42).and_then(get_email);
    if (email) std::println("邮箱: {}", *email);
    
    // 失败时链式中断
    auto bad = get_user(-1).and_then(get_email);
    if (!bad) std::println("错误: {}", bad.error());
    
    // transform: 转换值
    auto upper = get_user(99).transform([](const std::string& s) {
        std::string u = s;
        for (auto& c : u) c = std::toupper(c);
        return u;
    });
    if (upper) std::println("大写: {}", *upper);
}

int main() {
    std::println("=== C++23 精选新特性 ===");
    
    test_expected();
    test_print();
    test_consteval();
    test_flat_map();
    test_chaining();
    
    std::println("\n所有 C++23 特性演示完成!");
    return 0;
}
