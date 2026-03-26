// ============================================================================
// 状态模式 (State Pattern)
// ============================================================================
// 状态模式允许一个对象在其内部状态改变时改变它的行为。
// 对象看起来好像修改了它的类。
//
// 适用场景：
// 1. 对象的行为取决于它的状态，并且它必须在运行时刻根据状态改变它的行为
// 2. 一个操作中含有庞大的多分支的条件语句，且这些分支依赖于该对象的状态
// 3. 状态转换是显式的，而不是隐藏在条件语句中
//
// 示例：订单状态管理
// ============================================================================

#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ============================================================================
// 前向声明
// ============================================================================

class Order;
class OrderContext;

// ============================================================================
// 状态接口（抽象状态）
// ============================================================================

class OrderState {
public:
    virtual ~OrderState() = default;

    virtual void processPayment(Order* order) = 0;
    virtual void shipOrder(Order* order) = 0;
    virtual void deliverOrder(Order* order) = 0;
    virtual void cancelOrder(Order* order) = 0;
    virtual string getName() const = 0;
};

// ============================================================================
// 具体状态类
// ============================================================================

// 新订单状态
class NewOrderState : public OrderState {
public:
    void processPayment(Order* order) override;

    void shipOrder(Order* order) override {
        cout << "错误：订单尚未支付，无法发货！" << endl;
    }

    void deliverOrder(Order* order) override {
        cout << "错误：订单尚未发货，无法送达！" << endl;
    }

    void cancelOrder(Order* order) override;

    string getName() const override { return "新订单"; }
};

// 已支付状态
class PaidState : public OrderState {
public:
    void processPayment(Order* order) override {
        cout << "错误：订单已经支付过！" << endl;
    }

    void shipOrder(Order* order) override;

    void deliverOrder(Order* order) override {
        cout << "错误：订单尚未发货，无法送达！" << endl;
    }

    void cancelOrder(Order* order) override {
        cout << "警告：订单已支付，取消将退款" << endl;
        // 在实际实现中会退款
    }

    string getName() const override { return "已支付"; }
};

// 已发货状态
class ShippedState : public OrderState {
public:
    void processPayment(Order* order) override {
        cout << "错误：订单已发货，不能支付！" << endl;
    }

    void shipOrder(Order* order) override {
        cout << "错误：订单已发货！" << endl;
    }

    void deliverOrder(Order* order) override;

    void cancelOrder(Order* order) override {
        cout << "错误：订单已发货，无法取消！" << endl;
    }

    string getName() const override { return "已发货"; }
};

// 已送达状态
class DeliveredState : public OrderState {
public:
    void processPayment(Order* order) override {
        cout << "错误：订单已送达，不能支付！" << endl;
    }

    void shipOrder(Order* order) override {
        cout << "错误：订单已送达，不能发货！" << endl;
    }

    void deliverOrder(Order* order) override {
        cout << "错误：订单已送达！" << endl;
    }

    void cancelOrder(Order* order) override {
        cout << "错误：订单已送达，无法取消！" << endl;
    }

    string getName() const override { return "已送达"; }
};

// 已取消状态
class CancelledState : public OrderState {
public:
    void processPayment(Order* order) override {
        cout << "错误：订单已取消！" << endl;
    }

    void shipOrder(Order* order) override {
        cout << "错误：订单已取消，不能发货！" << endl;
    }

    void deliverOrder(Order* order) override {
        cout << "错误：订单已取消，不能送达！" << endl;
    }

    void cancelOrder(Order* order) override {
        cout << "错误：订单已取消！" << endl;
    }

    string getName() const override { return "已取消"; }
};

// ============================================================================
// 上下文类（Context）
// ============================================================================

class Order {
private:
    unique_ptr<OrderState> currentState;
    string orderId;

    friend class NewOrderState;
    friend class PaidState;
    friend class ShippedState;
    friend class DeliveredState;
    friend class CancelledState;

public:
    Order(const string& id) : orderId(id) {
        cout << "创建订单: " << orderId << endl;
        currentState = make_unique<NewOrderState>();
    }

    void setState(unique_ptr<OrderState> newState) {
        currentState = move(newState);
    }

    void processPayment() {
        cout << "\n--- 处理支付 ---" << endl;
        currentState->processPayment(this);
    }

    void ship() {
        cout << "\n--- 处理发货 ---" << endl;
        currentState->shipOrder(this);
    }

    void deliver() {
        cout << "\n--- 处理送达 ---" << endl;
        currentState->deliverOrder(this);
    }

    void cancel() {
        cout << "\n--- 处理取消 ---" << endl;
        currentState->cancelOrder(this);
    }

    void displayStatus() {
        cout << "\n订单 " << orderId << " 当前状态: " << currentState->getName() << endl;
    }
};

// ============================================================================
// 状态转换实现
// ============================================================================

void NewOrderState::processPayment(Order* order) {
    cout << "处理订单支付..." << endl;
    cout << "支付成功！" << endl;
    order->setState(make_unique<PaidState>());
}

void NewOrderState::cancelOrder(Order* order) {
    cout << "取消新订单..." << endl;
    order->setState(make_unique<CancelledState>());
}

void PaidState::shipOrder(Order* order) {
    cout << "发货订单..." << endl;
    cout << "订单已发货！" << endl;
    order->setState(make_unique<ShippedState>());
}

void ShippedState::deliverOrder(Order* order) {
    cout << "送达订单..." << endl;
    cout << "订单已送达！" << endl;
    order->setState(make_unique<DeliveredState>());
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 状态模式示例 ==========" << endl;

    // 创建订单
    Order order("ORD-2024-001");
    order.displayStatus();

    // 正常流程
    cout << "\n========== 正常订单流程 ==========";
    order.processPayment();
    order.displayStatus();

    order.ship();
    order.displayStatus();

    order.deliver();
    order.displayStatus();

    // 尝试已送达订单的操作
    cout << "\n========== 已送达订单的操作 ==========";
    order.processPayment();
    order.ship();
    order.deliver();
    order.cancel();

    // 创建新订单并取消
    cout << "\n========== 取消订单流程 ==========";
    Order order2("ORD-2024-002");
    order2.displayStatus();
    order2.cancel();
    order2.displayStatus();

    cout << "\n========================================" << endl;

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 state.cpp -o state.exe && ./state.exe
// ============================================================================
