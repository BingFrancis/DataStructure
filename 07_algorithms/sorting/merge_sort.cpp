/**
 * 归并排序 (Merge Sort)
 *
 * 时间复杂度：
 * - 最坏：O(n log n)
 * - 平均：O(n log n)
 * - 最优：O(n log n)
 *
 * 空间复杂度：O(n) (需要额外空间)
 * 稳定性：稳定
 */

#include <iostream>
#include <vector>

using namespace std;

// 合并两个有序数组
void merge(vector<int>& arr, int left, int mid, int right) {
    // 计算两个子数组的大小
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    // 创建临时数组
    vector<int> L(n1);
    vector<int> R(n2);

    // 复制数据到临时数组
    for (size_t i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (size_t j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    // 合并临时数组回原数组
    size_t i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {  // <= 保持稳定性
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // 复制剩余元素（如果有）
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// 递归实现的归并排序
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        // 计算中间位置
        int mid = left + (right - left) / 2;

        // 递归排序左右两半
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // 合并已排序的两半
        merge(arr, left, mid, right);
    }
}

// 包装函数
void mergeSort(vector<int>& arr) {
    if (!arr.empty()) {
        mergeSort(arr, 0, arr.size() - 1);
    }
}

// 自底向上的归并排序（非递归实现）
void mergeSortBottomUp(vector<int>& arr) {
    size_t n = arr.size();

    // 从大小为1的子数组开始，逐步合并
    for (size_t size = 1; size < n; size *= 2) {
        for (size_t left = 0; left < n - size; left += 2 * size) {
            size_t mid = left + size - 1;
            size_t right = min(left + 2 * size - 1, n - 1);

            merge(arr, left, mid, right);
        }
    }
}

// 打印数组
void printArray(const vector<int>& arr) {
    for (const int& val : arr) {
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    vector<int> arr1 = {64, 34, 25, 12, 22, 11, 90, 45, 33, 77};
    vector<int> arr2 = {64, 34, 25, 12, 22, 11, 90, 45, 33, 77};

    cout << "原始数组: ";
    printArray(arr1);

    cout << "\n=== 递归归并排序 ===" << endl;
    mergeSort(arr1);
    cout << "排序后: ";
    printArray(arr1);

    cout << "\n=== 自底向上归并排序 ===" << endl;
    mergeSortBottomUp(arr2);
    cout << "排序后: ";
    printArray(arr2);

    return 0;
}
