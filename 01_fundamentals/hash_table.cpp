#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <type_traits>
#include <functional>
using namespace std;

// 哈希表节点模板类
template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    
    // 构造函数
    HashNode(const K& k, const V& v) : key(k), value(v) {}
};

// 简单的哈希表实现（使用链地址法处理冲突）
template <typename K, typename V>
class HashTable {
private:
    // 哈希表数组，每个元素是一个链表
    vector<list<HashNode<K, V>>> table;
    int size;           // 哈希表大小
    int count;          // 元素数量
    
    // 哈希函数（使用标准库的hash函数）
    int hashFunction(const K& key) const {
        try {
            // 使用标准库的hash函数
            hash<K> hasher;
            size_t hashValue = hasher(key);
            return static_cast<int>(hashValue % static_cast<size_t>(size));
        } catch (...) {
            // 如果哈希计算失败，返回一个默认值
            return 0;
        }
    }
    
public:
    // 构造函数
    HashTable(int tableSize = 10) : size(tableSize), count(0) {
        table.resize(size);
    }
    
    // 析构函数
    ~HashTable() {
        clear();
    }
    
    // 插入键值对
    bool insert(const K& key, const V& value) {
        int index = hashFunction(key);
        
        // 检查键是否已存在
        for (auto& node : table[index]) {
            if (node.key == key) {
                node.value = value;  // 更新现有值
                return true;
            }
        }
        
        // 添加新节点
        table[index].emplace_back(key, value);
        count++;
        return true;
    }
    
    // 删除键值对
    bool remove(const K& key) {
        int index = hashFunction(key);
        
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                table[index].erase(it);
                count--;
                return true;
            }
        }
        
        return false;  // 键不存在
    }
    
    // 获取值
    bool get(const K& key, V& value) const {
        int index = hashFunction(key);
        
        for (const auto& node : table[index]) {
            if (node.key == key) {
                value = node.value;
                return true;
            }
        }
        
        return false;  // 键不存在
    }
    
    // 检查键是否存在
    bool contains(const K& key) const {
        int index = hashFunction(key);
        
        for (const auto& node : table[index]) {
            if (node.key == key) {
                return true;
            }
        }
        
        return false;
    }
    
    // 获取元素数量
    int getSize() const {
        return count;
    }
    
    // 检查哈希表是否为空
    bool isEmpty() const {
        return count == 0;
    }
    
    // 清空哈希表
    void clear() {
        for (int i = 0; i < size; ++i) {
            table[i].clear();
        }
        count = 0;
    }
    
    // 打印哈希表（简化版）
    void print() const {
        cout << "哈希表内容 (共" << count << "个元素):" << endl;
        for (int i = 0; i < size; ++i) {
            if (!table[i].empty()) {
                cout << "[" << i << "]: ";
                for (const auto& node : table[i]) {
                    cout << "{" << node.key << ": " << node.value << "} ";
                }
                cout << endl;
            }
        }
    }
    
    // 计算负载因子
    double getLoadFactor() const {
        return static_cast<double>(count) / size;
    }
};

// 整数哈希表演示
void intHashTableDemo() {
    cout << "\n=== 整数键哈希表演示 ===" << endl;
    
    HashTable<int, string> table(5);
    
    // 插入数据
    table.insert(1, "苹果");
    table.insert(6, "香蕉");  // 6 % 5 = 1，与1冲突
    table.insert(11, "橙子"); // 11 % 5 = 1，与1、6冲突
    table.insert(2, "葡萄");
    table.insert(7, "西瓜");  // 7 % 5 = 2，与2冲突
    
    cout << "插入了5个元素" << endl;
    
    // 简化的查找数据
    string value;
    table.get(1, value) && cout << "键1的值: " << value << endl;
    table.get(6, value) && cout << "键6的值: " << value << endl;
    table.get(11, value) && cout << "键11的值: " << value << endl;
    
    // 简化的键检查
    cout << "键2存在: " << (table.contains(2) ? "是" : "否") << endl;
    cout << "键99存在: " << (table.contains(99) ? "是" : "否") << endl;
    
    // 删除数据
    table.remove(6);
    cout << "删除键6后，当前负载因子: " << table.getLoadFactor() << endl;
}

// 字符串哈希表演示
void stringHashTableDemo() {
    cout << "\n=== 字符串键哈希表演示 ===" << endl;
    
    try {
        HashTable<string, int> scores(7);
        
        // 先只插入一个元素测试
        cout << "尝试插入第一个学生..." << endl;
        scores.insert("张三", 85);
        cout << "成功插入张三" << endl;
        
        // 再插入其他学生成绩
        scores.insert("李四", 92);
        scores.insert("王五", 78);
        scores.insert("赵六", 96);
        scores.insert("钱七", 89);
        scores.insert("孙八", 73);
        scores.insert("周九", 88);
        
        cout << "插入了7个学生成绩" << endl;
        
        // 简化的查找成绩
        int score;
        scores.get("张三", score) && cout << "张三的成绩: " << score << endl;
        scores.get("赵六", score) && cout << "赵六的成绩: " << score << endl;
        
        // 更新成绩
        scores.insert("李四", 95);
        scores.get("李四", score) && cout << "李四的新成绩: " << score << endl;
        
        // 删除学生
        scores.remove("孙八");
        cout << "删除孙八后，当前学生数: " << scores.getSize() << endl;
    } catch (const exception& e) {
        cout << "字符串哈希表演示出错: " << e.what() << endl;
    }
}

// 高级操作演示
void advancedHashTableDemo() {
    cout << "\n=== 哈希表高级操作演示 ===" << endl;
    
    HashTable<int, string> phoneBook(8);
    
    // 插入电话号码
    cout << "创建电话簿:" << endl;
    phoneBook.insert(1001, "张三 - 123-4567");
    phoneBook.insert(2002, "李四 - 234-5678");
    phoneBook.insert(3003, "王五 - 345-6789");
    phoneBook.insert(4004, "赵六 - 456-7890");
    phoneBook.insert(5005, "钱七 - 567-8901");
    phoneBook.insert(6006, "孙八 - 678-9012");
    
    cout << "电话簿创建完成，当前大小: " << phoneBook.getSize() << endl;
    phoneBook.print();
    
    // 展示冲突处理
    cout << "\n冲突处理演示:" << endl;
    phoneBook.insert(1009, "周九 - 789-0123");  // 1009 % 8 = 1，与1001冲突
    phoneBook.insert(2017, "吴十 - 890-1234");  // 2017 % 8 = 1，与1001、1009冲突
    
    cout << "添加冲突后的电话簿:" << endl;
    phoneBook.print();
    
    // 查找冲突中的元素
    cout << "\n查找冲突中的元素:" << endl;
    string info;
    if (phoneBook.get(1001, info)) {
        cout << "1001: " << info << endl;
    }
    
    if (phoneBook.get(1009, info)) {
        cout << "1009: " << info << endl;
    }
    
    if (phoneBook.get(2017, info)) {
        cout << "2017: " << info << endl;
    }
    
    // 性能指标
    cout << "\n哈希表性能指标:" << endl;
    cout << "元素数量: " << phoneBook.getSize() << endl;
    cout << "表大小: " << 8 << endl;
    cout << "负载因子: " << phoneBook.getLoadFactor() << endl;
}

int main() {
    cout << "C++ 哈希表基础实现演示" << endl;
    
    intHashTableDemo();
    stringHashTableDemo();
    advancedHashTableDemo();
    
    cout << "\n程序结束" << endl;
    return 0;
}