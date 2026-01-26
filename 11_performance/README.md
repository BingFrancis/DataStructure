# 11 - Performance Optimization（性能优化）

本模块涵盖 C++ 性能优化的各种技术和工具。

## 目录结构

```
11_performance/
├── profiling/               # 性能分析工具使用
│   ├── gprof_example.cpp   # gprof 示例
│   ├── perf_example.cpp    # perf 示例
│   └── valgrind_example.cpp# Valgrind/Memcheck 示例
├── memory_optimization/    # 内存优化
│   ├── cache_friendly.cpp  # 缓存友好代码
│   ├── memory_pool_opt.cpp # 内存池优化
│   └── soa_aos.cpp         # SoA vs AoS
├── cpu_optimization/       # CPU 优化
│   ├── branch_prediction.cpp # 分支预测优化
│   ├── loop_unrolling.cpp   # 循环展开
│   ├── simd_example.cpp     # SIMD 向量化
│   └── multithreading.cpp   # 多线程优化
├── compile_optimization/   # 编译优化
│   ├── flags_guide.md      # 编译标志指南
│   ├── lto_example.cpp     # LTO 示例
│   └── pg_example.cpp      # PGO 示例
└── README.md               # 本文件
```

## 性能优化原则

### 1. 测量先行

**永远不要假设，要测量！**

```cpp
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();
// 执行代码
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
```

### 2. 80-20 法则

- 80% 的时间花在 20% 的代码上
- 优化热点代码，而非所有代码
- 使用 profiler 找到真正的瓶颈

### 3. 算法优先

选择正确的时间复杂度比微优化更重要：

| 操作 | 时间复杂度 | 说明 |
|------|-----------|------|
| O(n²) → O(n log n) | 巨大提升 | 排序等 |
| O(n) → O(log n) | 显著提升 | 搜索等 |
| O(log n) → O(1) | 有提升 | 缓存查找等 |

## 优化策略

### 内存优化

#### 缓存友好

```cpp
 // ❌ 不缓存友好（间接访问）
struct Node { int value; Node* next; };
for (Node* p = head; p; p = p->next) {
    sum += p->value;  // 可能频繁缓存未命中
}

// ✅ 缓存友好（连续访问）
std::vector<int> data;
for (int val : data) {
    sum += val;  // 空间局部性好
}
```

#### SoA vs AoS

```cpp
// AoS (Array of Structures) - 缓存不友好
struct Particle { float x, y, z, mass; };
std::vector<Particle> particles;

// SoA (Structure of Arrays) - 缓存友好
struct Particles {
    std::vector<float> x, y, z;
    std::vector<float> mass;
};
```

### CPU 优化

#### 分支预测

```cpp
// ❌ 分支预测失败率高
for (int i = 0; i < n; ++i) {
    if (data[i] > threshold) {
        sum += data[i];
    }
}

// ✅ 分支可预测（排序后）
std::sort(data.begin(), data.end());
for (int i = 0; i < n; ++i) {
    if (data[i] > threshold) {  // 分支变得可预测
        sum += data[i];
    }
}
```

#### SIMD 向量化

```cpp
// 自动向量化（编译器自动优化）
for (int i = 0; i < n; ++i) {
    c[i] = a[i] + b[i];
}

// 手动向量化（使用 SIMD intrinsics）
#include <immintrin.h>
__m256 va = _mm256_load_ps(a + i);
__m256 vb = _mm256_load_ps(b + i);
__m256 vc = _mm256_add_ps(va, vb);
_mm256_store_ps(c + i, vc);
```

### 并发优化

#### 线程池

```cpp
// 避免频繁创建/销毁线程
ThreadPool pool(std::thread::hardware_concurrency());

// 提交任务
auto result = pool.enqueue([...] { /* 任务 */ });
```

#### 无锁数据结构

```cpp
std::atomic<int> counter(0);
counter.fetch_add(1, std::memory_order_relaxed);
```

## 编译器优化

### 常用编译标志

#### GCC/Clang

```bash
# 基础优化
-O2          # 推荐的优化级别
-O3          # 激进优化（可能增加代码大小）
-Os          # 优化代码大小
-Oz          # 更激进的大小优化

# 特定优化
-march=native     # 针对当前 CPU 架构优化
-ffast-math       # 快速数学运算（可能损失精度）
-flto             # 链接时优化
-fprofile-generate   # 生成 PGO 数据
-fprofile-use       # 使用 PGO 数据

# 调试
-g          # 生成调试信息
-pg         # 生成 gprof 数据
```

#### MSVC

```bash
/O1         # 优化代码大小
/O2         # 优化速度（推荐）
/Ox         # 最大优化
/arch:AVX2  # 启用 AVX2 指令集
/GL         # 全程序优化
/LTCG       # 链接时代码生成
```

## 性能分析工具

### Linux 平台

#### perf

```bash
# CPU 性能分析
perf stat ./program
perf record ./program
perf report

# 火焰图
perf record -F 99 -g ./program
perf script | stackcollapse-perf.pl | flamegraph.pl > flamegraph.svg
```

#### Valgrind/Memcheck

```bash
# 内存泄漏检测
valgrind --leak-check=full --show-leak-kinds=all ./program

# 缓存分析
valgrind --tool=cachegrind ./program
cg_annotate cachegrind.out.<pid>
```

#### gprof

```bash
g++ -pg program.cpp -o program
./program
gprof program gmon.out > analysis.txt
```

### Windows 平台

#### Visual Studio Profiler

- **CPU Usage**: 查看 CPU 使用情况
- **Memory Usage**: 内存分配分析
- **Performance Wizard**: 性能向导

#### Intel VTune

- 热点分析
- 微架构分析
- 并发分析

## 性能测量

### 时间测量

```cpp
// 高精度时钟
auto start = std::chrono::high_resolution_clock::now();
// ... 执行代码 ...
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<
    std::chrono::nanoseconds>(end - start).count();
std::cout << "耗时: " << duration << " ns" << std::endl;
```

### RDTSC (Read Time-Stamp Counter)

```cpp
inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

auto start = rdtsc();
// ... 执行代码 ...
auto cycles = rdtsc() - start;
```

## 常见性能陷阱

### 1. 过早优化

**问题**: 在找到瓶颈前就优化代码

**解决**: 使用 profiler 找到真正的热点

### 2. 过度使用虚函数

**问题**: 虚函数调用有开销

```cpp
// ❌ 内部循环使用虚函数
for (int i = 0; i < n; ++i) {
    sum += objects[i]->calculate();  // 虚函数调用
}

// ✅ 使用模板或策略模式
template<typename T>
int calculateSum(const std::vector<T>& objects) {
    int sum = 0;
    for (const auto& obj : objects) {
        sum += obj.calculate();  // 内联
    }
    return sum;
}
```

### 3. 不必要的拷贝

```cpp
// ❌ 多次拷贝
std::string result = process(getInput());

// ✅ 使用移动语义
std::string result = process(std::move(getInput()));
```

### 4. 频繁的小内存分配

```cpp
// ❌ 每次分配内存
for (int i = 0; i < n; ++i) {
    std::string s = "temp" + std::to_string(i);  // 多次分配
}

// ✅ 预分配或使用内存池
std::vector<std::string> strings;
strings.reserve(n);
```

## 性能优化检查清单

### 算法层面
- [ ] 是否使用了最优的时间复杂度算法？
- [ ] 是否有不必要的嵌套循环？
- [ ] 数据结构是否合适？

### 内存层面
- [ ] 是否有内存泄漏？
- [ ] 缓存命中率是否足够？
- [ ] 是否频繁分配/释放内存？

### 并发层面
- [ ] 是否有足够的并行度？
- [ ] 是否有锁竞争？
- [ ] 是否有假共享？

### 编译层面
- [ ] 是否启用了正确的优化级别？
- [ ] 是否使用了 LTO/PGO？
- [ ] 是否针对目标 CPU 架构优化？

## 推荐学习顺序

1. 学习使用性能分析工具（perf, Valgrind）
2. 理解 CPU 缓存和内存层次结构
3. 学习编译器优化标志
4. 掌握 SIMD 和向量化
5. 学习多线程和无锁编程

## 相关模块

- `03_memory_management` - 内存管理
- `06_multithreading` - 多线程编程
- `10_practical_projects` - 实战项目

## 参考资源

- 《C++ 性能优化指南》- Agner Fog
- 《高效程序员的 45 个习惯》
- cpp-optimization.readthedocs.io
- Intel 优化手册
- Linux `perf` 文档
