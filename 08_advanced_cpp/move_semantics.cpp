/**
 * 移动语义 (Move Semantics) 示例
 *
 * 移动语义允许在不复制数据的情况下转移资源所有权
 *
 * 编译：g++ -std=c++17 move_semantics.cpp -o move_semantics.exe
 */

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

// 示例类：管理动态内存
class MyString {
private:
    char* data;
    size_t size;

public:
    // 默认构造函数
    MyString() : data(nullptr), size(0) {
        cout << "默认构造" << endl;
    }

    // 构造函数
    explicit MyString(const char* str) {
        size = strlen(str);
        data = new char[size + 1];
        strcpy(data, str);
        cout << "构造: " << data << endl;
    }

    // 拷贝构造函数（深拷贝）
    MyString(const MyString& other) : size(other.size) {
        data = new char[size + 1];
        strcpy(data, other.data);
        cout << "拷贝构造: " << data << endl;
    }

    // 移动构造函数（转移资源）
    MyString(MyString&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "移动构造: " << data << endl;
    }

    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        cout << "拷贝赋值" << endl;
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new char[size + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        cout << "移动赋值" << endl;
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // 析构函数
    ~MyString() {
        if (data) {
            cout << "析构: " << data << endl;
            delete[] data;
        } else {
            cout << "析构 (空)" << endl;
        }
    }

    // 获取 C 字符串
    const char* c_str() const {
        return data ? data : "";
    }

    // 获取长度
    size_t length() const {
        return size;
    }
};

// 测试值传递（会导致拷贝）
MyString processByValue(MyString str) {
    cout << "processByValue 接收: " << str.c_str() << endl;
    return str;
}

// 测试 const 引用传递
void processByConstRef(const MyString& str) {
    cout << "processByConstRef 接收: " << str.c_str() << endl;
}

// 测试右值引用传递
void processByRvalueRef(MyString&& str) {
    cout << "processByRvalueRef 接收: " << str.c_str() << endl;
}

// 测试 std::move
MyString createString() {
    MyString str("临时对象");
    return str;  // RVO (Return Value Optimization) 可能会优化掉移动
}

int main() {
    cout << "=== 1. 构造函数 ===" << endl;
    MyString s1("Hello");
    MyString s2;
    cout << endl;

    cout << "=== 2. 拷贝构造 ===" << endl;
    MyString s3 = s1;  // 拷贝构造
    cout << endl;

    cout << "=== 3. 移动构造 ===" << endl;
    MyString s4 = std::move(s1);  // 移动构造
    cout << "s1 长度: " << s1.length() << endl;
    cout << "s4 内容: " << s4.c_str() << endl;
    cout << endl;

    cout << "=== 4. 拷贝赋值 ===" << endl;
    s2 = s3;  // 拷贝赋值
    cout << endl;

    cout << "=== 5. 移动赋值 ===" << endl;
    s2 = std::move(s3);  // 移动赋值
    cout << "s3 长度: " << s3.length() << endl;
    cout << "s2 内容: " << s2.c_str() << endl;
    cout << endl;

    cout << "=== 6. 值传递（会拷贝）===" << endl;
    MyString s5 = processByValue(MyString("World"));
    cout << endl;

    cout << "=== 7. const 引用传递（不拷贝）===" << endl;
    processByConstRef(s5);
    cout << endl;

    cout << "=== 8. 右值引用传递 ===" << endl;
    processByRvalueRef(MyString("Test"));
    processByRvalueRef(std::move(s5));
    cout << endl;

    cout << "=== 9. vector 的移动语义 ===" << endl;
    vector<MyString> vec;
    vec.push_back(MyString("One"));    // 移动构造
    vec.push_back(MyString("Two"));    // 移动构造
    vec.push_back(MyString("Three"));  // 移动构造
    cout << endl;

    cout << "=== 10. vector 扩容时的移动 ===" << endl;
    vec.push_back(MyString("Four"));  // 可能触发扩容，移动元素
    cout << endl;

    cout << "=== 11. reserve 避免扩容 ===" << endl;
    vector<MyString> vec2;
    vec2.reserve(4);  // 预分配空间，避免后续移动
    vec2.push_back(MyString("One"));
    vec2.push_back(MyString("Two"));
    vec2.push_back(MyString("Three"));
    vec2.push_back(MyString("Four"));
    cout << endl;

    return 0;
}
