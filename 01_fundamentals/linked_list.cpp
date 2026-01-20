#include <iostream>
using namespace std;

// 链表节点定义
struct Node {
    int data;       // 数据域
    Node* next;     // 指针域，指向下一个节点
    
    // 构造函数
    Node(int val) : data(val), next(nullptr) {}
};

// 链表类
class LinkedList {
private:
    Node* head;     // 头指针
    int size;       // 链表大小
    
public:
    // 构造函数
    LinkedList() : head(nullptr), size(0) {}
    
    // 析构函数
    ~LinkedList() {
        clear();
    }
    
    // 在链表头部插入节点
    void insertAtHead(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        size++;
    }
    
    // 在链表尾部插入节点
    void insertAtTail(int value) {
        Node* newNode = new Node(value);
        
        // 如果链表为空，新节点就是头节点
        if (head == nullptr) {
            head = newNode;
        } else {
            // 找到尾节点
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }
    
    // 在指定位置插入节点
    bool insertAt(int index, int value) {
        // 检查索引是否有效
        if (index < 0 || index > size) {
            return false;
        }
        
        if (index == 0) {
            insertAtHead(value);
            return true;
        }
        
        if (index == size) {
            insertAtTail(value);
            return true;
        }
        
        // 找到插入位置的前一个节点
        Node* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        
        Node* newNode = new Node(value);
        newNode->next = current->next;
        current->next = newNode;
        size++;
        
        return true;
    }
    
    // 删除头节点
    bool deleteHead() {
        if (head == nullptr) {
            return false;
        }
        
        Node* temp = head;
        head = head->next;
        delete temp;
        size--;
        
        return true;
    }
    
    // 删除尾节点
    bool deleteTail() {
        if (head == nullptr) {
            return false;
        }
        
        // 如果只有一个节点
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            size--;
            return true;
        }
        
        // 找到倒数第二个节点
        Node* current = head;
        while (current->next->next != nullptr) {
            current = current->next;
        }
        
        delete current->next;
        current->next = nullptr;
        size--;
        
        return true;
    }
    
    // 删除指定位置的节点
    bool deleteAt(int index) {
        // 检查索引是否有效
        if (index < 0 || index >= size) {
            return false;
        }
        
        if (index == 0) {
            return deleteHead();
        }
        
        // 找到要删除节点的前一个节点
        Node* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        
        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
        size--;
        
        return true;
    }
    
    // 查找元素是否存在
    bool contains(int value) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // 获取链表大小
    int getSize() const {
        return size;
    }
    
    // 检查链表是否为空
    bool isEmpty() const {
        return size == 0;
    }
    
    // 清空链表
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
    }
    
    // 打印链表
    void print() const {
        cout << "链表内容: ";
        Node* current = head;
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << endl;
    }
};

// 演示函数
void basicLinkedListDemo() {
    cout << "\n=== 基本链表操作演示 ===" << endl;
    
    LinkedList list;
    
    // 在头部插入
    cout << "在头部插入 10, 20, 30:" << endl;
    list.insertAtHead(10);
    list.insertAtHead(20);
    list.insertAtHead(30);
    list.print();  // 应该输出: 30 -> 20 -> 10
    
    // 在尾部插入
    cout << "\n在尾部插入 5, 15:" << endl;
    list.insertAtTail(5);
    list.insertAtTail(15);
    list.print();  // 应该输出: 30 -> 20 -> 10 -> 5 -> 15
    
    // 在指定位置插入
    cout << "\n在位置2插入 25:" << endl;
    list.insertAt(2, 25);
    list.print();  // 应该输出: 30 -> 20 -> 25 -> 10 -> 5 -> 15
    
    // 删除头节点
    cout << "\n删除头节点:" << endl;
    list.deleteHead();
    list.print();  // 应该输出: 20 -> 25 -> 10 -> 5 -> 15
    
    // 删除尾节点
    cout << "\n删除尾节点:" << endl;
    list.deleteTail();
    list.print();  // 应该输出: 20 -> 25 -> 10 -> 5
    
    // 删除指定位置节点
    cout << "\n删除位置1的节点:" << endl;
    list.deleteAt(1);
    list.print();  // 应该输出: 20 -> 10 -> 5
    
    // 查找元素
    cout << "\n查找元素10: " << (list.contains(10) ? "存在" : "不存在") << endl;
    cout << "查找元素99: " << (list.contains(99) ? "存在" : "不存在") << endl;
    
    // 链表大小
    cout << "\n当前链表大小: " << list.getSize() << endl;
}

int main() {
    cout << "C++ 链表基础实现演示" << endl;
    
    basicLinkedListDemo();
    
    cout << "\n程序结束" << endl;
    return 0;
}