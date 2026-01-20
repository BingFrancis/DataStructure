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

    std::vector<int> data = {1, 2, 3, 4, 5};
    processor.process(data);
    return 0;
}