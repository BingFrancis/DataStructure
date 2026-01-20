#include <iostream>
#include<string>


//模板实例化过程
template <typename T>
class Calculator {
private:
    T result;
public:
    Calculator(T initial = T{}) : result(initial){}

    T add (T value){
        result += value;
        return result;
    }

    T subtract(T value) {
        result -= value;
        return result;
    }


    T getResult() const {
        return result;
    }

    //成员函数模板
    template<typename U>
    U convert() const {
        return static_cast<U>(result);
    }
};

//显示实例化声明

extern template class Calculator<int>;

extern template class Calculator<double>;

int main(){
    std::cout << "=== 模板实例化演示 ===" << std::endl;
    //隐式实例化：编译器自动生成具体版本
    Calculator<int> intCalc;
    std::cout << "整数计算器：" << std::endl;
    std::cout <<"加10: " << intCalc.add(10) << std::endl;
    std::cout <<"减5: " << intCalc.subtract(5) << std::endl;
    std::cout << "转换为double: " << intCalc.convert<double>() << std::endl;
    std::cout <<"结果："<< intCalc.getResult()<< std::endl;

    std::cout << "\n 双精度计算器：" << std::endl;
    Calculator<double> doubleCalc(10.5);
    std::cout << "加10.5: " << doubleCalc.add(10.5) << std::endl;
    std::cout << "减5.25: " << doubleCalc.subtract(5.25) << std::endl;
    std::cout << "转换为int: " << doubleCalc.convert<int>() << std::endl;
    std::cout << "结果：" << doubleCalc.getResult() << std::endl;

    //显示模板参数信息
    std::cout << "\n类型信息: "<< std::endl;
    std::cout << "intCalc: " << typeid(intCalc).name() << std::endl;
    std::cout << "doubleCalc: " << typeid(doubleCalc).name() << std::endl;
    return 0;
}

// 显式实例化定义（通常在单独的.cpp文件中）
template class Calculator<int>;
template class Calculator<double>;




