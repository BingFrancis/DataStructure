// ============================================================================
// 05 - String Processing: std::string 操作实战
// ============================================================================
// 1. 构造与访问
// 2. 查找与替换
// 3. 子串与拼接
// 4. 分割与合并
// 5. 数值转换
// 6. string_view (C++17)
// 7. 正则表达式
// ============================================================================

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>

// ============================================================================
// 1. 构造与访问
// ============================================================================
void test_construction() {
    std::cout << "=== 1. 构造与访问 ===" << std::endl;
    
    std::string s1 = "hello";
    std::string s2("world");
    std::string s3(5, 'x');      // "xxxxx"
    std::string s4(s1);          // 拷贝
    
    std::cout << "s1: " << s1 << std::endl;
    std::cout << "s2: " << s2 << std::endl;
    std::cout << "s3: " << s3 << std::endl;
    
    // 访问
    std::cout << "s1[0]: " << s1[0] << std::endl;
    std::cout << "s1.front(): " << s1.front() << std::endl;
    std::cout << "s1.back(): " << s1.back() << std::endl;
    std::cout << "s1.length(): " << s1.length() << std::endl;
    std::cout << "s1.empty(): " << std::boolalpha << s1.empty() << std::endl;
}

// ============================================================================
// 2. 查找与替换
// ============================================================================
void test_find_replace() {
    std::cout << "\n=== 2. 查找与替换 ===" << std::endl;
    
    std::string s = "Hello, World! Hello, C++!";
    
    // 查找
    size_t pos = s.find("World");
    std::cout << "find 'World': " << pos << std::endl;
    
    pos = s.rfind("Hello");  // 从右查找
    std::cout << "rfind 'Hello': " << pos << std::endl;
    
    pos = s.find_first_of("aeiou");  // 查找第一个元音
    std::cout << "第一个元音在位置: " << pos << " ('" << s[pos] << "')" << std::endl;
    
    pos = s.find_last_of("aeiou");
    std::cout << "最后一个元音在位置: " << pos << " ('" << s[pos] << "')" << std::endl;
    
    pos = s.find_first_not_of("Helo, Wrd!");  // 第一个不在集合中的字符
    if (pos != std::string::npos) {
        std::cout << "第一个不在集合中的字符: '" << s[pos] << "'" << std::endl;
    }
    
    // 替换
    std::string replaced = s;
    replaced.replace(7, 5, "C++");  // 从位置7开始，替换5个字符
    std::cout << "替换后: " << replaced << std::endl;
}

// ============================================================================
// 3. 子串与拼接
// ============================================================================
void test_substr_concat() {
    std::cout << "\n=== 3. 子串与拼接 ===" << std::endl;
    
    std::string s = "Hello, World!";
    
    // 子串
    std::cout << "s.substr(0, 5): " << s.substr(0, 5) << std::endl;
    std::cout << "s.substr(7): " << s.substr(7) << std::endl;
    
    // 拼接
    std::string result = s + " How are you?";
    result += " I'm fine.";
    std::cout << "拼接结果: " << result << std::endl;
    
    // 插入
    result.insert(5, " dear");
    std::cout << "插入后: " << result << std::endl;
    
    // 删除
    result.erase(5, 5);  // 删除 " dear"
    std::cout << "删除后: " << result << std::endl;
    
    // append
    std::string prefix = ">>> ";
    std::string suffix = " <<<";
    std::cout << "append: " << prefix.append(s).append(suffix) << std::endl;
}

// ============================================================================
// 4. 分割与合并
// ============================================================================
std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) tokens.push_back(item);
    }
    return tokens;
}

std::string join(const std::vector<std::string>& parts, const std::string& delim) {
    if (parts.empty()) return "";
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += delim + parts[i];
    }
    return result;
}

void test_split_join() {
    std::cout << "\n=== 4. 分割与合并 ===" << std::endl;
    
    std::string csv = "apple,banana,orange,grape";
    auto parts = split(csv, ',');
    
    std::cout << "分割 '" << csv << "':" << std::endl;
    for (size_t i = 0; i < parts.size(); ++i) {
        std::cout << "  [" << i << "] " << parts[i] << std::endl;
    }
    
    std::string merged = join(parts, " | ");
    std::cout << "合并: " << merged << std::endl;
    
    // 路径分割
    std::string path = "/usr/local/bin/program";
    auto pathParts = split(path, '/');
    std::cout << "路径分割: " << join(pathParts, " -> ") << std::endl;
}

// ============================================================================
// 5. 数值转换
// ============================================================================
void test_conversion() {
    std::cout << "\n=== 5. 数值转换 ===" << std::endl;
    
    // string -> number
    int i = std::stoi("42");
    long l = std::stol("123456789");
    double d = std::stod("3.14159");
    float f = std::stof("2.718");
    
    std::cout << "stoi('42'): " << i << std::endl;
    std::cout << "stol('123456789'): " << l << std::endl;
    std::cout << "stod('3.14159'): " << d << std::endl;
    std::cout << "stof('2.718'): " << f << std::endl;
    
    // number -> string
    std::cout << "to_string(42): " << std::to_string(42) << std::endl;
    std::cout << "to_string(3.14): " << std::to_string(3.14) << std::endl;
    
    // 十六进制/八进制
    std::string hex = "0xFF";
    size_t hexVal = std::stoul(hex, nullptr, 16);
    std::cout << "stoul('0xFF', 16): " << hexVal << std::endl;
}

// ============================================================================
// 6. string_view (C++17) - 零拷贝的字符串视图
// ============================================================================
void process(std::string_view sv) {
    std::cout << "string_view 处理: " << sv 
              << " (长度: " << sv.length() << ")" << std::endl;
}

void test_string_view() {
    std::cout << "\n=== 6. string_view (C++17) ===" << std::endl;
    
    std::string s = "Hello, World!";
    const char* cstr = "Hello from C string";
    
    // string_view 可以从多种类型构造
    std::string_view sv1 = s;              // 从 string
    std::string_view sv2 = cstr;           // 从 C 字符串
    std::string_view sv3 = "literal";      // 从字面量
    
    std::cout << "sv1: " << sv1 << std::endl;
    std::cout << "sv2: " << sv2 << std::endl;
    std::cout << "sv3: " << sv3 << std::endl;
    
    // 子视图（零拷贝）
    std::string_view sub = sv1.substr(0, 5);
    std::cout << "substr(0,5): " << sub << std::endl;
    
    // 查找
    size_t pos = sv1.find("World");
    std::cout << "find 'World' in sv1: " << pos << std::endl;
    
    // 前后缀检查 (C++20: starts_with / ends_with)
    process("Hello from main");
    process(s);
    process(cstr);
}

// ============================================================================
// 7. 正则表达式
// ============================================================================
void test_regex() {
    std::cout << "\n=== 7. 正则表达式 ===" << std::endl;
    
    // 匹配
    std::regex emailPattern(R"((\w+)(\.\w+)*@(\w+\.\w+))");
    std::string text = "Contact us at support@example.com or sales@company.org";
    
    std::smatch match;
    if (std::regex_search(text, match, emailPattern)) {
        std::cout << "找到邮箱: " << match[0] << std::endl;
        std::cout << "  用户名: " << match[1] << std::endl;
        std::cout << "  域名: " << match[3] << std::endl;
    }
    
    // 查找所有匹配
    std::cout << "所有邮箱:" << std::endl;
    auto begin = std::sregex_iterator(text.begin(), text.end(), emailPattern);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        std::cout << "  " << (*it)[0] << std::endl;
    }
    
    // 替换
    std::string replaced = std::regex_replace(text, emailPattern, "[邮箱已隐藏]");
    std::cout << "替换后: " << replaced << std::endl;
    
    // 验证格式
    std::regex phonePattern(R"(\d{3}-\d{4}-\d{4})");
    std::string phone = "010-1234-5678";
    std::cout << "手机号 '" << phone << "' 格式" 
              << (std::regex_match(phone, phonePattern) ? "正确" : "错误") << std::endl;
}

// ============================================================================
// 8. 大小写转换与修剪
// ============================================================================
void test_case_trim() {
    std::cout << "\n=== 8. 大小写与修剪 ===" << std::endl;
    
    std::string s = "  Hello, World!  ";
    std::cout << "原始: '" << s << "'" << std::endl;
    
    // 修剪空白 (trim)
    auto ltrim = [](std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    };
    auto rtrim = [](std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    };
    
    ltrim(s); rtrim(s);
    std::cout << "修剪后: '" << s << "'" << std::endl;
    
    // 大小写转换
    std::string upper = s;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    std::cout << "大写: " << upper << std::endl;
    
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    std::cout << "小写: " << lower << std::endl;
}

int main() {
    test_construction();
    test_find_replace();
    test_substr_concat();
    test_split_join();
    test_conversion();
    test_string_view();
    test_regex();
    test_case_trim();
    
    std::cout << "\n所有字符串操作演示完成!" << std::endl;
    return 0;
}
