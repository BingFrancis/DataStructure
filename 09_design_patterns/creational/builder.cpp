// ============================================================================
// 建造者模式 (Builder Pattern)
// ============================================================================
// 建造者模式将复杂对象的构造与其表示分离，
// 使得同样的构造过程可以创建不同的表示。
//
// 适用场景：
// 1. 创建复杂对象，有多个组成部分
// 2. 需要多种方式构造对象
// 3. 构造过程与表示分离
//
// 示例：HTML 文档构建器
// ============================================================================

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// 产品类
// ============================================================================

// HTML 元素
class HTMLElement {
private:
    string tag;
    string text;
    vector<unique_ptr<HTMLElement>> children;

public:
    HTMLElement(const string& t, const string& txt = "")
        : tag(t), text(txt) {}

    void addChild(unique_ptr<HTMLElement> child) {
        children.push_back(move(child));
    }

    string toString(int indent = 0) const {
        string result;
        string padding(indent * 2, ' ');

        result += padding + "<" + tag + ">\n";

        if (!text.empty()) {
            result += padding + "  " + text + "\n";
        }

        for (const auto& child : children) {
            result += child->toString(indent + 1);
        }

        result += padding + "</" + tag + ">\n";
        return result;
    }
};

// ============================================================================
// 抽象建造者接口
// ============================================================================

class HTMLBuilder {
public:
    virtual ~HTMLBuilder() = default;
    virtual void buildTitle(const string& text) = 0;
    virtual void buildParagraph(const string& text) = 0;
    virtual void buildList(const vector<string>& items) = 0;
    virtual unique_ptr<HTMLElement> getResult() = 0;
};

// ============================================================================
// 具体建造者
// ============================================================================

// 简单的 HTML 构建器
class SimpleHTMLBuilder : public HTMLBuilder {
private:
    unique_ptr<HTMLElement> root;

public:
    SimpleHTMLBuilder() {
        root = make_unique<HTMLElement>("html");
    }

    void buildTitle(const string& text) override {
        auto title = make_unique<HTMLElement>("h1", text);
        root->addChild(move(title));
    }

    void buildParagraph(const string& text) override {
        auto paragraph = make_unique<HTMLElement>("p", text);
        root->addChild(move(paragraph));
    }

    void buildList(const vector<string>& items) override {
        auto ul = make_unique<HTMLElement>("ul");

        for (const auto& item : items) {
            auto li = make_unique<HTMLElement>("li", item);
            ul->addChild(move(li));
        }

        root->addChild(move(ul));
    }

    unique_ptr<HTMLElement> getResult() override {
        return move(root);
    }
};

// 装饰性的 HTML 构建器（添加样式）
class StyledHTMLBuilder : public HTMLBuilder {
private:
    unique_ptr<HTMLElement> root;

public:
    StyledHTMLBuilder() {
        root = make_unique<HTMLElement>("html");
    }

    void buildTitle(const string& text) override {
        auto title = make_unique<HTMLElement>("h1", text);
        root->addChild(move(title));
        // 添加样式装饰
        cout << "添加标题样式：大号加粗字体" << endl;
    }

    void buildParagraph(const string& text) override {
        auto paragraph = make_unique<HTMLElement>("p", text);
        root->addChild(move(paragraph));
        // 添加样式装饰
        cout << "添加段落样式：首行缩进 2em" << endl;
    }

    void buildList(const vector<string>& items) override {
        auto ul = make_unique<HTMLElement>("ul");

        for (const auto& item : items) {
            auto li = make_unique<HTMLElement>("li", item);
            ul->addChild(move(li));
        }

        root->addChild(move(ul));
        // 添加样式装饰
        cout << "添加列表样式：圆点样式" << endl;
    }

    unique_ptr<HTMLElement> getResult() override {
        return move(root);
    }
};

// ============================================================================
// 指挥者（Director）
// ============================================================================

class HTMLDirector {
private:
    HTMLBuilder* builder;

public:
    explicit HTMLDirector(HTMLBuilder* b) : builder(b) {}

    void constructArticle(const string& title, const string& content,
                       const vector<string>& items) {
        builder->buildTitle(title);
        builder->buildParagraph(content);
        builder->buildList(items);
    }

    void constructListPage(const string& title, const vector<string>& items) {
        builder->buildTitle(title);
        builder->buildList(items);
    }
};

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 建造者模式示例 ==========" << endl;

    // 创建简单的 HTML 文档
    cout << "\n--- 简单 HTML 文档 ---" << endl;
    SimpleHTMLBuilder simpleBuilder;
    HTMLDirector director1(&simpleBuilder);

    director1.constructArticle(
        "我的第一篇文章",
        "这是文章的内容。建造者模式允许我们分步骤构建复杂对象。",
        {"要点1", "要点2", "要点3"}
    );

    auto simpleHTML = simpleBuilder.getResult();
    cout << "\n生成的 HTML：\n" << simpleHTML->toString();

    // 创建装饰性的 HTML 文档
    cout << "\n\n--- 装饰性 HTML 文档 ---" << endl;
    StyledHTMLBuilder styledBuilder;
    HTMLDirector director2(&styledBuilder);

    director2.constructListPage(
        "购物清单",
        {"苹果", "香蕉", "橙子", "葡萄"}
    );

    auto styledHTML = styledBuilder.getResult();
    cout << "\n生成的 HTML：\n" << styledHTML->toString();

    cout << "\n========================================" << endl;

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 builder.cpp -o builder.exe && ./builder.exe
// ============================================================================
