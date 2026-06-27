// ============================================================================
// 04 - Template Metaprogramming: 模板基础 (完整版)
// ============================================================================
// 1. 函数模板
// 2. 类模板
// 3. 非类型模板参数
// 4. 模板特化 (全特化/偏特化)
// 5. CTAD (C++17)
// 6. 成员函数模板
// ============================================================================

#include <iostream>
#include <string>
#include <typeinfo>

// ============================================================================
// 1. 函数模板
// ============================================================================

template<typename T>
T add(T a, T b) {
    return a + b;
}

template<typename T, typename U>
auto multiply(T a, U b) {
    return a * b;
}

template<typename T>
void print_type(const T& val) {
    std::cout << "值: " << val << " | 类型: " << typeid(T).name() << std::endl;
}

// ============================================================================
// 2. 类模板
// ============================================================================

// 2.1 Pair 类模板
template<typename T1, typename T2>
class Pair {
    T1 first;
    T2 second;
public:
    Pair(const T1& a, const T2& b) : first(a), second(b) {}
    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }
    void print() const {
        std::cout << "Pair(" << first << ", " << second << ")" << std::endl;
    }
};

// 2.2 固定大小栈 (非类型模板参数)
template<typename T, size_t MaxSize = 100>
class Stack {
    T data[MaxSize];
    size_t top = 0;
public:
    bool push(const T& val) {
        if (top >= MaxSize) return false;
        data[top++] = val;
        return true;
    }
    T pop() { return data[--top]; }
    bool empty() const { return top == 0; }
    size_t size() const { return top; }
    size_t capacity() const { return MaxSize; }
};

// ============================================================================
// 3. CTAD 推导指南 (C++17)
// ============================================================================
template<typename T>
Pair(T, T) -> Pair<T, T>;

// ============================================================================
// 4. 模板特化
// ============================================================================

// 4.1 通用版本
template<typename T>
struct TypeName {
    static std::string name() { return "未知类型"; }
};

// 4.2 全特化
template<>
struct TypeName<int> {
    static std::string name() { return "int"; }
};

template<>
struct TypeName<double> {
    static std::string name() { return "double"; }
};

template<>
struct TypeName<std::string> {
    static std::string name() { return "std::string"; }
};

// 4.3 偏特化：指针类型
template<typename T>
struct TypeName<T*> {
    static std::string name() { return "指针 -> " + TypeName<T>::name(); }
};

// 4.4 偏特化：引用类型
template<typename T>
struct TypeName<T&> {
    static std::string name() { return "引用 -> " + TypeName<T>::name(); }
};

// 4.5 偏特化：const 类型
template<typename T>
struct TypeName<const T> {
    static std::string name() { return "const " + TypeName<T>::name(); }
};

// ============================================================================
// 5. 成员函数模板
// ============================================================================
class Formatter {
public:
    template<typename T>
    std::string toString(const T& val) {
        return std::to_string(val);
    }
};

// 为 string 特化
template<>
inline std::string Formatter::toString(const std::string& val) {
    return "\"" + val + "\"";
}

// ============================================================================
// 测试
// ============================================================================

void test_function_templates() {
    std::cout << "=== 1. 函数模板 ===" << std::endl;
    std::cout << "add(1, 2) = " << add(1, 2) << std::endl;
    std::cout << "add(1.5, 2.5) = " << add(1.5, 2.5) << std::endl;
    std::cout << "add<double>(11, 2.0) = " << add<double>(11, 2.0) << std::endl;
    std::cout << "multiply(3, 4.5) = " << multiply(3, 4.5) << std::endl;
    print_type(42);
    print_type(3.14);
    print_type(std::string("hello"));
}

void test_class_templates() {
    std::cout << "\n=== 2. 类模板 ===" << std::endl;
    
    Pair<int, double> p1(10, 3.14);
    p1.print();
    
    Pair<std::string, int> p2("age", 25);
    p2.print();
    
    // CTAD (C++17)
    Pair p3(1, 2);
    p3.print();
    
    // Stack
    Stack<int, 5> s;
    for (int i = 0; i < 5; ++i) s.push(i * 10);
    std::cout << "Stack 容量: " << s.capacity() << " | 大小: " << s.size() << std::endl;
    while (!s.empty()) std::cout << s.pop() << " ";
    std::cout << std::endl;
}

void test_specialization() {
    std::cout << "\n=== 3. 模板特化 ===" << std::endl;
    std::cout << "int: " << TypeName<int>::name() << std::endl;
    std::cout << "double: " << TypeName<double>::name() << std::endl;
    std::cout << "string: " << TypeName<std::string>::name() << std::endl;
    std::cout << "float: " << TypeName<float>::name() << std::endl;
    std::cout << "int*: " << TypeName<int*>::name() << std::endl;
    std::cout << "int&: " << TypeName<int&>::name() << std::endl;
    std::cout << "const int: " << TypeName<const int>::name() << std::endl;
}

void test_member_template() {
    std::cout << "\n=== 4. 成员函数模板 ===" << std::endl;
    Formatter fmt;
    std::cout << fmt.toString(42) << std::endl;
    std::cout << fmt.toString(3.14) << std::endl;
    std::cout << fmt.toString(std::string("hello")) << std::endl;
}

int main() {
    test_function_templates();
    test_class_templates();
    test_specialization();
    test_member_template();
    
    std::cout << "\n所有模板基础测试完成!" << std::endl;
    return 0;
}
