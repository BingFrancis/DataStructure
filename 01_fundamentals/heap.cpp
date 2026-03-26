/**
 * 堆 (Heap) - 优先队列的基础
 *
 * 堆是一种完全二叉树，满足堆性质：
 * - 最大堆：每个节点的值都大于或等于其子节点的值
 * - 最小堆：每个节点的值都小于或等于其子节点的值
 *
 * 时间复杂度：
 * - push/insert: O(log n)
 * - pop: O(log n)
 * - top: O(1)
 * - buildHeap: O(n)
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;

// 最大堆实现
class MaxHeap {
private:
    vector<int> data;

    // 上浮
    void siftUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (data[index] <= data[parent]) {
                break;
            }
            swap(data[index], data[parent]);
            index = parent;
        }
    }

    // 下沉
    void siftDown(size_t index) {
        size_t size = data.size();
        while (index < size) {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t largest = index;

            if (left < size && data[left] > data[largest]) {
                largest = left;
            }
            if (right < size && data[right] > data[largest]) {
                largest = right;
            }

            if (largest == index) {
                break;
            }

            swap(data[index], data[largest]);
            index = largest;
        }
    }

public:
    // 插入元素
    void push(int value) {
        data.push_back(value);
        siftUp(data.size() - 1);
    }

    // 删除并返回最大元素
    int pop() {
        if (isEmpty()) {
            throw out_of_range("Heap is empty");
        }

        int maxValue = data[0];
        data[0] = data.back();
        data.pop_back();
        siftDown(0);

        return maxValue;
    }

    // 获取最大元素
    int top() const {
        if (isEmpty()) {
            throw out_of_range("Heap is empty");
        }
        return data[0];
    }

    // 检查是否为空
    bool isEmpty() const {
        return data.empty();
    }

    // 获取堆的大小
    size_t size() const {
        return data.size();
    }

    // 从数组构建堆（O(n)）
    void buildHeap(const vector<int>& arr) {
        data = arr;
        // 从最后一个非叶子节点开始下沉
        for (int i = (data.size() / 2) - 1; i >= 0; i--) {
            // 需要临时移除const来调用siftDown
            MaxHeap* nonConstThis = const_cast<MaxHeap*>(this);
            nonConstThis->siftDown(i);
        }
    }

    // 堆排序
    void heapSort() {
        vector<int> result;
        while (!isEmpty()) {
            result.push_back(pop());
        }
        reverse(result.begin(), result.end());
        data = result;
    }

    // 获取堆中的所有元素
    vector<int> getData() const {
        return data;
    }
};

// 最小堆实现
class MinHeap {
private:
    vector<int> data;

    void siftUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (data[index] >= data[parent]) {
                break;
            }
            swap(data[index], data[parent]);
            index = parent;
        }
    }

    void siftDown(size_t index) {
        size_t size = data.size();
        while (index < size) {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t smallest = index;

            if (left < size && data[left] < data[smallest]) {
                smallest = left;
            }
            if (right < size && data[right] < data[smallest]) {
                smallest = right;
            }

            if (smallest == index) {
                break;
            }

            swap(data[index], data[smallest]);
            index = smallest;
        }
    }

public:
    void push(int value) {
        data.push_back(value);
        siftUp(data.size() - 1);
    }

    int pop() {
        if (isEmpty()) {
            throw out_of_range("Heap is empty");
        }

        int minValue = data[0];
        data[0] = data.back();
        data.pop_back();
        siftDown(0);

        return minValue;
    }

    int top() const {
        if (isEmpty()) {
            throw out_of_range("Heap is empty");
        }
        return data[0];
    }

    bool isEmpty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};

// 示例：Top K 问题
vector<int> topKFrequent(vector<int>& nums, int k) {
    // 统计频率
    unordered_map<int, int> freq;
    for (int num : nums) {
        freq[num]++;
    }

    // 最小堆，保持k个元素
    auto cmp = [](pair<int, int>& a, pair<int, int>& b) {
        return a.second > b.second;  // 频率小的在堆顶
    };
    vector<pair<int, int>> heap;
    make_heap(heap.begin(), heap.end(), cmp);

    for (auto& [num, count] : freq) {
        if (heap.size() < k) {
            heap.push_back({num, count});
            push_heap(heap.begin(), heap.end(), cmp);
        } else if (count > heap[0].second) {
            pop_heap(heap.begin(), heap.end(), cmp);
            heap.pop_back();
            heap.push_back({num, count});
            push_heap(heap.begin(), heap.end(), cmp);
        }
    }

    vector<int> result;
    for (auto& p : heap) {
        result.push_back(p.first);
    }

    return result;
}

int main() {
    cout << "=== 最大堆基本操作 ===" << endl;
    MaxHeap maxHeap;

    maxHeap.push(10);
    maxHeap.push(30);
    maxHeap.push(20);
    maxHeap.push(40);
    maxHeap.push(50);

    cout << "堆大小: " << maxHeap.size() << endl;
    cout << "最大元素: " << maxHeap.top() << endl;

    while (!maxHeap.isEmpty()) {
        cout << "pop: " << maxHeap.pop() << endl;
    }

    cout << "\n=== 从数组构建堆 ===" << endl;
    vector<int> arr = {3, 1, 6, 5, 2, 4};
    cout << "原数组: ";
    for (int val : arr) {
        cout << val << " ";
    }
    cout << endl;

    MaxHeap heapFromArr;
    heapFromArr.buildHeap(arr);
    cout << "构建后的堆: ";
    for (int val : heapFromArr.getData()) {
        cout << val << " ";
    }
    cout << endl;

    cout << "\n=== 堆排序 ===" << endl;
    heapFromArr.heapSort();
    cout << "排序后: ";
    for (int val : heapFromArr.getData()) {
        cout << val << " ";
    }
    cout << endl;

    cout << "\n=== 最小堆 ===" << endl;
    MinHeap minHeap;
    minHeap.push(50);
    minHeap.push(30);
    minHeap.push(20);
    minHeap.push(40);
    minHeap.push(10);

    while (!minHeap.isEmpty()) {
        cout << "pop: " << minHeap.pop() << endl;
    }

    cout << "\n=== Top K 高频元素 (k=2) ===" << endl;
    vector<int> nums = {1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5};
    vector<int> topK = topKFrequent(nums, 2);
    cout << "Top " << 2 << " 高频元素: ";
    for (int val : topK) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
