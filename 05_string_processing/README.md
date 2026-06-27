# 05_string_processing - 字符串处理

本模块学习 C++ 标准库字符串处理技术，包括 std::string 操作、string_view 和正则表达式。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `string_operations.cpp` | std::string 实战（查找/替换/分割/正则/string_view） | ⭐⭐ |

## 🎯 学习目标

- 熟练使用 std::string 的所有常用操作
- 掌握字符串分割、拼接、查找、替换
- 理解 string_view 的使用场景（零拷贝）
- 学会正则表达式基础应用

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

### string_view (C++17) - 零拷贝

```cpp
void process(string_view sv) {
    // 不拷贝，直接引用原字符串
    cout << sv.substr(0, 5) << endl;
}

string s = "hello world";
process(s);            // 从 string
process("literal");    // 从字面量
```

### 正则表达式

```cpp
regex emailPattern(R"((\w+)(\.\w+)*@(\w+\.\w+))");
string text = "Contact: support@example.com";

smatch match;
if (regex_search(text, match, emailPattern)) {
    cout << match[0] << endl;  // 完整匹配
}
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 string_operations.cpp -o string_ops && ./string_ops
```

## ✅ 学习检查清单

- [ ] 熟练使用 std::string 的常用方法
- [ ] 能实现字符串分割和拼接
- [ ] 了解 string_view 的使用场景
- [ ] 掌握字符串和数值类型的转换
- [ ] 能使用正则表达式进行模式匹配

## 🔗 相关模块

- `04_template_metaprogramming` — 泛型字符串处理
- `07_algorithms` — 字符串匹配算法
- `08_advanced_cpp` — Rule of 5 中 MyString 的移动语义实战

---

**字符串处理是日常开发中最常用的技能之一！** 🚀
