/**
 * 栈 (Stack) - 基于动态数组的实现
 *
 * 栈是一种后进先出 (LIFO, Last In First Out) 的数据结构
 *
 * 时间复杂度：
 * - push: O(1) 均摊
 * - pop: O(1)
 * - top: O(1)
 * - isEmpty: O(1)
 * - size: O(1)
 */

#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename T>
class Stack {
private:
    vector<T> data;

public:
    // 构造函数
    Stack() = default;

    // 压入元素到栈顶
    void push(const T& value) {
        data.push_back(value);
    }

    // 弹出栈顶元素
    void pop() {
        if (isEmpty()) {
            throw out_of_range("Stack is empty");
        }
        data.pop_back();
    }

    // 获取栈顶元素
    T& top() {
        if (isEmpty()) {
            throw out_of_range("Stack is empty");
        }
        return data.back();
    }

    // 获取栈顶元素（const版本）
    const T& top() const {
        if (isEmpty()) {
            throw out_of_range("Stack is empty");
        }
        return data.back();
    }

    // 检查栈是否为空
    bool isEmpty() const {
        return data.empty();
    }

    // 获取栈的大小
    size_t size() const {
        return data.size();
    }

    // 清空栈
    void clear() {
        data.clear();
    }
};

// 示例1：括号匹配
bool isValidParentheses(const string& s) {
    Stack<char> stack;

    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            stack.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (stack.isEmpty()) {
                return false;
            }

            char top = stack.top();
            stack.pop();

            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return false;
            }
        }
    }

    return stack.isEmpty();
}

// 示例2：计算后缀表达式
int evaluatePostfix(const string& expression) {
    Stack<int> stack;

    for (char c : expression) {
        if (c == ' ') continue;

        if (isdigit(c)) {
            stack.push(c - '0');
        } else {
            int b = stack.top();
            stack.pop();
            int a = stack.top();
            stack.pop();

            switch (c) {
                case '+': stack.push(a + b); break;
                case '-': stack.push(a - b); break;
                case '*': stack.push(a * b); break;
                case '/': stack.push(a / b); break;
            }
        }
    }

    return stack.top();
}

int main() {
    cout << "=== 栈的基本操作 ===" << endl;
    Stack<int> stack;

    stack.push(1);
    stack.push(2);
    stack.push(3);

    cout << "栈的大小: " << stack.size() << endl;
    cout << "栈顶元素: " << stack.top() << endl;

    stack.pop();
    cout << "pop 后的栈顶元素: " << stack.top() << endl;

    cout << "\n=== 示例1：括号匹配 ===" << endl;
    vector<string> tests = {
        "()",
        "()[]{}",
        "(]",
        "([{}])",
        "({[}]})"
    };

    for (const string& test : tests) {
        cout << test << ": "
             << (isValidParentheses(test) ? "有效" : "无效")
             << endl;
    }

    cout << "\n=== 示例2：计算后缀表达式 ===" << endl;
    vector<string> expressions = {
        "2 3 1 * + 9 -",  // 2 + 3*1 - 9 = -4
        "4 5 * 6 +",       // 4*5 + 6 = 26
        "7 2 + 3 * 4 -"    // (7+2)*3 - 4 = 23
    };

    for (const string& expr : expressions) {
        cout << expr << " = " << evaluatePostfix(expr) << endl;
    }

    return 0;
}
