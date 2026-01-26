/**
 * 队列 (Queue) - 基于双端队列的实现
 *
 * 队列是一种先进先出 (FIFO, First In First Out) 的数据结构
 *
 * 时间复杂度：
 * - push/enqueue: O(1)
 * - pop/dequeue: O(1)
 * - front: O(1)
 * - isEmpty: O(1)
 * - size: O(1)
 */

#include <iostream>
#include <deque>
#include <vector>
#include <stdexcept>
#include <unordered_map>

using namespace std;

template <typename T>
class Queue {
private:
    deque<T> data;

public:
    // 构造函数
    Queue() = default;

    // 入队
    void push(const T& value) {
        data.push_back(value);
    }

    // 出队
    void pop() {
        if (isEmpty()) {
            throw out_of_range("Queue is empty");
        }
        data.pop_front();
    }

    // 获取队首元素
    T& front() {
        if (isEmpty()) {
            throw out_of_range("Queue is empty");
        }
        return data.front();
    }

    // 获取队首元素（const版本）
    const T& front() const {
        if (isEmpty()) {
            throw out_of_range("Queue is empty");
        }
        return data.front();
    }

    // 获取队尾元素
    T& back() {
        if (isEmpty()) {
            throw out_of_range("Queue is empty");
        }
        return data.back();
    }

    // 检查队列是否为空
    bool isEmpty() const {
        return data.empty();
    }

    // 获取队列的大小
    size_t size() const {
        return data.size();
    }

    // 清空队列
    void clear() {
        data.clear();
    }
};

// 循环队列实现（固定大小）
template <typename T>
class CircularQueue {
private:
    vector<T> data;
    int head;
    int tail;
    size_t capacity;
    size_t count;

public:
    explicit CircularQueue(size_t size) : data(size), head(0), tail(0), capacity(size), count(0) {}

    bool push(const T& value) {
        if (isFull()) {
            return false;
        }
        data[tail] = value;
        tail = (tail + 1) % capacity;
        count++;
        return true;
    }

    bool pop() {
        if (isEmpty()) {
            return false;
        }
        head = (head + 1) % capacity;
        count--;
        return true;
    }

    T& front() {
        if (isEmpty()) {
            throw out_of_range("Queue is empty");
        }
        return data[head];
    }

    const T& front() const {
        if (isEmpty()) {
            throw out_of_range("Queue is empty");
        }
        return data[head];
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == capacity;
    }

    size_t size() const {
        return count;
    }
};

// 示例：用队列实现BFS（广度优先搜索）
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

vector<int> bfs(TreeNode* root) {
    vector<int> result;
    if (!root) return result;

    Queue<TreeNode*> q;
    q.push(root);

    while (!q.isEmpty()) {
        TreeNode* node = q.front();
        q.pop();

        result.push_back(node->val);

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }

    return result;
}

// 示例：约瑟夫问题
int josephus(int n, int k) {
    Queue<int> q;
    for (int i = 1; i <= n; i++) {
        q.push(i);
    }

    while (q.size() > 1) {
        // 移动k-1个人到队尾
        for (int i = 0; i < k - 1; i++) {
            q.push(q.front());
            q.pop();
        }
        // 第k个人出队
        q.pop();
    }

    return q.front();
}

int main() {
    cout << "=== 队列的基本操作 ===" << endl;
    Queue<int> queue;

    queue.push(1);
    queue.push(2);
    queue.push(3);

    cout << "队首元素: " << queue.front() << endl;
    cout << "队列大小: " << queue.size() << endl;

    queue.pop();
    cout << "pop 后的队首: " << queue.front() << endl;

    cout << "\n=== 循环队列示例 ===" << endl;
    CircularQueue<int> circularQueue(3);

    cout << "push 1: " << (circularQueue.push(1) ? "成功" : "失败") << endl;
    cout << "push 2: " << (circularQueue.push(2) ? "成功" : "失败") << endl;
    cout << "push 3: " << (circularQueue.push(3) ? "成功" : "失败") << endl;
    cout << "push 4: " << (circularQueue.push(4) ? "成功" : "失败") << " (已满)" << endl;

    cout << "front: " << circularQueue.front() << endl;
    circularQueue.pop();
    cout << "pop 后 front: " << circularQueue.front() << endl;

    cout << "\n=== 约瑟夫问题 (n=7, k=3) ===" << endl;
    cout << "幸存者: " << josephus(7, 3) << endl;

    cout << "\n=== BFS 遍历二叉树 ===" << endl;
    /*
         1
       /   \
      2     3
     / \   / \
    4   5 6   7
    */
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);

    vector<int> bfsResult = bfs(root);
    cout << "BFS 结果: ";
    for (int val : bfsResult) {
        cout << val << " ";
    }
    cout << endl;

    // 清理内存
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right->left;
    delete root->right->right;
    delete root->right;
    delete root;

    return 0;
}
