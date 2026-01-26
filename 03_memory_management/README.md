# 03 - 内存管理

## 📚 模块概述

本模块深入探讨 C++ 内存管理的核心技术，包括内存布局、自定义分配器、内存池和对象池等。这些技术在高性能计算、游戏开发、实时系统等领域非常重要。

---

## 📁 文件列表

| 文件名 | 描述 | 学习时长 |
|--------|------|----------|
| `memory_layout.cpp` | C++ 内存布局分析 | 2 小时 |
| `memory_pool.cpp` | 内存池实现 | 2-3 小时 |
| `smart_allocator.cpp` | 高级分配器实现 | 3-4 小时 |
| `object_pool_demo.cpp` | 对象池实现 | 2-3 小时 |
| `object.h` | 对象池接口定义 | - |

---

## 🎯 学习目标

- 理解 C++ 程序的内存布局（栈、堆、代码段、数据段）
- 掌握自定义内存分配器的设计与实现
- 学会实现高效的内存池和对象池
- 理解内存对齐和缓存友好性
- 掌握 STL 容器自定义分配器的使用

---

## 📖 学习顺序建议

### 第一步：内存布局 (memory_layout.cpp)

**核心概念：**

```
高地址
    ↓
    ┌─────────────────┐
    │  命令行参数和环境变量 │
    ├─────────────────┤
    │      栈 (Stack)    │  ↓ 增长
    ├─────────────────┤
    │      (空闲区)      │
    ├─────────────────┤
    │      堆 (Heap)    │  ↑ 增长
    ├─────────────────┤
    │   未初始化数据段 (BSS)│
    ├─────────────────┤
    │   已初始化数据段 (Data)│
    ├─────────────────┤
    │     代码段 (Text)   │
    └─────────────────┘
低地址
```

**重点内容：**
- 栈和堆的区别
- 静态存储期、自动存储期、动态存储期
- 内存对齐对性能的影响
- 虚函数表的内存布局

**编译运行：**
```bash
g++ -std=c++17 memory_layout.cpp -o memory_layout && ./memory_layout
```

---

### 第二步：内存池 (memory_pool.cpp)

**核心思想：**
- 预先分配一大块内存
- 避免频繁调用 new/delete
- 减少内存碎片
- 提高内存分配性能

**应用场景：**
- 游戏引擎（每帧创建销毁大量对象）
- 高频交易系统
- 实时渲染系统
- 数据库系统

**⚠️ 重要注意事项 - 指针转换问题**

在内存池实现中，最常见且危险的错误是指针转换问题：

**问题代码示例：**
```cpp
struct Block {
    Block* next;
    alignas(T) char data[sizeof(T)];
};

// 分配时
T* allocate() {
    Block* block = /* ... */;
    return reinterpret_cast<T*>(block->data);  // 返回 data 的地址
}

// 释放时（错误！）
void deallocate(T* ptr) {
    Block* block = reinterpret_cast<Block*>(ptr);  // ❌ 错误！
    // 问题：ptr 指向 data，而不是 Block 的起始地址
}
```

**错误原因：**
- `ptr` 指向的是 `block->data` 的位置
- `Block` 结构体包含 `next` 指针（8 字节）和 `data` 数组
- 两者偏移了 8 字节，直接强制转换会导致地址错误
- 释放时访问错误的内存地址，导致 `free(): invalid pointer` 错误

**正确解决方案：**

使用映射表保存指针关系：

```cpp
std::unordered_map<T*, Block*> ptr_to_block;  // 保存映射

T* allocate() {
    Block* block = /* ... */;
    T* ptr = reinterpret_cast<T*>(block->data);
    ptr_to_block[ptr] = block;  // 保存映射
    return ptr;
}

void deallocate(T* ptr) {
    auto it = ptr_to_block.find(ptr);
    if (it != ptr_to_block.end()) {
        Block* block = it->second;  // 通过映射找到正确的 Block
        free_blocks.push_back(block);
        ptr_to_block.erase(it);
    }
}
```

**其他技巧：**
- 也可以在 `data` 数组前额外存储 Block 指针
- 或使用偏移量计算（但更复杂）
- 使用智能指针管理 Block 的生命周期，避免手动 `delete[]`

**总结：**
在进行指针转换时，必须清楚指针指向的内存布局，避免盲目强制转换导致地址错误。使用映射表是一种安全可靠的方法。

---

**⚠️ 性能测试注意事项**

内存池的性能优势不在单次分配速度，而在于：
1. **减少内存碎片**：频繁 new/delete 会产生大量碎片
2. **缓存友好性**：连续内存布局提高缓存命中率
3. **减少系统调用**：一次 `new Block[]` 胜过多次 `new Particle`
4. **避免内存耗尽**：预分配保证内存可用性

在简单的性能测试中（如 1000 次分配），内存池可能因映射表查找开销而表现不佳。但在真实场景中（游戏引擎每帧创建/销毁数万个对象），其优势才真正体现。

如果性能测试显示内存池较慢，这是正常的。可以通过以下方式优化：
- 减少映射表查找（使用地址对齐计算）
- 使用更高效的数据结构
- 根据场景调整块大小和预分配策略

**编译运行：**
```bash
g++ -std=c++17 memory_pool.cpp -o memory_pool && ./memory_pool
```

---

### 第三步：智能分配器 (smart_allocator.cpp)

本文件包含 **5 种高级分配器**：

#### 1. **Arena Allocator（线性分配器）**
- 最简单的分配器
- 只分配不释放
- 适合短生命周期的对象
- 性能最佳

**应用：** 游戏帧数据、临时缓冲区

---

#### 2. **Stack Allocator（栈分配器）**
- LIFO（后进先出）语义
- 支持释放操作
- 必须按相反顺序释放
- 性能接近 Arena

**应用：** 作用域内的临时对象

---

#### 3. **FreeList Allocator（自由链表分配器）**
- 维护空闲块链表
- 支持任意顺序的分配和释放
- 需要处理碎片化问题
- 实现复杂度较高

**应用：** 通用内存管理、STL 自定义分配器

---

#### 4. **ArenaAllocatorWrapper（STL 自定义分配器）**
- 将 Arena Allocator 包装为 STL 兼容的分配器
- 展示如何为 STL 容器使用自定义分配器
- 包含类型重绑定 (rebind) 技术

**应用：**
```cpp
std::vector<int, ArenaAllocatorWrapper<int>> vec;
```

---

#### 5. **Proxy Allocator（代理分配器）**
- 根据分配大小选择不同的分配器
- 大对象使用 FreeList，小对象使用 Stack
- 综合性能优化
- 展示分配器组合的高级用法

**应用：** 复杂系统的内存管理策略

---

**编译运行：**
```bash
g++ -std=c++17 -O2 smart_allocator.cpp -o smart_allocator && ./smart_allocator
```

---

### 第四步：对象池 (object_pool_demo.cpp + object.h)

**核心思想：**
- 预先创建一组对象
- 对象不销毁，而是回收到池中
- 避免频繁构造/析构
- 提高性能，减少碎片

**应用场景：**
- 游戏中的子弹、敌人、粒子效果
- 数据库连接池
- 线程池
- 网络连接管理

**关键设计：**
```cpp
template<typename T>
class ObjectPool {
public:
    std::shared_ptr<T> acquire();  // 获取对象
    void release(std::shared_ptr<T> obj);  // 回收对象
};
```

**编译运行：**
```bash
g++ -std=c++17 object_pool_demo.cpp -o object_pool && ./object_pool
```

---

## 🔗 知识图谱

```
内存管理
├── 内存布局
│   ├── 栈 (Stack)
│   ├── 堆 (Heap)
│   ├── 代码段 (Text)
│   └── 数据段 (Data/BSS)
├── 内存池
│   ├── 线性分配器
│   └── 栈分配器
├── 自定义分配器
│   ├── Arena Allocator
│   ├── Stack Allocator
│   ├── FreeList Allocator
│   ├── Proxy Allocator
│   └── STL 兼容分配器
└── 对象池
    ├── 预分配策略
    ├── 回收机制
    └── 智能指针集成
```

---

## 💡 最佳实践

### ✅ 何时使用自定义内存管理

1. **性能关键代码**：
   - 游戏引擎
   - 高频交易
   - 实时系统

2. **大量临时对象**：
   - 每帧创建/销毁的对象
   - 短生命周期的临时数据

3. **减少碎片化**：
   - 长时间运行的服务器
   - 嵌入式系统

### ❌ 何时不需要

- 一般应用程序（使用智能指针即可）
- 内存需求不稳定的场景
- 开发初期（先优化，再重构）

---

## 🔧 性能对比

| 分配方式 | 速度 | 碎片化 | 复杂度 | 适用场景 |
|---------|------|--------|--------|----------|
| new/delete | 慢 | 高 | 低 | 通用 |
| Arena | 最快 | 无 | 低 | 短生命周期 |
| Stack | 快 | 无 | 低 | LIFO 语义 |
| FreeList | 中 | 中 | 高 | 通用 |
| 对象池 | 快 | 无 | 中 | 固定类型对象 |

---

## 📚 推荐资源

- **书籍**：《Game Engine Architecture》- Jason Gregory
- **论文**："Dynamic Storage Allocation: A Survey and Critical Review"
- **开源项目**：Facebook's folly (内存管理组件)、Tencent's RapidJSON

---

## ✅ 学习检查清单

- [ ] 理解 C++ 内存布局的各个段
- [ ] 掌握内存池的基本原理和实现
- [ ] 理解 5 种不同分配器的设计思想
- [ ] 能够为 STL 容器编写自定义分配器
- [ ] 掌握对象池的设计和使用
- [ ] 理解内存对齐和缓存友好性
- [ ] 能够根据场景选择合适的内存管理策略

---

## 🚀 下一步

完成本模块后，建议进入 **04_template_metaprogramming** 模块，学习 C++ 模板编程和元编程技术。
