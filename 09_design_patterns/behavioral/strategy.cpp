/**
 * 策略模式 (Strategy Pattern)
 *
 * 定义一系列算法，把它们封装起来，并使它们可以互换
 *
 * 应用场景：
 * - 不同的排序算法
 * - 不同的支付方式
 * - 不同的数据压缩算法
 * - 不同的路径规划算法
 */

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>

using namespace std;

// ========================================
// 方法1：传统策略模式（基于接口）
// ========================================

// 抽象策略
class SortingStrategy {
public:
    virtual ~SortingStrategy() = default;
    virtual void sort(vector<int>& arr) = 0;
};

// 具体策略：冒泡排序
class BubbleSort : public SortingStrategy {
public:
    void sort(vector<int>& arr) override {
        size_t n = arr.size();
        for (size_t i = 0; i < n - 1; i++) {
            for (size_t j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
        cout << "使用冒泡排序" << endl;
    }
};

// 具体策略：快速排序
class QuickSort : public SortingStrategy {
private:
    void quickSort(vector<int>& arr, int low, int high) {
        if (low < high) {
            int pivot = arr[high];
            int i = low - 1;

            for (int j = low; j < high; j++) {
                if (arr[j] < pivot) {
                    i++;
                    swap(arr[i], arr[j]);
                }
            }
            swap(arr[i + 1], arr[high]);

            int pi = i + 1;
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

public:
    void sort(vector<int>& arr) override {
        if (!arr.empty()) {
            quickSort(arr, 0, arr.size() - 1);
        }
        cout << "使用快速排序" << endl;
    }
};

// 具体策略：STL 排序
class STLSort : public SortingStrategy {
public:
    void sort(vector<int>& arr) override {
        std::sort(arr.begin(), arr.end());
        cout << "使用 STL 排序" << endl;
    }
};

// 上下文
class Sorter {
private:
    unique_ptr<SortingStrategy> strategy;

public:
    void setStrategy(unique_ptr<SortingStrategy> s) {
        strategy = move(s);
    }

    void sort(vector<int>& arr) {
        if (strategy) {
            strategy->sort(arr);
        }
    }
};

// ========================================
// 方法2：现代 C++ 策略模式（基于 std::function）
// ========================================

class ModernSorter {
private:
    function<void(vector<int>&)> strategy;

public:
    void setStrategy(function<void(vector<int>&)> s) {
        strategy = s;
    }

    void sort(vector<int>& arr) {
        if (strategy) {
            strategy(arr);
        }
    }
};

// ========================================
// 方法3：支付系统示例
// ========================================

// 抽象支付策略
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual void pay(double amount) = 0;
};

// 信用卡支付
class CreditCardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "使用信用卡支付: $" << amount << endl;
    }
};

// 支付宝支付
class AlipayPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "使用支付宝支付: ￥" << amount << endl;
    }
};

// 微信支付
class WeChatPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "使用微信支付: ￥" << amount << endl;
    }
};

// 支付上下文
class PaymentContext {
private:
    unique_ptr<PaymentStrategy> strategy;

public:
    void setPaymentStrategy(unique_ptr<PaymentStrategy> s) {
        strategy = move(s);
    }

    void pay(double amount) {
        if (strategy) {
            strategy->pay(amount);
        }
    }
};

// ========================================
// 方法4：折扣策略
// ========================================

class DiscountStrategy {
public:
    virtual ~DiscountStrategy() = default;
    virtual double calculateDiscount(double price) = 0;
};

// 无折扣
class NoDiscount : public DiscountStrategy {
public:
    double calculateDiscount(double price) override {
        return price;
    }
};

// 固定折扣
class FixedDiscount : public DiscountStrategy {
private:
    double discountAmount;

public:
    explicit FixedDiscount(double amount) : discountAmount(amount) {}

    double calculateDiscount(double price) override {
        return max(0.0, price - discountAmount);
    }
};

// 百分比折扣
class PercentageDiscount : public DiscountStrategy {
private:
    double percentage;

public:
    explicit PercentageDiscount(double percent) : percentage(percent / 100.0) {}

    double calculateDiscount(double price) override {
        return price * (1.0 - percentage);
    }
};

// 购物车上下文
class ShoppingCart {
private:
    unique_ptr<DiscountStrategy> strategy;

public:
    void setDiscountStrategy(unique_ptr<DiscountStrategy> s) {
        strategy = move(s);
    }

    double checkout(double price) {
        if (strategy) {
            return strategy->calculateDiscount(price);
        }
        return price;
    }
};

// ========================================
// 辅助函数
// ========================================

void printArray(const vector<int>& arr) {
    for (int val : arr) {
        cout << val << " ";
    }
    cout << endl;
}

// ========================================
// 测试
// ========================================

void testSortingStrategies() {
    cout << "=== 排序策略 ===" << endl;

    vector<int> arr1 = {64, 34, 25, 12, 22, 11, 90};
    vector<int> arr2 = {64, 34, 25, 12, 22, 11, 90};
    vector<int> arr3 = {64, 34, 25, 12, 22, 11, 90};

    Sorter sorter;

    cout << "原数组: ";
    printArray(arr1);

    sorter.setStrategy(make_unique<BubbleSort>());
    sorter.sort(arr1);
    cout << "排序后: ";
    printArray(arr1);

    sorter.setStrategy(make_unique<QuickSort>());
    sorter.sort(arr2);
    cout << "排序后: ";
    printArray(arr2);

    sorter.setStrategy(make_unique<STLSort>());
    sorter.sort(arr3);
    cout << "排序后: ";
    printArray(arr3);
    cout << endl;
}

void testModernSortingStrategies() {
    cout << "=== 现代 C++ 排序策略 ===" << endl;

    ModernSorter sorter;
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};

    // 使用 lambda 作为策略
    sorter.setStrategy([](vector<int>& a) {
        sort(a.begin(), a.end(), greater<int>());
        cout << "使用降序排序" << endl;
    });

    cout << "原数组: ";
    printArray(arr);

    sorter.sort(arr);
    cout << "排序后: ";
    printArray(arr);
    cout << endl;
}

void testPaymentStrategies() {
    cout << "=== 支付策略 ===" << endl;

    PaymentContext context;

    context.setPaymentStrategy(make_unique<CreditCardPayment>());
    context.pay(100.0);

    context.setPaymentStrategy(make_unique<AlipayPayment>());
    context.pay(680.0);

    context.setPaymentStrategy(make_unique<WeChatPayment>());
    context.pay(520.0);
    cout << endl;
}

void testDiscountStrategies() {
    cout << "=== 折扣策略 ===" << endl;

    ShoppingCart cart;
    double price = 1000.0;

    cout << "原价: $" << price << endl;

    cart.setDiscountStrategy(make_unique<NoDiscount>());
    cout << "无折扣: $" << cart.checkout(price) << endl;

    cart.setDiscountStrategy(make_unique<FixedDiscount>(100.0));
    cout << "固定折扣 $100: $" << cart.checkout(price) << endl;

    cart.setDiscountStrategy(make_unique<PercentageDiscount>(20.0));
    cout << "20% 折扣: $" << cart.checkout(price) << endl;
    cout << endl;
}

int main() {
    testSortingStrategies();
    testModernSortingStrategies();
    testPaymentStrategies();
    testDiscountStrategies();

    return 0;
}
