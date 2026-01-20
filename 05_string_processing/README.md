# 05 - 字符串处理

## 📚 模块概述

字符串处理是编程中的常见任务。本模块涵盖 C++ 字符串的各种处理技术，包括 C 风格字符串、std::string、字符串转换、格式化和高级操作。掌握这些技巧能让你更高效地处理文本数据。

---

## 📁 文件列表

| 文件名 | 描述 | 学习时长 |
|--------|------|----------|
| `string.cpp` | 字符串处理综合示例 | 2-3 小时 |

---

## 🎯 学习目标

- 理解 C 风格字符串和 std::string 的区别
- 掌握 std::string 的常用操作
- 学会字符串与数值类型的转换
- 掌握字符串查找、替换、分割等高级操作
- 了解字符串视图 (std::string_view)
- 掌握正则表达式在字符串处理中的应用

---

## 📖 学习内容

### 1. 字符串类型对比

#### C 风格字符串 (C-Style Strings)
```cpp
const char* cstr = "Hello";          // 字符串字面量
char arr[] = "World";                // 字符数组

// 特点：
// - 以 '\0' 结尾
// - 不自动管理内存
// - 容易出现缓冲区溢出
// - 性能高，但安全性差
```

#### std::string
```cpp
#include <string>

std::string str1 = "Hello";          // 字符串字面量构造
std::string str2(5, 'a');           // 5 个 'a'
std::string str3(str1);              // 拷贝构造

// 特点：
// - 自动管理内存
// - 动态扩容
// - 安全性高
// - 提供丰富的成员函数
```

#### std::string_view (C++17)
```cpp
#include <string_view>

std::string_view sv = "Hello";       // 不拥有内存，只视图

// 特点：
// - 零拷贝字符串访问
// - 不分配内存
// - 适合作为函数参数
// - 必须注意生命周期
```

---

### 2. std::string 常用操作

#### 基本操作
```cpp
std::string s = "Hello, World!";

// 访问字符
char c1 = s[0];      // 'H'
char c2 = s.at(0);   // 'H'（带边界检查）

// 获取信息
size_t len = s.length();    // 13
size_t size = s.size();     // 13（同 length）
bool empty = s.empty();     // false
size_t capacity = s.capacity(); // 容量

// 修改
s.append(" Welcome");
s.push_back('!');
s.insert(0, ">>> ");

s.erase(0, 3);  // 删除前 3 个字符
s.clear();      // 清空
```

#### 查找操作
```cpp
std::string s = "Hello, World!";

size_t pos1 = s.find("World");        // 7
size_t pos2 = s.find("xyz");         // std::string::npos
size_t pos3 = s.rfind('l');          // 10（从右查找）
size_t pos4 = s.find_first_of("abc");// 2（'l'）
size_t pos5 = s.find_last_of('!');   // 12
```

#### 子串操作
```cpp
std::string s = "Hello, World!";

std::string sub = s.substr(7, 5);    // "World"
std::string sub2 = s.substr(7);      // "World!"
```

#### 比较操作
```cpp
std::string s1 = "abc";
std::string s2 = "def";

bool eq = (s1 == s2);          // false
bool ne = (s1 != s2);          // true
bool lt = (s1 < s2);           // true
int cmp = s1.compare(s2);      // -1（小于）
```

---

### 3. 字符串转换

#### 字符串转数值
```cpp
#include <string>
#include <sstream>
#include <cstdlib>

// 方法 1：std::stoi, std::stof, etc.
int i = std::stoi("42");           // 42
double d = std::stod("3.14");      // 3.14
float f = std::stof("2.5f");       // 2.5

// 方法 2：stringstream
std::stringstream ss;
ss << "123";
int num;
ss >> num;  // num = 123

// 方法 3：sscanf (C 风格)
int value;
sscanf("456", "%d", &value);  // value = 456
```

#### 数值转字符串
```cpp
// 方法 1：std::to_string
std::string s1 = std::to_string(42);       // "42"
std::string s2 = std::to_string(3.14);    // "3.140000"

// 方法 2：stringstream
std::stringstream ss;
ss << 123;
std::string s3 = ss.str();  // "123"

// 方法 3：sprintf (C 风格)
char buffer[100];
sprintf(buffer, "%.2f", 3.14159);  // "3.14"
```

#### 编码转换
```cpp
#include <locale>
#include <codecvt>

// UTF-8 转宽字符（C++11，已弃用但仍可用）
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
std::wstring wide = converter.from_bytes("Hello");
std::string utf8 = converter.to_bytes(wide);

// 现代 C++ 推荐使用第三方库，如 ICU
```

---

### 4. 高级操作

#### 字符串分割
```cpp
#include <vector>
#include <sstream>

std::string s = "one,two,three";
std::stringstream ss(s);
std::string item;
std::vector<std::string> tokens;

while (std::getline(ss, item, ',')) {
    tokens.push_back(item);
}
// tokens = ["one", "two", "three"]
```

#### 字符串替换
```cpp
std::string s = "Hello, World!";

size_t pos = s.find("World");
if (pos != std::string::npos) {
    s.replace(pos, 5, "C++");  // "Hello, C++!"
}
```

#### 去除空白字符
```cpp
#include <algorithm>
#include <cctype>

std::string s = "  Hello  ";

// 去除首尾空白
s.erase(0, s.find_first_not_of(" \t\n\r"));
s.erase(s.find_last_not_of(" \t\n\r") + 1);
// s = "Hello"

// 使用自定义函数
std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}
```

#### 大小写转换
```cpp
#include <algorithm>
#include <cctype>

std::string s = "Hello";

// 转大写
std::transform(s.begin(), s.end(), s.begin(), ::toupper);  // "HELLO"

// 转小写
std::transform(s.begin(), s.end(), s.begin(), ::tolower);  // "hello"
```

---

### 5. 正则表达式

#### 基本匹配
```cpp
#include <regex>

std::string text = "Email: user@example.com";
std::regex pattern(R"(\b[\w.-]+@[\w.-]+\.\w+\b)");

if (std::regex_search(text, pattern)) {
    std::cout << "Found email!\n";
}
```

#### 捕获分组
```cpp
std::string text = "2023-12-25";
std::regex pattern(R"((\d{4})-(\d{2})-(\d{2}))");
std::smatch matches;

if (std::regex_match(text, matches, pattern)) {
    std::cout << "Year: " << matches[1] << '\n';    // 2023
    std::cout << "Month: " << matches[2] << '\n';   // 12
    std::cout << "Day: " << matches[3] << '\n';     // 25
}
```

#### 查找所有匹配
```cpp
std::string text = "apple, banana, apple, cherry";
std::regex pattern("apple");

std::sregex_iterator it(text.begin(), text.end(), pattern);
std::sregex_iterator end;

for (; it != end; ++it) {
    std::cout << "Found at position " << it->position() << '\n';
}
```

#### 替换
```cpp
std::string text = "Hello, World!";
std::regex pattern("World");
std::string result = std::regex_replace(text, pattern, "C++");
// result = "Hello, C++!"
```

---

### 6. 性能优化

#### 使用 string_view 避免拷贝
```cpp
// 避免
void process(std::string s) { /* ... */ }  // 拷贝

// 推荐
void process(std::string_view sv) { /* ... */ }  // 零拷贝
```

#### 预分配容量
```cpp
std::string s;
s.reserve(1000);  // 预分配，避免多次扩容
```

#### 使用 emplace_back
```cpp
std::vector<std::string> vec;
vec.emplace_back("Hello");  // 直接构造，不拷贝
```

---

## 🔗 知识图谱

```
字符串处理
├── 字符串类型
│   ├── C 风格字符串 (char*)
│   ├── std::string
│   └── std::string_view (C++17)
├── 基本操作
│   ├── 访问与修改
│   ├── 查找与子串
│   └── 比较与连接
├── 类型转换
│   ├── 字符串转数值
│   ├── 数值转字符串
│   └── 编码转换
├── 高级操作
│   ├── 分割与合并
│   ├── 替换与删除
│   ├── 去除空白
│   └── 大小写转换
└── 正则表达式
    ├── 匹配与查找
    ├── 捕获分组
    └── 替换操作
```

---

## 💡 最佳实践

### ✅ 推荐做法

1. **优先使用 std::string**：
   ```cpp
   std::string s = "Hello";  // 自动管理内存
   ```

2. **使用 string_view 作为函数参数**：
   ```cpp
   void print(std::string_view sv);  // 避免不必要的拷贝
   ```

3. **使用 to_string 和 stoi 等函数**：
   ```cpp
   std::string s = std::to_string(42);
   int i = std::stoi(s);
   ```

4. **预分配大字符串的容量**：
   ```cpp
   s.reserve(expected_size);
   ```

### ❌ 避免的做法

1. **混淆 C 风格和 C++ 风格**：
   ```cpp
   // 危险
   char* cstr = s.c_str();
   strcpy(cstr, "new");  // 未定义行为
   ```

2. **过度拷贝字符串**：
   ```cpp
   // 低效
   std::string s1 = "Hello";
   std::string s2 = s1;  // 不必要的拷贝
   process(s1);
   ```

3. **忽略生命周期**：
   ```cpp
   // 危险
   std::string_view sv = get_temp_string();  // 临时对象已销毁
   ```

---

## 🔧 编译运行

```bash
g++ -std=c++17 string.cpp -o string && ./string
```

---

## 📚 推荐资源

- **文档**：[cppreference - std::string](https://en.cppreference.com/w/cpp/string/basic_string)
- **书籍**：《C++ Primer》第 5 版 - 字符串章节
- **书籍**：《Effective C++》- Scott Meyers
- **工具**：Regex101（在线正则表达式测试）

---

## ✅ 学习检查清单

- [ ] 理解 C 风格字符串和 std::string 的区别
- [ ] 掌握 std::string 的常用操作
- [ ] 掌握字符串与数值的相互转换
- [ ] 理解 std::string_view 的使用场景
- [ ] 掌握字符串的查找、替换、分割等高级操作
- [ ] 掌握正则表达式的基本使用
- [ ] 了解字符串处理的性能优化技巧

---

## 🚀 下一步

完成本模块后，建议进入 **06_multithreading** 模块，学习多线程编程技术，包括线程、互斥锁、条件变量、线程池等。
