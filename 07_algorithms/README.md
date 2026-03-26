# 07_algorithms - 算法

本模块学习常用排序、查找和动态规划算法。

## 📁 文件列表

```
07_algorithms/
├── sorting/                    # 排序算法
│   ├── bubble_sort.cpp        # 冒泡排序
│   ├── merge_sort.cpp         # 归并排序
│   └── quick_sort.cpp         # 快速排序
├── searching/                  # 查找算法
│   └── binary_search.cpp      # 二分查找
└── dynamic_programming/        # 动态规划
    └── fibonacci.cpp          # 斐波那契数列
```

## 🎯 学习目标

- 掌握常用排序算法的原理和实现
- 理解算法时间复杂度分析
- 学会二分查找的思想
- 入门动态规划

## 📖 排序算法对比

| 算法 | 平均时间 | 最坏时间 | 空间 | 稳定 | 文件 |
|------|----------|----------|------|------|------|
| 冒泡排序 | O(n²) | O(n²) | O(1) | ✅ | `bubble_sort.cpp` |
| 快速排序 | O(n log n) | O(n²) | O(log n) | ❌ | `quick_sort.cpp` |
| 归并排序 | O(n log n) | O(n log n) | O(n) | ✅ | `merge_sort.cpp` |

## 📖 核心概念

### 1. 冒泡排序

```cpp
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}
```

### 2. 快速排序

```cpp
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
```

### 3. 二分查找

```cpp
int binarySearch(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
```

### 4. 动态规划（斐波那契）

```cpp
int fibonacci(int n) {
    vector<int> dp(n + 1);
    dp[0] = 0; dp[1] = 1;
    for (int i = 2; i <= n; i++)
        dp[i] = dp[i - 1] + dp[i - 2];
    return dp[n];
}
```

## 🛠️ 编译运行

```bash
cd sorting && g++ -std=c++17 bubble_sort.cpp -o bubble_sort && ./bubble_sort
cd sorting && g++ -std=c++17 quick_sort.cpp -o quick_sort && ./quick_sort
cd searching && g++ -std=c++17 binary_search.cpp -o binary_search && ./binary_search
cd dynamic_programming && g++ -std=c++17 fibonacci.cpp -o fibonacci && ./fibonacci
```

## ✅ 学习检查清单

- [ ] 能实现冒泡排序并分析复杂度
- [ ] 能实现快速排序并理解分区过程
- [ ] 能实现归并排序并理解分治思想
- [ ] 能实现二分查找
- [ ] 理解动态规划的基本思想
- [ ] 能分析算法的时间/空间复杂度

## 🔗 相关模块

- `01_fundamentals` - 基础数据结构
- `06_multithreading` - 并行排序算法

---

**算法 = 程序的灵魂！** 🚀
