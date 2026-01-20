#include <iostream>
#include <typeinfo>

//1.1.1  最简单的函数模板
template<typename T>
T add(T a, T b) {
    return a + b;
}

//1.1.2 多类型参数的模板
template<typename T, typename U>
auto multiply(T a, U b) -> decltype(a * b) {
    std::cout << "多模型参数的模板调试" << std::endl;
    return a * b;
}

int main() {

    std::cout << add(1, 2) << std::endl;

    std::cout << add(1.0, 2.0) << std::endl;

    std::cout << add<double>(11, 2.0) << std::endl;

    //多类型参数
    std::cout << multiply(1, 3.35) << std::endl;

    std::cout << 1 + 4.0 <<std::endl;

    return 0;

}