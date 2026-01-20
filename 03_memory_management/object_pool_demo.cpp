/**
 * 对象池演示程序
 * 
 * 学习目标：
 * 1. 理解对象池的工作原理和使用场景
 * 2. 掌握对象池的接口使用方法
 * 3. 对比对象池与普通内存分配的性能差异
 */

#include "object.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <ctime>

// ==================== 使用对象池的对象 =================///
class Particle {
public:
    float x, y, z;
    float vx, vy, vz;
    int lifetime;

    Particle(float x = 0, float y = 0, float z = 0,
             float vx = 0, float vy = 0, float vz = 0,
             int lifetime = 100)
        : x(x), y(y), z(z), vx(vx), vy(vy), vz(vz), lifetime(lifetime) {}

    void update() {
        x += vx;
        y += vy;
        z += vz;
        lifetime--;
    }

    bool isAlive() const { return lifetime > 0; }
};

// ==================== 性能对比测试 =================///
void benchmarkObjectPool() {
    std::cout << "\n=== 对象池 vs 堆分配性能对比 ===\n";

    const int iterations = 100000;

    // 测试1: new/delete
    std::cout << "\n测试1: new/delete\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        Particle* p = new Particle(i, i, i, 1, 1, 1, 100);
        p->update();
        delete p;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto new_delete_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "时间: " << new_delete_time.count() << " 微秒\n";

    // 测试2: 对象池
    std::cout << "\n测试2: 对象池\n";
    ObjectPool<Particle, 100000> obj_pool;
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        Particle* p = obj_pool.acquire(i, i, i, 1, 1, 1, 100);
        p->update();
        obj_pool.release(p);
    }

    end = std::chrono::high_resolution_clock::now();
    auto obj_pool_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "时间: " << obj_pool_time.count() << " 微秒\n";

    std::cout << "\n性能提升: "
              << (double)new_delete_time.count() / obj_pool_time.count() << " 倍\n";
}

// ==================== 粒子系统示例 =================///
void particleSystemDemo() {
    std::cout << "\n=== 粒子系统演示 ===\n";

    const int particle_count = 1000;
    const int frames = 100;

    // 使用对象池
    ObjectPool<Particle, particle_count> pool;
    std::vector<Particle*> particles;

    std::cout << "创建 " << particle_count << " 个粒子...\n";

    // 创建粒子
    for (int i = 0; i < particle_count; ++i) {
        float x = static_cast<float>(rand() % 100);
        float y = static_cast<float>(rand() % 100);
        float vx = (rand() % 10 - 5) / 10.0f;
        float vy = (rand() % 10 - 5) / 10.0f;
        int lifetime = rand() % 100 + 50;

        particles.push_back(pool.acquire(x, y, 0, vx, vy, 0, lifetime));
    }

    std::cout << "活跃粒子: " << pool.getActiveCount() << "\n";
    std::cout << "可用粒子: " << pool.getAvailableCount() << "\n";

    // 模拟粒子更新
    std::cout << "\n模拟 " << frames << " 帧更新...\n";

    for (int frame = 0; frame < frames; ++frame) {
        // 更新所有粒子
        for (auto& p : particles) {
            if (p->isAlive()) {
                p->update();
            } else {
                // 粒子死亡，重新分配
                pool.release(p);
                float x = static_cast<float>(rand() % 100);
                float y = static_cast<float>(rand() % 100);
                float vx = (rand() % 10 - 5) / 10.0f;
                float vy = (rand() % 10 - 5) / 10.0f;
                int lifetime = rand() % 100 + 50;

                p = pool.acquire(x, y, 0, vx, vy, 0, lifetime);
            }
        }
    }

    std::cout << "活跃粒子: " << pool.getActiveCount() << "\n";

    // 清理
    for (auto& p : particles) {
        pool.release(p);
    }

    std::cout << "所有粒子已释放\n";
    std::cout << "活跃粒子: " << pool.getActiveCount() << "\n";
}

// ==================== 简单使用示例 =================///
void simpleUsageDemo() {
    std::cout << "\n=== 简单使用示例 ===\n";

    // 创建对象池
    ObjectPool<std::string, 10> string_pool;

    // 获取字符串对象
    std::string* str1 = string_pool.acquire("Hello");
    std::string* str2 = string_pool.acquire("World");
    std::string* str3 = string_pool.acquire("Object Pool");

    std::cout << "字符串1: " << *str1 << "\n";
    std::cout << "字符串2: " << *str2 << "\n";
    std::cout << "字符串3: " << *str3 << "\n";

    std::cout << "活跃对象数: " << string_pool.getActiveCount() << "\n";
    std::cout << "可用对象数: " << string_pool.getAvailableCount() << "\n";

    // 释放对象
    string_pool.release(str1);
    string_pool.release(str2);
    string_pool.release(str3);

    std::cout << "释放后活跃对象数: " << string_pool.getActiveCount() << "\n";
    std::cout << "释放后可用对象数: " << string_pool.getAvailableCount() << "\n";
}

// ==================== 异常情况测试 =================///
void exceptionHandlingDemo() {
    std::cout << "\n=== 异常情况测试 ===\n";

    // 创建小对象池
    ObjectPool<int, 3> int_pool;

    // 正常获取对象
    int* a = int_pool.acquire(1);
    int* b = int_pool.acquire(2);
    int* c = int_pool.acquire(3);

    std::cout << "获取了3个对象，活跃对象数: " << int_pool.getActiveCount() << "\n";

    // 尝试获取第四个对象（应该抛出异常）
    try {
        int* d = int_pool.acquire(4);
        std::cout << "错误：应该抛出异常但未抛出\n";
    } catch (const std::runtime_error& e) {
        std::cout << "正确捕获异常: " << e.what() << "\n";
    }

    // 释放一个对象后可以重新获取
    int_pool.release(a);
    std::cout << "释放一个对象后可用对象数: " << int_pool.getAvailableCount() << "\n";

    int* new_a = int_pool.acquire(10);
    std::cout << "重新获取对象成功，值: " << *new_a << "\n";

    // 清理
    int_pool.release(b);
    int_pool.release(c);
    int_pool.release(new_a);
}

int main() {
    // 初始化随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "=== 对象池演示程序 ===\n";

    simpleUsageDemo();
    exceptionHandlingDemo();
    particleSystemDemo();
    benchmarkObjectPool();

    std::cout << "\n=== 对象池关键要点总结 ===\n";
    std::cout << "1. 对象池通过预分配减少系统调用\n";
    std::cout << "2. 适用于频繁创建销毁对象的场景\n";
    std::cout << "3. 提供 acquire() 和 release() 接口管理对象生命周期\n";
    std::cout << "4. 支持任意类型的对象\n";
    std::cout << "5. 性能提升显著，减少内存碎片\n";
    std::cout << "6. 适用场景：游戏对象、网络连接、频繁创建销毁的对象\n";

    return 0;
}