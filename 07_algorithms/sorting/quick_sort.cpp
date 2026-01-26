/**
 * 快速排序 (Quick Sort)
 *
 * 时间复杂度：
 * - 最坏：O(n²) (当枢轴选择最差时)
 * - 平均：O(n log n)
 * - 最优：O(n log n)
 *
 * 空间复杂度：O(log n) (递归栈空间)
 * 稳定性：不稳定
 */

#include <iostream>
#include <vector>
#include <random>

using namespace std;

// 分区函数：返回枢轴的最终位置
size_t partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];  // 选择最后一个元素作为枢轴
    size_t i = low;          // i 指向小于枢轴的区域的边界

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            i++;
        }
    }

    swap(arr[i], arr[high]);  // 将枢轴放到正确位置
    return i;
}

// 随机化分区（避免最坏情况）
size_t randomizedPartition(vector<int>& arr, int low, int high) {
    // 在 low 到 high 之间随机选择一个索引
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(low, high);

    int randomIndex = dis(gen);
    swap(arr[randomIndex], arr[high]);  // 将随机选中的元素与最后一个交换

    return partition(arr, low, high);
}

// 递归实现的快速排序
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // 获取枢轴位置
        size_t pi = randomizedPartition(arr, low, high);

        // 递归排序枢轴左侧和右侧
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 包装函数，方便调用
void quickSort(vector<int>& arr) {
    if (!arr.empty()) {
        quickSort(arr, 0, arr.size() - 1);
    }
}

// 三路分区快速排序（处理大量重复元素）
void quickSort3Way(vector<int>& arr, int low, int high) {
    if (low >= high) return;

    int pivot = arr[low];
    int lt = low;      // 小于枢轴的边界
    int gt = high;     // 大于枢轴的边界
    int i = low + 1;   // 当前元素

    while (i <= gt) {
        if (arr[i] < pivot) {
            swap(arr[lt], arr[i]);
            lt++;
            i++;
        } else if (arr[i] > pivot) {
            swap(arr[i], arr[gt]);
            gt--;
        } else {
            i++;
        }
    }

    // 递归排序小于和大于枢轴的部分
    quickSort3Way(arr, low, lt - 1);
    quickSort3Way(arr, gt + 1, high);
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

    cout << "\n=== 标准快速排序 ===" << endl;
    quickSort(arr1);
    cout << "排序后: ";
    printArray(arr1);

    cout << "\n=== 三路分区快速排序 ===" << endl;
    quickSort3Way(arr2, 0, arr2.size() - 1);
    cout << "排序后: ";
    printArray(arr2);

    // 测试包含大量重复元素的数组
    vector<int> arr3 = {5, 3, 5, 2, 5, 3, 5, 1, 5, 4};
    cout << "\n=== 测试大量重复元素 ===" << endl;
    cout << "原始数组: ";
    printArray(arr3);

    quickSort3Way(arr3, 0, arr3.size() - 1);
    cout << "排序后: ";
    printArray(arr3);

    return 0;
}
