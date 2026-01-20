#include <iostream>
#include <queue>
using namespace std;

// 二叉树节点定义
struct TreeNode {
    int data;           // 节点数据
    TreeNode* left;     // 左子节点
    TreeNode* right;    // 右子节点
    
    // 构造函数
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// 二叉树类
class BinaryTree {
private:
    TreeNode* root;     // 根节点
    
    // 递归释放节点内存（内部辅助函数）
    void destroyTree(TreeNode* node) {
        if (node == nullptr) return;
        
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
    
    // 递归前序遍历（内部辅助函数）
    void preorderTraversal(TreeNode* node) const {
        if (node == nullptr) return;
        
        cout << node->data << " ";
        preorderTraversal(node->left);
        preorderTraversal(node->right);
    }
    
    // 递归中序遍历（内部辅助函数）
    void inorderTraversal(TreeNode* node) const {
        if (node == nullptr) return;
        
        inorderTraversal(node->left);
        cout << node->data << " ";
        inorderTraversal(node->right);
    }
    
    // 递归后序遍历（内部辅助函数）
    void postorderTraversal(TreeNode* node) const {
        if (node == nullptr) return;
        
        postorderTraversal(node->left);
        postorderTraversal(node->right);
        cout << node->data << " ";
    }
    
    // 递归查找节点（内部辅助函数）
    TreeNode* search(TreeNode* node, int value) const {
        if (node == nullptr || node->data == value) {
            return node;
        }
        
        TreeNode* leftResult = search(node->left, value);
        if (leftResult != nullptr) {
            return leftResult;
        }
        
        return search(node->right, value);
    }
    
    // 递归计算树的高度（内部辅助函数）
    int height(TreeNode* node) const {
        if (node == nullptr) {
            return 0;
        }
        
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        
        return max(leftHeight, rightHeight) + 1;
    }
    
    // 递归计算节点数量（内部辅助函数）
    int countNodes(TreeNode* node) const {
        if (node == nullptr) {
            return 0;
        }
        
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
    
public:
    // 构造函数
    BinaryTree() : root(nullptr) {}
    
    // 析构函数
    ~BinaryTree() {
        destroyTree(root);
    }
    
    // 设置根节点
    void setRoot(int value) {
        if (root != nullptr) {
            destroyTree(root);
        }
        root = new TreeNode(value);
    }
    
    // 添加左子节点
    bool addLeftChild(int parentValue, int childValue) {
        TreeNode* parent = search(root, parentValue);
        if (parent == nullptr || parent->left != nullptr) {
            return false;
        }
        
        parent->left = new TreeNode(childValue);
        return true;
    }
    
    // 添加右子节点
    bool addRightChild(int parentValue, int childValue) {
        TreeNode* parent = search(root, parentValue);
        if (parent == nullptr || parent->right != nullptr) {
            return false;
        }
        
        parent->right = new TreeNode(childValue);
        return true;
    }
    
    // 前序遍历（根-左-右）
    void preorder() const {
        cout << "前序遍历: ";
        preorderTraversal(root);
        cout << endl;
    }
    
    // 中序遍历（左-根-右）
    void inorder() const {
        cout << "中序遍历: ";
        inorderTraversal(root);
        cout << endl;
    }
    
    // 后序遍历（左-右-根）
    void postorder() const {
        cout << "后序遍历: ";
        postorderTraversal(root);
        cout << endl;
    }
    
    // 层序遍历（使用队列）
    void levelOrder() const {
        cout << "层序遍历: ";
        
        if (root == nullptr) {
            cout << "树为空" << endl;
            return;
        }
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* current = q.front();
            q.pop();
            
            cout << current->data << " ";
            
            if (current->left != nullptr) {
                q.push(current->left);
            }
            
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
        
        cout << endl;
    }
    
    // 查找节点
    bool contains(int value) const {
        return search(root, value) != nullptr;
    }
    
    // 计算树的高度
    int getHeight() const {
        return height(root);
    }
    
    // 计算节点数量
    int getNodeCount() const {
        return countNodes(root);
    }
    
    // 检查树是否为空
    bool isEmpty() const {
        return root == nullptr;
    }
};

// 演示函数
void basicTreeDemo() {
    cout << "\n=== 基本二叉树操作演示 ===" << endl;
    
    BinaryTree tree;
    
    // 创建一个简单的二叉树
    cout << "创建二叉树:" << endl;
    tree.setRoot(1);
    tree.addLeftChild(1, 2);
    tree.addRightChild(1, 3);
    tree.addLeftChild(2, 4);
    tree.addRightChild(2, 5);
    tree.addLeftChild(3, 6);
    tree.addRightChild(3, 7);
    
    // 树的结构如下：
    //       1
    //     /   \
    //    2     3
    //   / \   / \
    //  4   5 6   7
    
    cout << "二叉树创建完成" << endl;
    
    // 不同遍历方式
    cout << "\n遍历结果:" << endl;
    tree.preorder();     // 前序遍历
    tree.inorder();      // 中序遍历
    tree.postorder();    // 后序遍历
    tree.levelOrder();   // 层序遍历
    
    // 查找节点
    cout << "\n查找节点:" << endl;
    cout << "查找节点4: " << (tree.contains(4) ? "存在" : "不存在") << endl;
    cout << "查找节点9: " << (tree.contains(9) ? "存在" : "不存在") << endl;
    
    // 树的基本属性
    cout << "\n树的基本属性:" << endl;
    cout << "树的高度: " << tree.getHeight() << endl;
    cout << "节点数量: " << tree.getNodeCount() << endl;
    cout << "树是否为空: " << (tree.isEmpty() ? "是" : "否") << endl;
}

// 更复杂的树操作演示
void advancedTreeDemo() {
    cout << "\n=== 复杂二叉树演示 ===" << endl;
    
    BinaryTree tree;
    
    // 创建一个更复杂的树
    tree.setRoot(10);
    tree.addLeftChild(10, 5);
    tree.addRightChild(10, 15);
    tree.addLeftChild(5, 3);
    tree.addRightChild(5, 7);
    tree.addLeftChild(15, 12);
    tree.addRightChild(15, 18);
    tree.addLeftChild(3, 1);
    tree.addRightChild(18, 20);
    
    // 树的结构如下：
    //          10
    //        /    \
    //       5      15
    //     /  \    /  \
    //    3    7  12   18
    //   /              \
    //  1                20
    
    cout << "复杂二叉树创建完成" << endl;
    
    // 不同遍历方式
    cout << "\n遍历结果:" << endl;
    tree.preorder();     // 前序遍历
    tree.inorder();      // 中序遍历（对于BST，这会得到有序序列）
    tree.postorder();    // 后序遍历
    tree.levelOrder();   // 层序遍历
    
    // 树的基本属性
    cout << "\n树的基本属性:" << endl;
    cout << "树的高度: " << tree.getHeight() << endl;
    cout << "节点数量: " << tree.getNodeCount() << endl;
}

int main() {
    cout << "C++ 二叉树基础实现演示" << endl;
    
    basicTreeDemo();
    advancedTreeDemo();
    
    cout << "\n程序结束" << endl;
    return 0;
}