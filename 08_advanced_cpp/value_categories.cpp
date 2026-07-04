// ============================================================================
// 08 - Advanced C++: 值类别 (Value Categories)
// ============================================================================
// 深入理解 C++ 的值类别体系：lvalue、rvalue、xvalue、prvalue、glvalue。
// 这是理解移动语义、完美转发、引用折叠的前提知识。
//
// C++ 值类别全景图：
//          expression
//         /          \
//     glvalue        rvalue
//     /     \        /     \
// lvalue    xvalue  prvalue
//
// 编译：g++ -std=c++17 value_categories.cpp -o value_categories
// ============================================================================

#include <iostream>
#include <string>
#include <utility>

using namespace std;

// ============================================================================
// 1. 左值 (lvalue)：有名字、可取地址的表达式
// ============================================================================
void demo_lvalue() {
    cout << "=== 1. 左值 (lvalue) ===" << endl;
    
    int x = 42;        // x 是左值
    int& ref = x;      // 左值引用绑定到左值
    int* ptr = &x;     // 可以取地址
    
    cout << "x 是左值，可取地址: " << ptr << endl;
    cout << "通过引用修改: " << ref << endl;
    
    string s = "hello";    // s 是左值
    s[0] = 'H';            // s[0] 返回左值引用，可修改
    cout << "修改后: " << s << endl;
}

// ============================================================================
// 2. 纯右值 (prvalue)：临时对象、字面量、返回值（非引用）
// ============================================================================
void demo_prvalue() {
    cout << "\n=== 2. 纯右值 (prvalue) ===" << endl;
    
    int a = 1 + 2;       // 1+2 是纯右值
    int b = 42;           // 42 是纯右值（字面量）
    
    auto getString = []() -> string {
        return string("temp");  // 返回值是纯右值
    };
    
    string s = getString();     // 纯右值用于初始化或赋值
    cout << "从纯右值初始化: " << s << endl;
    
    // 纯右值没有地址，不能取地址
    // int* p = &(1 + 2);       // 编译错误！
    // int& r = 42;             // 编译错误！不能绑定到左值引用
}

// ============================================================================
// 3. 将亡值 (xvalue)：即将被移动的表达式
// ============================================================================
void demo_xvalue() {
    cout << "\n=== 3. 将亡值 (xvalue) ===" << endl;
    
    string s1 = "hello";
    string s2 = std::move(s1);  // std::move(s1) 是将亡值
    
    cout << "s1 被移动后: " << s1 << " (长度: " << s1.length() << ")" << endl;
    cout << "s2: " << s2 << endl;
    
    // 函数返回右值引用也是将亡值
    auto getRvalueRef = [](string&& s) -> string&& {
        return std::move(s);    // 返回将亡值
    };
}

// ============================================================================
// 4. 右值引用 (rvalue reference)：绑定到右值的引用
// ============================================================================
void demo_rvalue_reference() {
    cout << "\n=== 4. 右值引用 ===" << endl;
    
    int&& r1 = 42;              // 右值引用绑定到纯右值
    cout << "r1 = " << r1 << endl;
    
    int x = 10;
    int&& r2 = std::move(x);    // 右值引用绑定到将亡值
    r2 = 100;                   // 右值引用本身是左值！
    cout << "x = " << x << endl; // x 被修改了
    
    // 关键理解：右值引用变量本身是左值
    // int&& r3 = r2;            // 编译错误！r2 是左值
    int&& r3 = std::move(r2);   // 需要 std::move 转回右值
}

// ============================================================================
// 5. 判断值类别的辅助函数
// ============================================================================
// 重载解析区分左值和右值
void process(int& x)  { cout << "  左值重载: " << x << endl; }
void process(int&& x) { cout << "  右值重载: " << x << endl; }

void demo_overload_resolution() {
    cout << "\n=== 5. 重载决议演示 ===" << endl;
    
    int a = 10;
    process(a);             // 调用左值版本
    process(20);            // 调用右值版本
    process(std::move(a));  // 调用右值版本
    
    int&& r = 30;
    process(r);             // 调用左值版本！（右值引用变量是左值）
    process(std::move(r));  // 调用右值版本
}

// ============================================================================
// 6. 实际应用：根据值类别选择拷贝或移动
// ============================================================================
class Buffer {
    char* data;
    size_t size;
public:
    Buffer(size_t n) : size(n), data(new char[n]) {
        cout << "  Buffer 构造 (" << n << " bytes)" << endl;
    }
    
    // 拷贝：深拷贝
    Buffer(const Buffer& other) : size(other.size), data(new char[other.size]) {
        copy(other.data, other.data + size, data);
        cout << "  Buffer 拷贝构造" << endl;
    }
    
    // 移动：浅拷贝 + 置空
    Buffer(Buffer&& other) noexcept : size(other.size), data(other.data) {
        other.data = nullptr;
        other.size = 0;
        cout << "  Buffer 移动构造" << endl;
    }
    
    ~Buffer() { delete[] data; }
};

void demo_practical_usage() {
    cout << "\n=== 6. 实际应用：自动选择拷贝/移动 ===" << endl;
    
    Buffer buf(1024);
    
    cout << "拷贝:" << endl;
    Buffer copy = buf;               // 左值 → 拷贝构造
    
    cout << "移动:" << endl;
    Buffer moved = std::move(buf);   // 将亡值 → 移动构造
    
    cout << "临时对象（移动）:" << endl;
    Buffer temp = Buffer(512);       // 纯右值 → 移动构造（或构造省略）
}

// ============================================================================
// 7. 引用折叠规则
// ============================================================================
template<typename T>
void forward_demo(T&& arg) {
    // T&& 是转发引用（universal reference）
    // 传入左值时，T 推导为 int&， T&& → int& && → int&（折叠）
    // 传入右值时，T 推导为 int，  T&& → int&&
    
    // 不完美转发
    cout << "  直接传递: ";
    process(arg);                    // arg 是左值，永远调用左值版本
    
    // 完美转发
    cout << "  完美转发: ";
    process(std::forward<T>(arg));   // 保留原始值类别
}

void demo_reference_collapsing() {
    cout << "\n=== 7. 引用折叠与完美转发 ===" << endl;
    
    int x = 42;
    cout << "传入左值:" << endl;
    forward_demo(x);                 // T = int&
    
    cout << "传入右值:" << endl;
    forward_demo(100);               // T = int
    
    cout << "传入 std::move:" << endl;
    forward_demo(std::move(x));      // T = int
}

// ============================================================================
// 8. 速查表
// ============================================================================
void demo_cheatsheet() {
    cout << "\n=== 8. 值类别速查表 ===" << endl;
    cout << "┌─────────────────────────────────────────────────────┐" << endl;
    cout << "│ 表达式                    │ 值类别   │ 可绑定到    │" << endl;
    cout << "├─────────────────────────────────────────────────────┤" << endl;
    cout << "│ 变量名 x                  │ lvalue   │ T&, const T&│" << endl;
    cout << "│ 字面量 42                 │ prvalue  │ T&&, const T&│" << endl;
    cout << "│ std::move(x)              │ xvalue   │ T&&, const T&│" << endl;
    cout << "│ 函数返回 T&&              │ xvalue   │ T&&, const T&│" << endl;
    cout << "│ 函数返回 T (非引用)       │ prvalue  │ T&&, const T&│" << endl;
    cout << "│ 解引用 *ptr               │ lvalue   │ T&, const T&│" << endl;
    cout << "│ 数组下标 arr[0]           │ lvalue   │ T&, const T&│" << endl;
    cout << "│ 字符串字面量 \"hello\"     │ lvalue   │ const char(&)[N]│" << endl;
    cout << "└─────────────────────────────────────────────────────┘" << endl;
}

// ============================================================================
int main() {
    demo_lvalue();
    demo_prvalue();
    demo_xvalue();
    demo_rvalue_reference();
    demo_overload_resolution();
    demo_practical_usage();
    demo_reference_collapsing();
    demo_cheatsheet();
    
    return 0;
}
