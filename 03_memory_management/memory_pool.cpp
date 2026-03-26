/**
 * 内存池（Memory Pool）实现
 *
 * 学习目标：
 * 1. 理解内存池的工作原理
 * 2. 学习如何减少内存分配开销
 * 3. 掌握固定大小内存池的实现
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <chrono>
#include <cstring>

// ==================== 简单内存池实现 =================///
template <typename T, size_t BlockSize = 256>
class SimpleMemoryPool {
private:
    struct Block {
        Block* next;
        alignas(T) char data[sizeof(T)];
    };

    std::vector<Block*> free_blocks;
    std::vector<Block*> chunks;  // 改用原始指针管理
    std::unordered_map<T*, Block*> ptr_to_block;  // 指针映射
    std::mutex mutex;
    size_t allocated_count = 0;
    size_t reused_count = 0;

    void allocateNewChunk() {
        Block* chunk = new Block[BlockSize];
        for (size_t i = 0; i < BlockSize - 1; ++i) {
            chunk[i].next = &chunk[i + 1];
        }
        chunk[BlockSize - 1].next = nullptr;  // 最后一个节点指向 nullptr
        for (size_t i = 0; i < BlockSize; ++i) {
            free_blocks.push_back(&chunk[i]);
        }
        chunks.push_back(chunk);
    }

public:
    SimpleMemoryPool() {
        allocateNewChunk();
    }

    ~SimpleMemoryPool() {
        // 手动释放所有 chunk
        for (size_t i = 0; i < chunks.size(); ++i) {
            delete[] chunks[i];
        }
    }

    // 分配内存
    T* allocate() {
        // std::lock_guard<std::mutex> lock(mutex);  // 暂时禁用 mutex 调试

        if (free_blocks.empty()) {
            allocateNewChunk();
        }

        Block* block = free_blocks.back();
        free_blocks.pop_back();
        allocated_count++;

        T* ptr = reinterpret_cast<T*>(block->data);
        ptr_to_block[ptr] = block;  // 保存映射关系
        return ptr;
    }

    // 释放内存
    void deallocate(T* ptr) {
        // std::lock_guard<std::mutex> lock(mutex);  // 暂时禁用 mutex 调试

        auto it = ptr_to_block.find(ptr);
        if (it != ptr_to_block.end()) {
            Block* block = it->second;
            block->next = nullptr;
            free_blocks.push_back(block);
            ptr_to_block.erase(it);
            reused_count++;
        }
    }

    // 统计信息
    void printStats() const {
        std::cout << "内存池统计:\n";
        std::cout << "  块大小: " << BlockSize << "\n";
        std::cout << "  已分配块数: " << allocated_count << "\n";
        std::cout << "  重复使用次数: " << reused_count << "\n";
        std::cout << "  当前可用块: " << free_blocks.size() << "\n";
        std::cout << "  总块数: " << chunks.size() * BlockSize << "\n";
    }
};

// ==================== 使用内存池的对象 =================///
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
#include <random>

void benchmarkMemoryPool() {
    std::cout << "\n=== 内存池 vs 堆分配性能对比 ===\n";

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

    // 测试2: 内存池
    std::cout << "\n测试2: 内存池\n";
    SimpleMemoryPool<Particle, 10000> pool;
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        Particle* p = pool.allocate();
        *p = Particle(i, i, i, 1, 1, 1, 100);
        p->update();
        pool.deallocate(p);
    }

    end = std::chrono::high_resolution_clock::now();
    auto pool_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "时间: " << pool_time.count() << " 微秒\n";
    
    std::cout << "\n性能提升: "
              << (double)new_delete_time.count() / pool_time.count() << " 倍\n";

    pool.printStats();
}



// ==================== 小对象优化（SOO）演示 =================///
class SmallOptimizedString {
private:
    static const size_t SMALL_BUFFER_SIZE = 16;

    char data[SMALL_BUFFER_SIZE];  // 小字符串直接使用栈缓冲区
    char* heap_ptr;                 // 大字符串使用堆内存指针
    size_t length;
    bool use_heap;

public:
    SmallOptimizedString(const char* str) : heap_ptr(nullptr), use_heap(false) {
        length = strlen(str);

        if (length + 1 > SMALL_BUFFER_SIZE) {
            // 大字符串：使用堆内存
            use_heap = true;
            heap_ptr = new char[length + 1];
            memcpy(heap_ptr, str, length + 1);
        } else {
            // 小字符串：使用栈缓冲区
            use_heap = false;
            memcpy(data, str, length + 1);
        }
    }

    // 禁止拷贝，避免浅拷贝问题
    SmallOptimizedString(const SmallOptimizedString&) = delete;
    SmallOptimizedString& operator=(const SmallOptimizedString&) = delete;

    ~SmallOptimizedString() {
        if (use_heap) {
            delete[] heap_ptr;
        }
    }

    const char* c_str() const {
        return use_heap ? heap_ptr : data;
    }

    size_t size() const { return length; }

    void print() const {
        std::cout << "String: \"" << c_str() << "\" ("
                  << "length=" << length
                  << ", is_small=" << (use_heap ? "false" : "true")
                  << ", buffer=" << (use_heap ? "heap" : "stack") << ")\n";
    }
};

void demonstrateSOO() {
    std::cout << "\n=== 小对象优化（SOO）演示 ===\n";

    SmallOptimizedString short_str("Hello");  // 使用栈
    SmallOptimizedString long_str("This is a very long string that will trigger heap allocation");

    short_str.print();
    long_str.print();

    std::cout << "\n优势：\n";
    std::cout << "1. 短字符串避免堆分配，提升性能\n";
    std::cout << "2. 减少内存碎片\n";
    std::cout << "3. 提高缓存命中率\n";
    std::cout << "std::string 通常实现了类似的优化（SSO）\n";
}

// ==================== 内存碎片演示 =================///
void demonstrateMemoryFragmentation() {
    std::cout << "\n=== 内存碎片演示 ===\n";
    std::vector<int*> pointers;

    // 分配不同大小的块
    std::cout << "分配不同大小的内存块...\n";
    for (int i = 0; i < 100; ++i) {
        size_t size = (i % 10 + 1) * 100;
        pointers.push_back(new int[size]);
    }

    // 随机释放部分块
    std::cout << "随机释放部分块...\n";
    for (int i = 0; i < 50; ++i) {
        int idx = rand() % pointers.size();
        if (pointers[idx]) {
            delete[] pointers[idx];
            pointers[idx] = nullptr;
        }
    }

    std::cout << "已分配的块: " << pointers.size() << "\n";
    int freed_count = 0;
    for (auto p : pointers) {
        if (p == nullptr) freed_count++;
    }
    std::cout << "已释放的块: " << freed_count << "\n";

    std::cout << "\n问题：\n";
    std::cout << "1. 频繁分配不同大小的块导致内存碎片\n";
    std::cout << "2. 碎片化导致虽然总内存足够，但无法分配连续大块\n";
    std::cout << "3. 内存池可以避免这个问题\n";

    // 清理
    for (auto p : pointers) {
        if (p) delete[] p;
    }
}

int main() {
    benchmarkMemoryPool();
    demonstrateSOO();
    demonstrateMemoryFragmentation();

    std::cout << "\n=== 内存池关键要点总结 ===\n";
    std::cout << "1. 内存池通过预分配减少系统调用\n";
    std::cout << "2. 固定大小池实现简单，性能优异\n";
    std::cout << "3. 小对象优化可以避免大量堆分配\n";
    std::cout << "4. 适用场景：游戏对象、网络连接、频繁创建销毁的对象\n";
    std::cout << "5. 性能提升：通常快 2-10 倍\n";
    std::cout << "6. 减少内存碎片，提高内存利用率\n";

    return 0;
}
