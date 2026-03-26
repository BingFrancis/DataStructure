// ============================================================================
// 适配器模式 (Adapter Pattern)
// ============================================================================
// 适配器模式将一个类的接口转换成客户希望的另一个接口。
// 适配器使得原本由于接口不兼容而不能一起工作的那些类可以一起工作。
//
// 适用场景：
// 1. 需要使用现有的类，但其接口与需要的接口不匹配
// 2. 想要创建一个可以复用的类，该类可以与其他不相关的类或不可预见的类协同工作
// 3. 需要使用多个子类，但通过让每个子类子类化一个适配器类，可以针对每个子类调整接口
//
// 示例：第三方支付网关适配
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// 目标接口（Target）- 客户端期望的接口
// ============================================================================

// 支付处理器接口
class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;
    virtual void processPayment(double amount) = 0;
    virtual void refundPayment(double amount) = 0;
};

// ============================================================================
// 被适配者（Adaptee）- 需要适配的现有类
// ============================================================================

// 第三方支付网关 A（使用不同的接口）
class LegacyPaymentGatewayA {
public:
    void makeTransaction(string type, double value) {
        cout << "[Gateway A] 交易类型: " << type
             << ", 金额: $" << value << endl;
    }

    void reverseTransaction(int transactionId) {
        cout << "[Gateway A] 退款交易 ID: " << transactionId << endl;
    }
};

// 第三方支付网关 B（另一个不同的接口）
class LegacyPaymentGatewayB {
public:
    bool sendPayment(const string& currency, double amount) {
        cout << "[Gateway B] 发送支付: " << currency << amount << endl;
        return true;
    }

    bool cancelPayment(const string& paymentId) {
        cout << "[Gateway B] 取消支付 ID: " << paymentId << endl;
        return true;
    }
};

// ============================================================================
// 适配器类（Adapter）
// ============================================================================

// 网关 A 的适配器
class GatewayAAdapter : public PaymentProcessor {
private:
    LegacyPaymentGatewayA& gateway;
    int nextTransactionId = 1000;

public:
    explicit GatewayAAdapter(LegacyPaymentGatewayA& g) : gateway(g) {}

    void processPayment(double amount) override {
        gateway.makeTransaction("PAYMENT", amount);
    }

    void refundPayment(double amount) override {
        gateway.reverseTransaction(nextTransactionId++);
    }
};

// 网关 B 的适配器
class GatewayBAdapter : public PaymentProcessor {
private:
    LegacyPaymentGatewayB& gateway;
    int transactionCounter = 0;

public:
    explicit GatewayBAdapter(LegacyPaymentGatewayB& g) : gateway(g) {}

    void processPayment(double amount) override {
        string paymentId = "PAY-" + to_string(transactionCounter++);
        gateway.sendPayment("USD", amount);
    }

    void refundPayment(double amount) override {
        string paymentId = "PAY-" + to_string(transactionCounter - 1);
        gateway.cancelPayment(paymentId);
    }
};

// ============================================================================
// 客户端代码
// ============================================================================

// 电商系统 - 使用统一的支付接口
class ECommerceSystem {
private:
    PaymentProcessor* paymentProcessor;

public:
    explicit ECommerceSystem(PaymentProcessor* processor)
        : paymentProcessor(processor) {}

    void checkout(double total) {
        cout << "\n=== 结账流程 ===" << endl;
        cout << "订单总额: $" << total << endl;
        paymentProcessor->processPayment(total);
        cout << "支付成功！" << endl;
    }

    void processRefund(double amount) {
        cout << "\n=== 退款流程 ===" << endl;
        cout << "退款金额: $" << amount << endl;
        paymentProcessor->refundPayment(amount);
        cout << "退款完成！" << endl;
    }
};

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 适配器模式示例 ==========" << endl;

    // 创建第三方支付网关实例
    LegacyPaymentGatewayA gatewayA;
    LegacyPaymentGatewayB gatewayB;

    // 创建适配器
    GatewayAAdapter adapterA(gatewayA);
    GatewayBAdapter adapterB(gatewayB);

    // 创建电商系统，使用适配后的网关
    cout << "\n--- 使用支付网关 A ---" << endl;
    ECommerceSystem shopA(&adapterA);
    shopA.checkout(99.99);
    shopA.processRefund(49.99);

    cout << "\n--- 使用支付网关 B ---" << endl;
    ECommerceSystem shopB(&adapterB);
    shopB.checkout(149.99);
    shopB.processRefund(74.99);

    cout << "\n========================================" << endl;

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 adapter.cpp -o adapter.exe && ./adapter.exe
// ============================================================================
