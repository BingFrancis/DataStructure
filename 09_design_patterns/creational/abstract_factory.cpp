// ============================================================================
// 抽象工厂模式 (Abstract Factory Pattern)
// ============================================================================
// 抽象工厂模式提供一个接口，用于创建相关或依赖对象的家族，
// 而不需要明确指定具体类。
//
// 适用场景：
// 1. 需要创建对象家族
// 2. 系统需要多个产品族
// 3. 强调一系列相关的产品对象一起使用
//
// 示例：跨平台 UI 组件工厂
// ============================================================================

#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ============================================================================
// 抽象产品接口（不同类型的产品）
// ============================================================================

// 按钮接口
class Button {
public:
    virtual ~Button() = default;
    virtual void render() const = 0;
    virtual void onClick() const = 0;
};

// 复选框接口
class Checkbox {
public:
    virtual ~Checkbox() = default;
    virtual void render() const = 0;
    virtual void toggle() const = 0;
};

// ============================================================================
// 具体产品（Windows 风格）
// ============================================================================

class WindowsButton : public Button {
public:
    void render() const override {
        cout << "渲染 Windows 按钮样式" << endl;
    }

    void onClick() const override {
        cout << "Windows 按钮被点击" << endl;
    }
};

class WindowsCheckbox : public Checkbox {
public:
    void render() const override {
        cout << "渲染 Windows 复选框样式" << endl;
    }

    void toggle() const override {
        cout << "Windows 复选框状态切换" << endl;
    }
};

// ============================================================================
// 具体产品（MacOS 风格）
// ============================================================================

class MacButton : public Button {
public:
    void render() const override {
        cout << "渲染 MacOS 按钮样式" << endl;
    }

    void onClick() const override {
        cout << "MacOS 按钮被点击" << endl;
    }
};

class MacCheckbox : public Checkbox {
public:
    void render() const override {
        cout << "渲染 MacOS 复选框样式" << endl;
    }

    void toggle() const override {
        cout << "MacOS 复选框状态切换" << endl;
    }
};

// ============================================================================
// 抽象工厂接口
// ============================================================================

class GUIFactory {
public:
    virtual ~GUIFactory() = default;
    virtual unique_ptr<Button> createButton() const = 0;
    virtual unique_ptr<Checkbox> createCheckbox() const = 0;
};

// ============================================================================
// 具体工厂
// ============================================================================

// Windows 工厂
class WindowsFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() const override {
        return make_unique<WindowsButton>();
    }

    unique_ptr<Checkbox> createCheckbox() const override {
        return make_unique<WindowsCheckbox>();
    }
};

// MacOS 工厂
class MacFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() const override {
        return make_unique<MacButton>();
    }

    unique_ptr<Checkbox> createCheckbox() const override {
        return make_unique<MacCheckbox>();
    }
};

// ============================================================================
// 客户端代码
// ============================================================================

// 客户端只使用抽象工厂和抽象产品接口
void renderUI(const GUIFactory& factory) {
    auto button = factory.createButton();
    auto checkbox = factory.createCheckbox();

    button->render();
    checkbox->render();

    button->onClick();
    checkbox->toggle();
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 抽象工厂模式示例 ==========" << endl;

    // 配置：检测操作系统（简化示例）
    string osType = "Windows";  // 可以是 "MacOS"

    unique_ptr<GUIFactory> factory;

    if (osType == "Windows") {
        cout << "\n=== 使用 Windows 风格 UI ===" << endl;
        factory = make_unique<WindowsFactory>();
    } else if (osType == "MacOS") {
        cout << "\n=== 使用 MacOS 风格 UI ===" << endl;
        factory = make_unique<MacFactory>();
    } else {
        cout << "\n未知操作系统！" << endl;
        return 1;
    }

    // 使用工厂创建 UI 组件
    renderUI(*factory);

    // 切换到 MacOS 风格
    cout << "\n=== 切换到 MacOS 风格 UI ===" << endl;
    factory = make_unique<MacFactory>();
    renderUI(*factory);

    cout << "\n========================================" << endl;

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 abstract_factory.cpp -o abstract_factory.exe && ./abstract_factory.exe
// ============================================================================
