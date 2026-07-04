// ============================================================================
// 02 - Smart Pointers: 现代 C++ 项目实战 (Modern C++ Project)
// ============================================================================
// 综合演示 unique_ptr、shared_ptr 在实际项目中的使用场景：
// 数据处理器、观察者模式、对象工厂等。
// ============================================================================

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>
#include <algorithm>

class DataProcessor{
private:
    std::vector<std::function<void(std::vector<int>&)>> pipeline;
public:
    template<typename F>
    void add_stage(F&& stage) {
        pipeline.emplace_back(std::forward<F>(stage));
    }
    auto process(std::vector<int>& data) {
        auto start = std::chrono::high_resolution_clock::now();
        for (auto& stage : pipeline) {
            stage(data);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "处理耗时：" << elapsed.count() << "秒" << std::endl;
    }
};


int main(){
    DataProcessor processor;
    processor.add_stage([](std::vector<int>& data) {
        std::cout << "阶段1：数据处理" << std::endl;
        std::sort(data.begin(), data.end());
    });

    processor.add_stage([](std::vector<int>& data) {
        std::cout << "阶段2：数据处理" << std::endl;
        for (auto& value : data) {
            value *= 2;
        }
    });

    processor.add_stage([](std::vector<int>& data) {
        std::cout << "阶段3：数据处理" << std::endl;
        for (auto& value : data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    });

    std::vector<int> data = {7, 6, 3, 4, 5,20,30,20,399,1289,21312,233,2131,4441,12412124,121441151,111111};
    processor.process(data);
    return 0;
}