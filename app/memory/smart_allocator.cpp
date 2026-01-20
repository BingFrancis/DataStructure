/**
 * 智能分配器和内存优化技术
 *
 * 学习目标：
 * 1. 理解自定义分配器
 * 2. 学习 Arena 分配器（线性分配）
 * 3. 掌握栈分配器
 * 4. 了解代理模式优化分配
 */

#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <new>
#include <cassert>

// ==================== Arena 分配器（线性分配器）===================///
/**
 * Arena 分配器特点：
 * - 线性分配，永不回收单个对象
 * - 分配极快（只需移动指针）
 * - 一次性释放所有内存
 * - 适合：单帧游戏对象、临时计算
 */
class ArenaAllocator {
private:
    char* buffer;
      capacity;
    size_t offset;

public:
    ArenaAllocator(size_t size) : capacity(size), offset(0) {
        buffer = static_cast<char*>(::operator new(size));
    }

    ~ArenaAllocator() {
        ::operator delete(buffer);
    }

    // 分配内存（对齐到8字节）
    void* allocate(size_t size) {
        size_t aligned_offset = (offset + 7) & ~7;  // 对齐到8字节

        if (aligned_offset + size > capacity) {
            throw std::bad_alloc();
        }

        offset = aligned_offset + size;
        return buffer + aligned_offset;
    }

    // 重置分配器（释放所有内存）
    void reset() {
        offset = 0;
    }

    // 获取已使用内存
    size_t getUsage() const {
        return offset;
    }

    size_t getCapacity() const {
        return capacity;
    }
};

// ==================== 栈分配器 =================///
/**
 * 栈分配器特点：
 * - LIFO 顺序释放
 * - 支持检查点（checkpoint）和回滚
 * - 适合：作用域临时对象、递归算法
 */
class StackAllocator {
private:
    struct Marker {
        size_t offset;
        Marker* prev;
    };

    char* buffer;
    size_t capacity;
    size_t offset;
    Marker* top_marker;

public:
    StackAllocator(size_t size) : capacity(size), offset(0), top_marker(nullptr) {
        buffer = static_cast<char*>(::operator new(size));
    }

    ~StackAllocator() {
        ::operator delete(buffer);
    }

    // 分配内存
    void* allocate(size_t size) {
        size_t aligned_offset = (offset + 7) & ~7;

        if (aligned_offset + size > capacity) {
            throw std::bad_alloc();
        }

        offset = aligned_offset + size;
        return buffer + aligned_offset;
    }

    // 创建检查点
    Marker* getMarker() {
        auto* marker = static_cast<Marker*>(allocate(sizeof(Marker)));
        marker->offset = offset;
        marker->prev = top_marker;
        top_marker = marker;
        return marker;
    }

    // 回滚到检查点
    void freeToMarker(Marker* marker) {
        if (marker) {
            offset = marker->offset;
            top_marker = marker->prev;
        }
    }

    // 释放到最近检查点
    void pop() {
        if (top_marker) {
            freeToMarker(top_marker->prev);
        }
    }

    // 获取当前使用量
    size_t getUsage() const {
        return offset;
    }
};

// ==================== 链表分配器（Free List）===================///
/**
 * 链表分配器特点：
 * - 管理释放的块
 * - 支持分配和释放
 * - 适合：固定大小对象
 */
class FreeListAllocator {
private:
    struct Block {
        Block* next;
        size_t size;
    };

    char* buffer;
    size_t capacity;
    Block* free_list;

public:
    FreeListAllocator(size_t size) : capacity(size) {
        buffer = static_cast<char*>(::operator new(size));
        free_list = reinterpret_cast<Block*>(buffer);
        free_list->next = nullptr;
        free_list->size = size;
    }

    ~FreeListAllocator() {
        ::operator delete(buffer);
    }

    // 分配内存
    void* allocate(size_t size) {
        // 查找足够大的块
        Block* prev = nullptr;
        Block* current = free_list;

        while (current && current->size < size) {
            prev = current;
            current = current->next;
        }

        if (!current) {
            throw std::bad_alloc();
        }

        // 如果块足够大，分割它
        if (current->size > size + sizeof(Block)) {
            Block* remaining = reinterpret_cast<Block*>(
                reinterpret_cast<char*>(current) + size + sizeof(Block));
            remaining->size = current->size - size - sizeof(Block);
            remaining->next = current->next;

            if (prev) {
                prev->next = remaining;
            } else {
                free_list = remaining;
            }
            current->size = size;
        } else {
            // 移除块
            if (prev) {
                prev->next = current->next;
            } else {
                free_list = current->next;
            }
        }

        return reinterpret_cast<char*>(current) + sizeof(Block);
    }

    // 释放内存
    void deallocate(void* ptr) {
        if (!ptr) return;

        Block* block = reinterpret_cast<Block*>(
            static_cast<char*>(ptr) - sizeof(Block));

        // 插入到空闲链表（简单实现：插在开头）
        block->next = free_list;
        free_list = block;

        // 实际实现应该合并相邻的空闲块（这里简化）
    }
};

// ==================== 自定义 STL 分配器 =================///
template <typename T>
class ArenaAllocatorWrapper {
private:
    ArenaAllocator* arena;

public:
    using value_type = T;

    ArenaAllocatorWrapper(ArenaAllocator* a) : arena(a) {}

    template <typename U>
    ArenaAllocatorWrapper(const ArenaAllocatorWrapper<U>& other) : arena(other.arena) {}

    T* allocate(size_t n) {
        return static_cast<T*>(arena->allocate(n * sizeof(T)));
    }

    void deallocate(T* p, size_t) {
        // Arena 分配器不支持单独释放
    }

    // 为了使用不同类型
    template <typename U>
    struct rebind {
        using other = ArenaAllocatorWrapper<U>;
    };
};

template <typename T, typename U>
bool operator==(const ArenaAllocatorWrapper<T>& a, const ArenaAllocatorWrapper<U>& b) {
    return a.arena == b.arena;
}

template <typename T, typename U>
bool operator!=(const ArenaAllocatorWrapper<T>& a, const ArenaAllocatorWrapper<U>& b) {
    return a.arena != b.arena;
}

// ==================== 代理模式优化分配 =================///
/**
 * 代理分配器：根据大小选择不同的策略
 * - 小对象：使用栈或内存池
 * - 大对象：直接使用 new/delete
 */
template <typename T>
class ProxyAllocator {
public:
    using value_type = T;

    T* allocate(size_t n) {
        size_t bytes = n * sizeof(T);

        if (bytes <= 64) {
            // 小对象：使用快速分配
            return static_cast<T*>(::operator new(bytes));
        } else if (bytes <= 4096) {
            // 中等对象：使用普通分配
            return static_cast<T*>(::operator new(bytes));
        } else {
            // 大对象：对齐分配
            return static_cast<T*>(std::aligned_alloc(64, bytes));
        }
    }

    void deallocate(T* p, size_t) {
        ::operator delete(p);
    }
};

// ==================== 性能测试 =================///
#include <chrono>
#include <random>

void testArenaAllocator() {
    std::cout << "=== Arena 分配器性能测试 ===\n";

    const int iterations = 1000000;

    // 测试 new/delete
    std::cout << "\n1. new/delete:\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        int* p = new int(i);
        delete p;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto new_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "时间: " << new_time.count() << " 微秒\n";

    // 测试 Arena 分配器
    std::cout << "\n2. Arena 分配器:\n";
    ArenaAllocator arena(1024 * 1024);  // 1MB
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        int* p = static_cast<int*>(arena.allocate(sizeof(int)));
        *p = i;
        // 不释放，一次性重置
    }

    arena.reset();  // 一次性释放

    end = std::chrono::high_resolution_clock::now();
    auto arena_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "时间: " << arena_time.count() << " 微秒\n";
    std::cout << "内存使用: " << arena.getUsage() << " / " << arena.getCapacity() << " 字节\n";

    std::cout << "\n性能提升: " << (double)new_time.count() / arena_time.count() << " 倍\n";
}

void testStackAllocator() {
    std::cout << "\n=== 栈分配器测试 ===\n";

    StackAllocator stack(1024 * 1024);

    // 分配内存
    int* a = static_cast<int*>(stack.allocate(sizeof(int) * 100));
    float* b = static_cast<float*>(stack.allocate(sizeof(float) * 50));

    std::cout << "初始使用: " << stack.getUsage() << " 字节\n";

    // 创建检查点
    auto* marker = stack.getMarker();
    std::cout << "创建检查点\n";

    // 分配更多内存
    double* c = static_cast<double*>(stack.allocate(sizeof(double) * 200));
    std::cout << "分配后使用: " << stack.getUsage() << " 字节\n";

    // 回滚到检查点
    stack.freeToMarker(marker);
    std::cout << "回滚后使用: " << stack.getUsage() << " 字节\n";
}

void testCustomSTLAllocator() {
    std::cout << "\n=== 自定义 STL 分配器测试 ===\n";

    ArenaAllocator arena(10 * 1024 * 1024);  // 10MB
    ArenaAllocatorWrapper<int> allocator(&arena);

    // 使用自定义分配器的 vector
    std::vector<int, ArenaAllocatorWrapper<int>> vec(allocator);

    std::cout << "添加元素...\n";
    for (int i = 0; i < 1000000; ++i) {
        vec.push_back(i);
    }

    std::cout << "Vector 大小: " << vec.size() << "\n";
    std::cout << "Arena 使用: " << arena.getUsage() << " / " << arena.getCapacity() << " 字节\n";

    std::cout << "\n访问元素...\n";
    int sum = 0;
    for (size_t i = 0; i < vec.size(); i += 100000) {
        sum += vec[i];
    }
    std::cout << "部分元素和: " << sum << "\n";
}

void testAlignment() {
    std::cout << "\n=== 内存对齐测试 ===\n";

    struct Unaligned {
        char a;
        int b;
        double c;
    };

    struct Aligned {
        double c;
        int b;
        char a;
    };

    std::cout << "未对齐结构体: " << sizeof(Unaligned) << " 字节\n";
    std::cout << "对齐结构体: " << sizeof(Aligned) << " 字节\n";

    // 使用 alignas 指定对齐
    struct alignas(16) ExplicitAligned {
        int a;
        int b;
    };

    ExplicitAligned aligned_obj;
    std::cout << "显式对齐(16字节)结构体: " << sizeof(ExplicitAligned) << " 字节\n";
    std::cout << "地址是否16字节对齐: " << (reinterpret_cast<uintptr_t>(&aligned_obj) % 16 == 0) << "\n";
}

// ==================== 内存池 vs Arena 分配器对比 =================///
void benchmarkDifferentAllocators() {
    std::cout << "\n=== 不同分配器对比 ===\n";

    const int iterations = 100000;
    const int object_size = 32;

    // 1. new/delete
    std::cout << "\n1. new/delete:\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        char* p = new char[object_size];
        delete[] p;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "时间: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " 微秒\n";

    // 2. Arena 分配器
    std::cout << "\n2. Arena 分配器:\n";
    ArenaAllocator arena(iterations * object_size * 2);
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        arena.allocate(object_size);
    }
    arena.reset();

    end = std::chrono::high_resolution_clock::now();
    std::cout << "时间: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " 微秒\n";

    // 3. 栈分配
    std::cout << "\n3. 栈分配:\n";
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        char buffer[object_size];  // 栈分配
        buffer[0] = i;  // 避免优化
    }

    end = std::chrono::high_resolution_clock::now();
    std::cout << "时间: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " 微秒\n";

    std::cout << "\n结论：\n";
    std::cout << "- 栈分配最快（只需移动指针）\n";
    std::cout << "- Arena 分配器次之（但支持批量分配）\n";
    std::cout << "- new/delete 最慢（需要系统调用）\n";
}

int main() {
    testArenaAllocator();
    testStackAllocator();
    testCustomSTLAllocator();
    testAlignment();
    benchmarkDifferentAllocators();

    std::cout << "\n=== 智能分配器关键要点总结 ===\n";
    std::cout << "1. Arena 分配器：线性分配，极快，适合临时对象\n";
    std::cout << "2. 栈分配器：LIFO，支持检查点和回滚\n";
    std::cout << "3. 链表分配器：支持分配和释放，适合固定大小对象\n";
    std::cout << "4. 自定义 STL 分配器：可以为 STL 容器优化分配\n";
    std::cout << "5. 内存对齐：提升访问速度，SIMD 编程必备\n";
    std::cout << "6. 根据场景选择合适的分配器\n";
    std::cout << "7. 游戏开发常用：Arena 分配器（每帧重置）\n";
    std::cout << "8. 服务器开发常用：内存池（减少分配开销）\n";

    return 0;
}
