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
    // 普通构造函数
    MyString(const char* str = "") {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
        log("构造");
    }
    
    // 拷贝构造函数
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];  // 新分配内存
        std::strcpy(data, other.data); // 复制数据
        log("拷贝构造");
    }
    
    // 移动构造函数
    MyString(MyString&& other) noexcept {
        length = other.length;
        data = other.data;           // 直接接管指针
        
        // 使原对象处于有效但空的状态
        other.data = nullptr;
        other.length = 0;
        log("移动构造");
    }
    
    // 析构函数
    ~MyString() {
        delete[] data;
        log("析构");
    }
    
    // 拷贝赋值运算符
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
    
    // 移动赋值运算符
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

void demonstrate_copy_vs_move() {
    std::cout << "=== 拷贝 vs 移动演示 ===" << std::endl;
    
    // 1. 拷贝构造：创建完整副本
    std::cout << "\n1. 拷贝构造:" << std::endl;
    MyString original("Original");
    MyString copy = original;  // 调用拷贝构造
    
    std::cout << "原对象: " << original.c_str() << std::endl;
    std::cout << "副本: " << copy.c_str() << std::endl;
    
    // 2. 移动构造：转移资源所有权
    std::cout << "\n2. 移动构造:" << std::endl;
    MyString temp("Temporary");
    MyString moved = std::move(temp);  // 调用移动构造
    
    std::cout << "原对象（被移动后）: " << temp.c_str() << std::endl;
    std::cout << "新对象: " << moved.c_str() << std::endl;
    
    // 3. 函数中的移动语义
    std::cout << "\n3. 函数返回值:" << std::endl;
    auto create_string = []() -> MyString {
        MyString local("Local in function");
        return local;  // 可能发生移动构造（NRVO优化）
    };
    
    MyString from_function = create_string();
    std::cout << "函数返回的对象: " << from_function.c_str() << std::endl;
}

void parameter_differences() {
    std::cout << "\n=== 参数类型差异 ===" << std::endl;
    
    MyString s("Test");
    
    // 拷贝构造参数：const MyString&
    // 可以接受：左值、const左值、临时对象（但会进行拷贝）
    MyString copy1 = s;                    // OK：左值
    MyString copy2 = MyString("Temp");     // OK：但会先构造临时对象，再拷贝
    
    // 移动构造参数：MyString&&  
    // 只能接受：右值（临时对象、std::move的结果）
    // MyString move1 = s;                  // 错误！不能绑定左值到右值引用
    MyString move2 = std::move(s);         // OK：std::move将左值转为右值
    MyString move3 = MyString("Direct");   // OK：临时对象是右值
}

int main() {
    demonstrate_copy_vs_move();
    parameter_differences();
    return 0;
}

