# 07 - Algorithms（算法）

本模块涵盖经典算法的实现与分析。

## 目录结构

```
07_algorithms/
├── sorting/                 # 排序算法
│   ├── bubble_sort.cpp      # 冒泡排序
│   ├── selection_sort.cpp   # 选择排序
│   ├── insertion_sort.cpp   # 插入排序
│   ├── merge_sort.cpp       # 归并排序
│   ├── quick_sort.cpp       # 快速排序
│   └── heap_sort.cpp        # 堆排序
├── searching/               # 搜索算法
│   ├── binary_search.cpp    # 二分查找
│   ├── linear_search.cpp    # 线性查找
│   └── interpolation_search.cpp # 插值查找
├── dynamic_programming/     # 动态规划
│   ├── fibonacci.cpp        # 斐波那契数列
│   ├── knapsack.cpp         # 背包问题
│   └── longest_common_subsequence.cpp # 最长公共子序列
└── README.md                # 本文件
```

## 学习目标

- 掌握常见排序算法的实现原理和时间复杂度
- 理解搜索算法的适用场景
- 学习动态规划思想
- 理解算法的空间和时间复杂度分析

## 快速开始

### 编译示例

```bash
g++ -std=c++17 sorting/quick_sort.cpp -o quick_sort.exe
./quick_sort.exe
```

## 核心概念

### 1. 排序算法复杂度对比

| 算法 | 最坏时间 | 平均时间 | 最优时间 | 空间复杂度 | 稳定性 |
|------|---------|---------|---------|-----------|--------|
| 冒泡排序 | O(n²) | O(n²) | O(n) | O(1) | 稳定 |
| 选择排序 | O(n²) | O(n²) | O(n²) | O(1) | 不稳定 |
| 插入排序 | O(n²) | O(n²) | O(n) | O(1) | 稳定 |
| 归并排序 | O(n log n) | O(n log n) | O(n log n) | O(n) | 稳定 |
| 快速排序 | O(n²) | O(n log n) | O(n log n) | O(log n) | 不稳定 |
| 堆排序 | O(n log n) | O(n log n) | O(n log n) | O(1) | 不稳定 |

### 2. 搜索算法对比

| 算法 | 时间复杂度 | 前置条件 | 适用场景 |
|------|-----------|---------|---------|
| 线性查找 | O(n) | 无 | 小规模或无序数据 |
| 二分查找 | O(log n) | 有序数组 | 大规模有序数据 |
| 插值查找 | O(log log n) | 均匀分布有序数组 | 均匀分布数据 |

### 3. 动态规划核心思想

动态规划通过将复杂问题分解为更简单的子问题来求解。

**关键要素：**
- **最优子结构**：问题的最优解包含子问题的最优解
- **重叠子问题**：相同子问题被重复计算
- **状态转移方程**：描述子问题之间的关系
- **边界条件**：基础情况的解

**典型模式：**
```cpp
// 1. 定义状态 dp[i] 表示什么
// 2. 状态转移方程
dp[i] = dp[i-1] + dp[i-2];  // 例如斐波那契数列

// 3. 初始化
dp[0] = 0;
dp[1] = 1;

// 4. 计算顺序（从前往后）
for (int i = 2; i <= n; i++) {
    dp[i] = dp[i-1] + dp[i-2];
}
```

## 推荐学习顺序

1. **排序算法**：从简单的冒泡排序开始，逐步学习更复杂的算法
2. **搜索算法**：理解线性查找后，学习二分查找
3. **动态规划**：从斐波那契数列入手，理解DP思想

## 相关模块

- `01_fundamentals` - 基础数据结构（数组、链表等）
- `03_memory_management` - 内存管理（对理解算法空间复杂度有帮助）
- `04_template_metaprogramming` - 模板编程（可用于泛型算法实现）

## 参考资源

- 《算法导论》(Introduction to Algorithms)
- LeetCode - 算法练习平台
- 赫芬达尔不等式与排序算法下界分析
