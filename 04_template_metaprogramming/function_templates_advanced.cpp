#include <iostream>
#include <type_traits>
#include <cstring>

//2.1.1 模板类型推导
template<typename T>
void printType(T value) {
    std::cout <<"值："  << value << ",类型是：" << typeid(T).name() << std::endl;
}

//2.1.2 引用类型的模板
template<typename T>
void processReference(T& value) {//如果不加&，则会拷贝，不能进行修改
    std::cout << "左值引用处理：" << value << std::endl;
    value *= 2;
}
template<typename T>
void processReference(const T& value){
    std::cout << "常量引用处理：" << value << "不可修改" << std::endl;
}


//2.1.3 模板重载
template<typename T>
T maxium(T a, T b) {
    std::cout << "模板版本：";
    return (a > b) ? a : b;
}

//重载版本：处理C风格字符串
const char* maxium(const char* a, const char* b) {
    std::cout << "重载版本：";
    return (strcmp(a, b) > 0) ? a : b;
}

//2.1.4 完美转发模板
template<typename T>
void forwardValue(T&& value) {
    std::cout << "转发值：" << value << std::endl;
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "左值引用" << std::endl;
    }else{
        std::cout << "右值引用" << std::endl;
    }
}


int main() {
    std::cout << "=== 函数模板高级特性 ===" << std::endl;
    //类型推导演示
    printType(10);
    printType(10.5);
    printType("Hello, World!");
    //引用模板演示
    int x = 10;
    processReference(x);
    std::cout << "x: " << x << std::endl; 
    const int y = 20;
    processReference(y);

    //模板重载演示
    std::cout << "最大值：" << maxium(10, 20) << std::endl;
    std::cout << "最大值：" << maxium(10.5, 20.5) << std::endl;
    std::cout << "最大值：" << maxium("Hello", "World") << std::endl;

    //完美转发演示
    int a = 100;
    forwardValue(a);
    forwardValue(200);
    forwardValue(std::move(a));
    return 0;
}

