#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// 示例类，用于演示智能指针的使用
class Resource {
private:
    string name;
public:
    Resource(const string& n) : name(n) {
        cout << "Resource " << name << " created" << endl;
    }
    
    ~Resource() {
        cout << "Resource " << name << " destroyed" << endl;
    }
    
    void doSomething() {
        cout << "Resource " << name << " is doing something" << endl;
    }
    
    string getName() const { return name; }
};

// 演示unique_ptr的使用
void unique_ptr_example() {
    cout << "\n=== unique_ptr 示例 ===" << endl;
    
    // 创建unique_ptr
    unique_ptr<Resource> ptr1(new Resource("Unique1"));
    ptr1->doSomething();
    
    // unique_ptr不能复制，但可以移动
    unique_ptr<Resource> ptr2 = move(ptr1);
    if (!ptr1) {
        cout << "ptr1 is empty after move" << endl;
    }
    ptr2->doSomething();
    
    // 自定义删除器
    auto deleter = [](Resource* r) {
        cout << "Custom deleter for " << r->getName() << endl;
        delete r;
    };
    
    unique_ptr<Resource, decltype(deleter)> ptr3(new Resource("Unique3"), deleter);
}

// 演示shared_ptr的使用
void shared_ptr_example() {
    cout << "\n=== shared_ptr 示例 ===" << endl;
    
    // 创建shared_ptr
    shared_ptr<Resource> ptr1 = make_shared<Resource>("Shared1");
    cout << "Reference count: " << ptr1.use_count() << endl;
    
    {
        shared_ptr<Resource> ptr2 = ptr1;
        cout << "Reference count after copy: " << ptr1.use_count() << endl;
        ptr2->doSomething();
    }
    
    cout << "Reference count after ptr2 out of scope: " << ptr1.use_count() << endl;
    
    // 使用shared_ptr在容器中
    vector<shared_ptr<Resource>> resources;
    resources.push_back(make_shared<Resource>("Vector1"));
    resources.push_back(make_shared<Resource>("Vector2"));
    resources.push_back(ptr1);
    
    for (const auto& res : resources) {
        res->doSomething();
    }
}

// 演示weak_ptr的使用，解决循环引用问题
class Node {
public:
    string name;
    shared_ptr<Node> next;
    weak_ptr<Node> prev;  // 使用weak_ptr避免循环引用
    
    Node(const string& n) : name(n) {
        cout << "Node " << name << " created" << endl;
    }
    
    ~Node() {
        cout << "Node " << name << " destroyed" << endl;
    }
};

void weak_ptr_example() {
    cout << "\n=== weak_ptr 示例 ===" << endl;
    
    shared_ptr<Node> node1 = make_shared<Node>("Node1");
    shared_ptr<Node> node2 = make_shared<Node>("Node2");
    
    node1->next = node2;
    node2->prev = node1;  // 使用weak_ptr，不会增加引用计数
    
    cout << "Node1 use count: " << node1.use_count() << endl;
    cout << "Node2 use count: " << node2.use_count() << endl;
    
    // 通过weak_ptr访问对象
    if (auto locked = node2->prev.lock()) {
        cout << "Previous node is: " << locked->name << endl;
    } else {
        cout << "Previous node has been destroyed" << endl;
    }
}

int main() {
    cout << "C++ 智能指针练习" << endl;
    
    unique_ptr_example();
    shared_ptr_example();
    weak_ptr_example();
    
    cout << "\n程序结束" << endl;
    return 0;
}