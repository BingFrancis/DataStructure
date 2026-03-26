# 01_fundamentals - 基础数据结构

本模块涵盖 C++ 基础数据结构和面向对象编程核心概念。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `linked_list.cpp` | 链表（单向、双向）实现 | ⭐⭐ |
| `binary_tree.cpp` | 二叉树（遍历、操作） | ⭐⭐⭐ |
| `hash_table.cpp` | 哈希表（冲突处理） | ⭐⭐⭐ |
| `stack.cpp` | 栈（LIFO） | ⭐⭐ |
| `queue.cpp` | 队列（FIFO） | ⭐⭐ |
| `heap.cpp` | 堆（优先队列） | ⭐⭐⭐ |
| `inheritance.cpp` | 继承机制全面学习 | ⭐⭐⭐ |
| `inheritance_guide.md` | 继承学习指南 | 参考 |

## 🎯 学习目标

- 掌握基本数据结构的实现原理
- 理解指针和动态内存分配
- 熟练使用 STL 容器
- **掌握 C++ 继承和多态机制**
- **理解面向对象编程的核心概念**

## 📖 学习步骤

### 第一步：线性结构

```bash
# 1. 链表
g++ -std=c++17 linked_list.cpp -o linked_list && ./linked_list

# 2. 栈
g++ -std=c++17 stack.cpp -o stack && ./stack

# 3. 队列
g++ -std=c++17 queue.cpp -o queue && ./queue
```

**重点：**
- 指针操作
- 动态内存分配
- 插入、删除、遍历操作

### 第二步：树形结构

```bash
# 二叉树
g++ -std=c++17 binary_tree.cpp -o binary_tree && ./binary_tree
```

**重点：**
- 递归思想
- 前序、中序、后序遍历
- 搜索树概念

### 第三步：哈希结构

```bash
# 哈希表
g++ -std=c++17 hash_table.cpp -o hash_table && ./hash_table
```

**重点：**
- 哈希函数设计
- 冲突处理（链地址法、开放寻址法）
- 负载因子和扩容

### 第四步：堆（优先队列） ⭐

```bash
g++ -std=c++17 heap.cpp -o heap && ./heap
```

**重点：**
- 完全二叉树
- 堆化操作（上浮、下沉）
- 优先队列应用

### 第五步：继承和多态 ⭐⭐⭐ 重要

```bash
g++ -std=c++17 inheritance.cpp -o inheritance && ./inheritance
```

**重点内容：**
- public/protected/private 继承
- 虚函数和动态绑定
- 纯虚函数和抽象类
- 多重继承和虚继承
- override 和 final 关键字
- 虚析构函数（必须！）

**关键知识点：**

```cpp
// 虚函数：支持多态
class Animal {
public:
    virtual void makeSound() {}  // 虚函数
    virtual ~Animal() {}        // 虚析构函数（必须！）
};

// 纯虚函数：定义接口
class Shape {
public:
    virtual double area() const = 0;  // 纯虚函数
};

// 继承和重写
class Dog : public Animal {
public:
    void makeSound() override {  // override 关键字
        cout << "汪汪叫" << endl;
    }
};
```

**详细学习：** 参考 `inheritance_guide.md`

## 🔗 知识图谱

```
基础数据结构
├── 线性结构
│   ├── 链表 (linked_list.cpp)
│   ├── 栈 (stack.cpp)
│   └── 队列 (queue.cpp)
├── 树形结构
│   └── 二叉树 (binary_tree.cpp)
├── 哈希结构
│   └── 哈希表 (hash_table.cpp)
├── 堆结构
│   └── 堆 (heap.cpp)
└── 面向对象基础
    └── 继承机制 (inheritance.cpp) ⭐
```

## ✅ 学习检查清单

### 数据结构
- [ ] 能熟练实现链表的基本操作
- [ ] 掌握二叉树的各种遍历方式
- [ ] 理解哈希表的原理和实现
- [ ] 理解栈的 LIFO 特性和应用场景
- [ ] 理解队列的 FIFO 特性和应用场景
- [ ] 理解堆的原理和优先队列应用

### 面向对象
- [ ] 掌握继承的三种方式（public/protected/private）
- [ ] 理解虚函数和多态机制
- [ ] 知道什么时候使用虚析构函数
- [ ] 理解纯虚函数和抽象类
- [ ] 了解多重继承和菱形继承问题
- [ ] 能正确使用 override 和 final 关键字

## 💡 常见误区

### 1. 忘记虚析构函数

```cpp
// ❌ 错误
class Base {
public:
    ~Base() {}  // 不是虚函数
};

// ✅ 正确
class Base {
public:
    virtual ~Base() {}  // 必须是虚函数
};
```

### 2. 多重继承不使用虚继承

```cpp
// ❌ 错误：菱形继承问题
class A {};
class B : public A {};
class C : public A {};
class D : public B, public C {};  // 两个 A 基类

// ✅ 正确：使用虚继承
class A {};
class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {};  // 只有一个 A 基类
```

## 📊 难度评估

| 知识点 | 难度 | 重要度 | 学习时长 |
|--------|------|--------|----------|
| 链表 | ⭐⭐ | ⭐⭐⭐ | 1-2h |
| 二叉树 | ⭐⭐⭐ | ⭐⭐⭐ | 2-3h |
| 哈希表 | ⭐⭐⭐ | ⭐⭐⭐⭐ | 2-3h |
| 栈/队列 | ⭐⭐ | ⭐⭐⭐ | 1-2h |
| 堆 | ⭐⭐⭐ | ⭐⭐⭐ | 2-3h |
| 继承和多态 | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | 3-4h |

**总学习时间：** 11-17 小时

## 🎓 相关模块

- `02_smart_pointers` - 智能指针（管理对象生命周期）
- `09_design_patterns` - 设计模式（基于继承和多态）

---

**掌握基础是学习进阶的关键！加油！** 🚀
