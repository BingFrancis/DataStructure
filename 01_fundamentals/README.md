# 01 - 基础数据结构与 C++ 基础

## 📚 模块概述

本模块涵盖了数据结构的核心基础和 C++ 语言的基本特性。这是深入学习 C++ 高级特性的前提，建议按照以下顺序学习。

---

## 📁 文件列表

| 文件名 | 描述 | 学习时长 |
|--------|------|----------|
| `linked_list.cpp` | 链表的实现与操作 | 1-2 小时 |
| `binary_tree.cpp` | 二叉树的遍历与操作 | 2-3 小时 |
| `hash_table.cpp` | 哈希表实现与冲突处理 | 2-3 小时 |
| `stack.cpp` | 栈的实现与应用 | 1-2 小时 |
| `queue.cpp` | 队列的实现与应用 | 1-2 小时 |
| `heap.cpp` | 堆（优先队列）实现 | 2-3 小时 |

---

## 🎯 学习目标

- 掌握基本数据结构的实现原理
- 理解指针操作和动态内存分配
- 学会使用 STL 容器
- 理解算法复杂度（时间/空间复杂度）

---

## 📖 学习顺序建议

### 第一步：链表 (linked_list.cpp)
**重点内容：**
- 单向链表和双向链表
- 节点的插入、删除、查找
- 链表的逆序和合并
- 链表的中间节点查找（快慢指针）

**关键知识点：**
```cpp
struct Node {
    int data;
    Node* next;
};
```

**编译运行：**
```bash
g++ -std=c++17 linked_list.cpp -o linked_list && ./linked_list
```

---

### 第二步：二叉树 (binary_tree.cpp)
**重点内容：**
- 二叉树的基本结构定义
- 前序、中序、后序遍历（递归与迭代）
- 层序遍历（BFS）
- 二叉搜索树的操作
- 树的深度和节点统计

**关键知识点：**
```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};
```

**编译运行：**
```bash
g++ -std=c++17 binary_tree.cpp -o binary_tree && ./binary_tree
```

---

### 第三步：哈希表 (hash_table.cpp)
**重点内容：**
- 哈希函数的设计
- 冲突解决方法（链地址法、开放寻址法）
- 哈希表的增删查操作
- 负载因子和扩容策略

**关键知识点：**
```cpp
// 使用STL的unordered_map
std::unordered_map<std::string, int> hashTable;
```

**编译运行：**
```bash
g++ -std=c++17 hash_table.cpp -o hash_table && ./hash_table
```

---

## 🔗 知识图谱

```
基础数据结构
├── 线性结构
│   ├── 链表 (linked_list.cpp)
│   └── 栈/队列
├── 树形结构
│   ├── 二叉树 (binary_tree.cpp)
│   └── 搜索树
└── 哈希结构
    └── 哈希表 (hash_table.cpp)
```

---

## 💡 学习建议

1. **动手实践**：先理解原理，再自己实现一遍
2. **画图理解**：数据结构可视化有助于理解
3. **复杂度分析**：养成分析时间/空间复杂度的习惯
4. **STL对比**：学会使用 STL 容器，如 `std::list`, `std::vector`, `std::unordered_map`

---

## 📚 推荐资源

- **书籍**：《算法（第4版）》- Robert Sedgewick
- **在线练习**：LeetCode、洛谷
- **可视化工具**：VisualAlgo

---

## ✅ 学习检查清单

- [ ] 能熟练实现链表的基本操作
- [ ] 掌握二叉树的各种遍历方式
- [ ] 理解哈希表的原理和实现
- [ ] 理解栈的 LIFO 特性和应用场景
- [ ] 理解队列的 FIFO 特性和应用场景
- [ ] 理解堆的原理和优先队列应用
- [ ] 能够分析算法的时间复杂度
- [ ] 熟练使用 STL 相关容器

---

## 🚀 下一步

完成本模块后，建议进入 **02_smart_pointers** 模块，学习现代 C++ 的智能指针技术。
