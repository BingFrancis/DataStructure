/**
 * 二分查找 (Binary Search)
 *
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1) (迭代版) / O(log n) (递归版)
 * 前置条件：数组必须有序
 */

#include <iostream>
#include <vector>

using namespace std;

// 迭代版二分查找
int binarySearchIterative(const vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        // 避免整数溢出
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;  // 找到目标
        } else if (arr[mid] < target) {
            left = mid + 1;  // 在右半部分继续查找
        } else {
            right = mid - 1;  // 在左半部分继续查找
        }
    }

    return -1;  // 未找到
}

// 递归版二分查找
int binarySearchRecursive(const vector<int>& arr, int left, int right, int target) {
    if (left > right) {
        return -1;  // 未找到
    }

    int mid = left + (right - left) / 2;

    if (arr[mid] == target) {
        return mid;  // 找到目标
    } else if (arr[mid] < target) {
        return binarySearchRecursive(arr, mid + 1, right, target);  // 查找右半部分
    } else {
        return binarySearchRecursive(arr, left, mid - 1, target);   // 查找左半部分
    }
}

// 查找第一个大于等于目标值的位置（lower_bound）
int lowerBound(const vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size();

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return left;
}

// 查找第一个大于目标值的位置（upper_bound）
int upperBound(const vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size();

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return left;
}

// 在旋转有序数组中查找
int searchRotated(const vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;
        }

        // 判断哪一半是有序的
        if (arr[left] <= arr[mid]) {  // 左半部分有序
            if (arr[left] <= target && target < arr[mid]) {
                right = mid - 1;  // 目标在左半部分
            } else {
                left = mid + 1;   // 目标在右半部分
            }
        } else {  // 右半部分有序
            if (arr[mid] < target && target <= arr[right]) {
                left = mid + 1;   // 目标在右半部分
            } else {
                right = mid - 1;  // 目标在左半部分
            }
        }
    }

    return -1;
}

// 打印数组
void printArray(const vector<int>& arr) {
    for (const int& val : arr) {
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    vector<int> rotated = {4, 5, 6, 7, 0, 1, 2};

    cout << "有序数组: ";
    printArray(arr);

    int target = 7;

    cout << "\n=== 迭代版二分查找 ===" << endl;
    int result = binarySearchIterative(arr, target);
    cout << "查找 " << target << ": "
         << (result != -1 ? to_string(result) : "未找到") << endl;

    cout << "\n=== 递归版二分查找 ===" << endl;
    result = binarySearchRecursive(arr, 0, arr.size() - 1, target);
    cout << "查找 " << target << ": "
         << (result != -1 ? to_string(result) : "未找到") << endl;

    cout << "\n=== Lower Bound 和 Upper Bound ===" << endl;
    cout << "lower_bound(5): " << lowerBound(arr, 5) << endl;
    cout << "upper_bound(5): " << upperBound(arr, 5) << endl;
    cout << "lower_bound(6): " << lowerBound(arr, 6) << endl;
    cout << "upper_bound(6): " << upperBound(arr, 6) << endl;

    cout << "\n=== 在旋转有序数组中查找 ===" << endl;
    cout << "旋转数组: ";
    printArray(rotated);
    cout << "查找 0: " << searchRotated(rotated, 0) << endl;
    cout << "查找 5: " << searchRotated(rotated, 5) << endl;
    cout << "查找 3: " << searchRotated(rotated, 3) << endl;

    return 0;
}
