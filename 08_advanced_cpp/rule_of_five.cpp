// ============================================================================
// 08 - Advanced C++: Rule of 5 完整演示
// ============================================================================
// 通过自定义 MyString 类演示 C++ 五大特殊成员函数：
//   1. 析构函数
//   2. 拷贝构造函数
//   3. 拷贝赋值运算符
//   4. 移动构造函数
//   5. 移动赋值运算符
//
// 核心概念：拷贝 vs 移动的性能差异、资源所有权转移、NRVO 优化。
// ============================================================================

#include <iostream>
#include <cstring>

class MyString {
private:
    char* data;
    size_t length;
    
    void log(const char* operation) const {
        std::cout << operation << " [" << (data ? data : "null") 
                  << "] 地址: " << (void*)data << std::endl;
    }
    
public:
    // 1. 普通构造函数
    MyString(const char* str = "") {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
        log("构造");
    }
    
    // 2. 拷贝构造函数
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];  // 新分配内存
        std::strcpy(data, other.data); // 复制数据
        log("拷贝构造");
    }
    
    // 3. 移动构造函数
    MyString(MyString&& other) noexcept {
        length = other.length;
        data = other.data;           // 直接接管指针，不分配新内存
        
        // 使原对象处于有效但空的状态
        other.data = nullptr;
        other.length = 0;
        log("移动构造");
    }
    
    // 4. 析构函数
    ~MyString() {
        delete[] data;
        log("析构");
    }
    
    // 5. 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;                    // 释放旧内存
            length = other.length;
            data = new char[length + 1];      // 新分配内存
            std::strcpy(data, other.data);    // 复制数据
            log("拷贝赋值");
        }
        return *this;
    }
    
    // 6. 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;           // 释放旧内存
            length = other.length;
            data = other.data;       // 接管新指针
            
            other.data = nullptr;    // 置空原对象
            other.length = 0;
            log("移动赋值");
        }
        return *this;
    }
    
    const char* c_str() const { return data ? data : "null"; }
};

// ============================================================================
// 演示 1：拷贝 vs 移动
// ============================================================================
void demonstrate_copy_vs_move() {
    std::cout << "=== 拷贝 vs 移动演示 ===" << std::endl;
    
    // 拷贝构造：创建完整副本（分配新内存 + 复制数据）
    std::cout << "\n1. 拷贝构造:" << std::endl;
    MyString original("Original");
    MyString copy = original;  // 调用拷贝构造
    
    std::cout << "原对象: " << original.c_str() << std::endl;
    std::cout << "副本: " << copy.c_str() << std::endl;
    
    // 移动构造：转移资源所有权（不分配内存，只复制指针）
    std::cout << "\n2. 移动构造:" << std::endl;
    MyString temp("Temporary");
    MyString moved = std::move(temp);  // 调用移动构造
    
    std::cout << "原对象（被移动后）: " << temp.c_str() << std::endl;
    std::cout << "新对象: " << moved.c_str() << std::endl;
    
    // 函数返回值中的移动语义
    std::cout << "\n3. 函数返回值:" << std::endl;
    auto create_string = []() -> MyString {
        MyString local("Local in function");
        return local;  // 编译器可能使用 NRVO 优化，也可能移动
    };
    
    MyString from_function = create_string();
    std::cout << "函数返回的对象: " << from_function.c_str() << std::endl;
}

// ============================================================================
// 演示 2：参数类型差异（左值 vs 右值）
// ============================================================================
void parameter_differences() {
    std::cout << "\n=== 参数类型差异 ===" << std::endl;
    
    MyString s("Test");
    
    // 拷贝构造参数：const MyString&
    // 可以接受：左值、const左值、临时对象（但会进行拷贝）
    MyString copy1 = s;                    // OK：左值
    MyString copy2 = MyString("Temp");     // OK：临时对象触发拷贝（或移动，取决于编译器优化）
    
    // 移动构造参数：MyString&&  
    // 只能接受：右值（临时对象、std::move 的结果）
    // MyString move1 = s;                  // 错误！不能绑定左值到右值引用
    MyString move2 = std::move(s);         // OK：std::move 将左值转为右值
    MyString move3 = MyString("Direct");   // OK：临时对象是右值
}

// ============================================================================
int main() {
    demonstrate_copy_vs_move();
    parameter_differences();
    return 0;
}
