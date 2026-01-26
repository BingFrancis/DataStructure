/**
 * 完美转发 (Perfect Forwarding) 示例
 *
 * 完美转发允许将参数以原始的值类别（左值或右值）转发给其他函数
 *
 * 编译：g++ -std=c++17 perfect_forwarding.cpp -o perfect_forwarding.exe
 */

#include <iostream>
#include <utility>
#include <string>

using namespace std;

// 接收左值引用的函数
void processLvalue(string& str) {
    cout << "处理左值: " << str << endl;
}

// 接收右值引用的函数
void processRvalue(string&& str) {
    cout << "处理右值: " << str << endl;
}

// 接收 const 左值的函数
void processConstLvalue(const string& str) {
    cout << "处理 const 左值: " << str << endl;
}

// 不完美的转发（总是转发为左值）
template<typename T>
void imperfectForward(T&& arg) {
    processLvalue(arg);  // 即使 arg 是右值，也会作为左值处理
}

// 完美的转发（保持原始值类别）
template<typename T>
void perfectForward(T&& arg) {
    if constexpr (is_lvalue_reference_v<T>) {
        processLvalue(arg);
    } else {
        processRvalue(std::forward<T>(arg));
    }
}

// 使用 std::forward 实现完美转发
template<typename T>
void wrapper(T&& arg) {
    processLvalue(std::forward<T>(arg));      // 如果 T 是左值引用类型
    processRvalue(std::forward<T>(arg));      // 如果 T 是右值引用类型
    processConstLvalue(std::forward<T>(arg));  // 如果 T 是 const 左值引用类型
}

// 更实际的例子：包装容器的 emplace_back
class MyContainer {
private:
    string data;

public:
    // 接受左值
    explicit MyContainer(const string& str) : data(str) {
        cout << "MyContainer 构造 (const string&): " << data << endl;
    }

    // 接受右值
    explicit MyContainer(string&& str) : data(std::move(str)) {
        cout << "MyContainer 构造 (string&&): " << data << endl;
    }

    const string& getData() const { return data; }
};

// 工厂函数，使用完美转发
template<typename T, typename... Args>
MyContainer createMyContainer(Args&&... args) {
    return MyContainer(std::forward<Args>(args)...);
}

// 另一个实际例子：包装 push_back 和 emplace_back
class Vector {
private:
    vector<string> data;

public:
    // 普通 push_back
    void push_back(const string& value) {
        data.push_back(value);
    }

    void push_back(string&& value) {
        data.push_back(std::move(value));
    }

    // 使用完美转发封装
    template<typename T>
    void emplace_back(T&& value) {
        data.push_back(std::forward<T>(value));
    }

    void print() const {
        for (const auto& str : data) {
            cout << str << " ";
        }
        cout << endl;
    }
};

// 可变参数模板的完美转发
template<typename... Args>
void printAll(Args&&... args) {
    (cout << ... << args) << endl;  // C++17 折叠表达式
}

template<typename... Args>
void forwardAll(Args&&... args) {
    printAll(std::forward<Args>(args)...);
}

int main() {
    string lvalue = "Hello";
    const string constLvalue = "Const";

    cout << "=== 1. 直接调用 ===" << endl;
    processLvalue(lvalue);
    processRvalue(string("World"));
    processConstLvalue(constLvalue);
    cout << endl;

    cout << "=== 2. 不完美转发 ===" << endl;
    imperfectForward(lvalue);
    imperfectForward(string("Test"));  // 调用 processLvalue，不是 processRvalue
    cout << endl;

    cout << "=== 3. 完美转发 ===" << endl;
    perfectForward(lvalue);        // 转发为左值
    perfectForward(string("Test")); // 转发为右值
    cout << endl;

    cout << "=== 4. std::forward 示例 ===" << endl;
    string s1 = "Left";
    string s2 = "Right";

    wrapper(s1);           // T 推导为 string&，转发为左值
    wrapper(std::move(s2)); // T 推导为 string，转发为右值
    wrapper(constLvalue);   // T 推导为 const string&，转发为 const 左值
    cout << endl;

    cout << "=== 5. 工厂函数使用完美转发 ===" << endl;
    string str1 = "First";
    auto c1 = createMyContainer<string>(str1);           // 调用 const string& 版本
    auto c2 = createMyContainer<string>(string("Second")); // 调用 string&& 版本
    cout << endl;

    cout << "=== 6. Vector 的完美转发 ===" << endl;
    Vector vec;
    string s3 = "A", s4 = "B";

    vec.emplace_back(s3);              // 传递左值
    vec.emplace_back(string("C"));     // 传递右值
    vec.emplace_back(std::move(s4));   // 移动语义

    vec.print();
    cout << endl;

    cout << "=== 7. 可变参数完美转发 ===" << endl;
    forwardAll(1, 2.5, "Hello", 'X');
    cout << endl;

    cout << "=== 8. 引用折叠规则 ===" << endl;
    cout << "string& + & = " << is_same_v<string&, string&> << endl;
    cout << "string&& + & = " << is_same_v<string&&&, string&> << endl;
    cout << "string& + && = " << is_same_v<string&, string&> << endl;
    cout << "string&& + && = " << is_same_v<string&&, string&&> << endl;

    return 0;
}
