// ============================================================================
// 装饰器模式 (Decorator Pattern)
// ============================================================================
// 装饰器模式动态地给一个对象添加一些额外的职责。
// 就增加功能来说，装饰器模式比生成子类更为灵活。
//
// 适用场景：
// 1. 需要动态地、透明地给单个对象添加职责
// 2. 需要灵活地添加或撤销对象的职责
// 3. 通过使用类导致子类爆炸时，可以使用装饰器
//
// 示例：咖啡订单系统
// ============================================================================

#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ============================================================================
// 组件接口（Component）
// ============================================================================

// 饮料抽象类
class Beverage {
public:
    virtual ~Beverage() = default;
    virtual string getDescription() const = 0;
    virtual double cost() const = 0;
};

// ============================================================================
// 具体组件（Concrete Component）
// ============================================================================

// 浓缩咖啡
class Espresso : public Beverage {
public:
    string getDescription() const override {
        return "浓缩咖啡";
    }

    double cost() const override {
        return 1.99;
    }
};

// 混合咖啡
class HouseBlend : public Beverage {
public:
    string getDescription() const override {
        return "混合咖啡";
    }

    double cost() const override {
        return 0.89;
    }
};

// 低咖啡因
class Decaf : public Beverage {
public:
    string getDescription() const override {
        return "低咖啡因";
    }

    double cost() const override {
        return 1.05;
    }
};

// 黑咖啡
class DarkRoast : public Beverage {
public:
    string getDescription() const override {
        return "黑咖啡";
    }

    double cost() const override {
        return 0.99;
    }
};

// ============================================================================
// 装饰器抽象类（Decorator）
// ============================================================================

class CondimentDecorator : public Beverage {
private:
    unique_ptr<Beverage> beverage;

protected:
    CondimentDecorator(unique_ptr<Beverage> bev) : beverage(move(bev)) {}

    // 获取被装饰对象的描述
    string getBeverageDescription() const {
        return beverage->getDescription();
    }

    // 获取被装饰对象的价格
    double getBeverageCost() const {
        return beverage->cost();
    }

    Beverage* getBeverage() const { return beverage.get(); }
};

// ============================================================================
// 具体装饰器（Concrete Decorators）
// ============================================================================

// 牛奶
class Milk : public CondimentDecorator {
public:
    explicit Milk(unique_ptr<Beverage> bev) : CondimentDecorator(move(bev)) {}

    string getDescription() const override {
        return getBeverageDescription() + ", 牛奶";
    }

    double cost() const override {
        return getBeverageCost() + 0.10;
    }
};

// 摩卡
class Mocha : public CondimentDecorator {
public:
    explicit Mocha(unique_ptr<Beverage> bev) : CondimentDecorator(move(bev)) {}

    string getDescription() const override {
        return getBeverageDescription() + ", 摩卡";
    }

    double cost() const override {
        return getBeverageCost() + 0.20;
    }
};

// 大豆
class Soy : public CondimentDecorator {
public:
    explicit Soy(unique_ptr<Beverage> bev) : CondimentDecorator(move(bev)) {}

    string getDescription() const override {
        return getBeverageDescription() + ", 大豆";
    }

    double cost() const override {
        return getBeverageCost() + 0.15;
    }
};

// 奶泡
class Whip : public CondimentDecorator {
public:
    explicit Whip(unique_ptr<Beverage> bev) : CondimentDecorator(move(bev)) {}

    string getDescription() const override {
        return getBeverageDescription() + ", 奶泡";
    }

    double cost() const override {
        return getBeverageCost() + 0.10;
    }
};

// 焦糖
class Caramel : public CondimentDecorator {
public:
    explicit Caramel(unique_ptr<Beverage> bev) : CondimentDecorator(move(bev)) {}

    string getDescription() const override {
        return getBeverageDescription() + ", 焦糖";
    }

    double cost() const override {
        return getBeverageCost() + 0.25;
    }
};

// 香草糖浆
class VanillaSyrup : public CondimentDecorator {
public:
    explicit VanillaSyrup(unique_ptr<Beverage> bev) : CondimentDecorator(move(bev)) {}

    string getDescription() const override {
        return getBeverageDescription() + ", 香草糖浆";
    }

    double cost() const override {
        return getBeverageCost() + 0.30;
    }
};

// ============================================================================
// 订单系统
// ============================================================================

void displayOrder(unique_ptr<Beverage> beverage) {
    cout << "\n--- 订单详情 ---" << endl;
    cout << "饮品: " << beverage->getDescription() << endl;
    cout << "价格: $" << beverage->cost() << endl;
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 装饰器模式示例 ==========" << endl;

    // 示例 1：纯浓缩咖啡
    cout << "\n=== 订单 1：纯浓缩咖啡 ===" << endl;
    unique_ptr<Beverage> order1 = make_unique<Espresso>();
    displayOrder(move(order1));

    // 示例 2：浓缩咖啡 + 摩卡 + 摩卡 + 奶泡
    cout << "\n=== 订单 2：双重摩卡浓缩咖啡 ===" << endl;
    unique_ptr<Beverage> order2 = make_unique<Espresso>();
    order2 = make_unique<Mocha>(move(order2));
    order2 = make_unique<Mocha>(move(order2));
    order2 = make_unique<Whip>(move(order2));
    displayOrder(move(order2));

    // 示例 3：黑咖啡 + 大豆 + 摩卡 + 奶泡
    cout << "\n=== 订单 3：黑咖啡 + 多重配料 ===" << endl;
    unique_ptr<Beverage> order3 = make_unique<DarkRoast>();
    order3 = make_unique<Soy>(move(order3));
    order3 = make_unique<Mocha>(move(order3));
    order3 = make_unique<Whip>(move(order3));
    displayOrder(move(order3));

    // 示例 4：低咖啡因 + 焦糖 + 香草糖浆
    cout << "\n=== 订单 4：甜味低咖啡因 ===" << endl;
    unique_ptr<Beverage> order4 = make_unique<Decaf>();
    order4 = make_unique<Caramel>(move(order4));
    order4 = make_unique<VanillaSyrup>(move(order4));
    displayOrder(move(order4));

    // 示例 5：混合咖啡 + 牛奶 + 大豆 + 焦糖 + 奶泡 + 香草糖浆
    cout << "\n=== 订单 5：超大杯混合咖啡 ===" << endl;
    unique_ptr<Beverage> order5 = make_unique<HouseBlend>();
    order5 = make_unique<Milk>(move(order5));
    order5 = make_unique<Soy>(move(order5));
    order5 = make_unique<Caramel>(move(order5));
    order5 = make_unique<Whip>(move(order5));
    order5 = make_unique<VanillaSyrup>(move(order5));
    displayOrder(move(order5));

    cout << "\n========================================" << endl;

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 decorator.cpp -o decorator.exe && ./decorator.exe
// ============================================================================
