/**
 * 冒泡排序 (Bubble Sort)
 *
 * 时间复杂度：
 * - 最坏：O(n²)
 * - 平均：O(n²)
 * - 最优：O(n) (当数组已经有序时)
 *
 * 空间复杂度：O(1)
 * 稳定性：稳定
 */

#include <iostream>
#include <vector>

using namespace std;

// 基础版冒泡排序
void bubbleSort(vector<int>& arr) {
    size_t n = arr.size();

    for (size_t i = 0; i < n - 1; i++) {
        // 每次将最大的元素"冒泡"到最后
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 优化版：添加early exit标志
void bubbleSortOptimized(vector<int>& arr) {
    size_t n = arr.size();
    bool swapped;

    for (size_t i = 0; i < n - 1; i++) {
        swapped = false;

        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // 如果这一轮没有发生交换，说明数组已经有序
        if (!swapped) {
            break;
        }
    }
}

// 进一步优化：记录最后一次交换的位置
void bubbleSortAdvanced(vector<int>& arr) {
    size_t n = arr.size();
    size_t lastSwapPos = n - 1;

    for (size_t i = 0; i < n - 1; i++) {
        bool swapped = false;
        size_t currentSwapPos = 0;

        for (size_t j = 0; j < lastSwapPos; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
                currentSwapPos = j;
            }
        }

        lastSwapPos = currentSwapPos;

        if (!swapped) {
            break;
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
    vector<int> arr1 = {64, 34, 25, 12, 22, 11, 90};
    vector<int> arr2 = {64, 34, 25, 12, 22, 11, 90};
    vector<int> arr3 = {64, 34, 25, 12, 22, 11, 90};

    cout << "原始数组: ";
    printArray(arr1);

    cout << "\n=== 基础版冒泡排序 ===" << endl;
    bubbleSort(arr1);
    cout << "排序后: ";
    printArray(arr1);

    cout << "\n=== 优化版冒泡排序 ===" << endl;
    bubbleSortOptimized(arr2);
    cout << "排序后: ";
    printArray(arr2);

    cout << "\n=== 高级版冒泡排序 ===" << endl;
    bubbleSortAdvanced(arr3);
    cout << "排序后: ";
    printArray(arr3);

    // 测试已排序数组（最优情况）
    vector<int> sortedArr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "\n=== 测试最优情况（已排序数组）===" << endl;
    cout << "原始数组: ";
    printArray(sortedArr);

    bubbleSortOptimized(sortedArr);
    cout << "排序后: ";
    printArray(sortedArr);

    return 0;
}
