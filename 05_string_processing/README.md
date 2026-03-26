# 05_string_processing - 字符串处理

本模块学习 C++ 字符串处理技术和常见操作。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `string.cpp` | std::string 常用操作和处理技巧 | ⭐⭐ |

## 🎯 学习目标

- 熟练使用 std::string
- 掌握字符串分割、拼接、查找
- 了解正则表达式基础应用

## 📖 核心概念

### 常用字符串操作

```cpp
string s = "Hello, World!";

// 查找
s.find("World");          // 查找子串
s.rfind("l");             // 从右查找
s.find_first_of("aeiou"); // 查找任一字符

// 修改
s.substr(0, 5);           // 截取 "Hello"
s.replace(7, 5, "C++");   // 替换
s.erase(5, 2);            // 删除

// 转换
stoi("42");               // string -> int
to_string(3.14);          // 数值 -> string
```

### 字符串分割

```cpp
// 按分隔符分割
vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}
```

### 字符串视图 (C++17)

```cpp
// string_view：零拷贝的字符串引用
void print(string_view sv) {
    cout << sv << endl;
}

string s = "hello";
print(s);            // 从 string 构造
print("world");      // 从字符串字面量构造
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 string.cpp -o string && ./string
```

## ✅ 学习检查清单

- [ ] 熟练使用 std::string 的常用方法
- [ ] 能实现字符串分割和拼接
- [ ] 了解 string_view 的使用场景
- [ ] 掌握字符串和数值类型的转换

## 🔗 相关模块

- `04_template_metaprogramming` - 泛型字符串处理
- `07_algorithms` - 字符串相关算法

---

**字符串处理是日常开发中最常用的技能之一！** 🚀
