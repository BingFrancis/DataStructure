/**
 * 观察者模式 (Observer Pattern)
 *
 * 定义对象间的一对多依赖，当一个对象状态改变时，所有依赖它的对象都会收到通知
 *
 * 应用场景：
 * - 事件系统
 * - GUI 框架
 * - 消息队列
 * - 发布-订阅系统
 */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

// ========================================
// 方法1：传统观察者模式（基于接口）
// ========================================

// 抽象观察者
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(int data) = 0;
};

// 具体观察者A
class ConcreteObserverA : public Observer {
public:
    void update(int data) override {
        cout << "ObserverA 收到更新: " << data << endl;
    }
};

// 具体观察者B
class ConcreteObserverB : public Observer {
public:
    void update(int data) override {
        cout << "ObserverB 收到更新: " << data * 2 << endl;
    }
};

// 主题（被观察者）
class Subject {
private:
    vector<Observer*> observers;
    int data;

public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    void detach(Observer* observer) {
        observers.erase(
            remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

    void notify() {
        for (auto* observer : observers) {
            observer->update(data);
        }
    }

    void setData(int newData) {
        data = newData;
        notify();
    }
};

// ========================================
// 方法2：现代 C++ 观察者模式（基于 std::function）
// ========================================

class ModernSubject {
private:
    vector<function<void(int)>> observers;
    int data;

public:
    // 注册观察者（支持 lambda、函数指针、函数对象）
    size_t attach(function<void(int)> observer) {
        observers.push_back(observer);
        return observers.size() - 1;
    }

    void detach(size_t index) {
        if (index < observers.size()) {
            observers.erase(observers.begin() + index);
        }
    }

    void notify() {
        for (auto& observer : observers) {
            observer(data);
        }
    }

    void setData(int newData) {
        data = newData;
        notify();
    }
};

// ========================================
// 方法3：类型安全的现代观察者模式
// ========================================

template<typename Event>
class Observable {
private:
    vector<function<void(const Event&)>> observers;

public:
    void subscribe(function<void(const Event&)> observer) {
        observers.push_back(observer);
    }

    void notify(const Event& event) {
        for (auto& observer : observers) {
            observer(event);
        }
    }
};

// 事件类型
struct UserEvent {
    string username;
    string action;
};

// ========================================
// 方法4：观察者管理器（支持 ID 管理）
// ========================================

class ObserverManager {
private:
    vector<pair<size_t, function<void(int)>>> observers;
    size_t nextId = 0;

public:
    size_t attach(function<void(int)> observer) {
        observers.emplace_back(nextId, observer);
        return nextId++;
    }

    void detach(size_t id) {
        observers.erase(
            remove_if(observers.begin(), observers.end(),
                [id](const auto& p) { return p.first == id; }),
            observers.end()
        );
    }

    void notify(int data) {
        for (auto& [id, observer] : observers) {
            observer(data);
        }
    }
};

// ========================================
// 方法5：多事件观察者模式
// ========================================

enum class EventType {
    DATA_CHANGED,
    STATUS_CHANGED,
    ERROR_OCCURRED
};

class MultiEventSubject {
private:
    unordered_map<EventType, vector<function<void()>>> observers;

public:
    void subscribe(EventType type, function<void()> observer) {
        observers[type].push_back(observer);
    }

    void notify(EventType type) {
        if (observers.find(type) != observers.end()) {
            for (auto& observer : observers[type]) {
                observer();
            }
        }
    }

    void publishDataChanged() { notify(EventType::DATA_CHANGED); }
    void publishStatusChanged() { notify(EventType::STATUS_CHANGED); }
    void publishErrorOccurred() { notify(EventType::ERROR_OCCURRED); }
};

// ========================================
// 测试
// ========================================

void testTraditionalObserver() {
    cout << "=== 传统观察者模式 ===" << endl;

    Subject subject;
    ConcreteObserverA obsA;
    ConcreteObserverB obsB;

    subject.attach(&obsA);
    subject.attach(&obsB);

    subject.setData(10);
    cout << endl;
}

void testModernObserver() {
    cout << "=== 现代 C++ 观察者模式 ===" << endl;

    ModernSubject subject;

    // 使用 lambda
    subject.attach([](int data) {
        cout << "Lambda 观察者: " << data << endl;
    });

    // 使用函数对象
    auto observer2 = [](int data) {
        cout << "另一个 Lambda: " << data * data << endl;
    };
    subject.attach(observer2);

    subject.setData(5);
    cout << endl;
}

void testTypeSafeObserver() {
    cout << "=== 类型安全观察者模式 ===" << endl;

    Observable<UserEvent> observable;

    observable.subscribe([](const UserEvent& event) {
        cout << "用户 " << event.username << " " << event.action << endl;
    });

    observable.subscribe([](const UserEvent& event) {
        cout << "记录日志: " << event.action << endl;
    });

    observable.notify({"Alice", "登录"});
    observable.notify({"Bob", "登出"});
    cout << endl;
}

void testObserverManager() {
    cout << "=== 观察者管理器 ===" << endl;

    ObserverManager manager;

    size_t id1 = manager.attach([](int data) {
        cout << "观察者 1: " << data << endl;
    });

    size_t id2 = manager.attach([](int data) {
        cout << "观察者 2: " << data * 2 << endl;
    });

    manager.notify(7);

    cout << "删除观察者 1 后:" << endl;
    manager.detach(id1);
    manager.notify(7);
    cout << endl;
}

void testMultiEventObserver() {
    cout << "=== 多事件观察者模式 ===" << endl;

    MultiEventSubject subject;

    subject.subscribe(EventType::DATA_CHANGED, []() {
        cout << "数据已更改！" << endl;
    });

    subject.subscribe(EventType::STATUS_CHANGED, []() {
        cout << "状态已更改！" << endl;
    });

    subject.subscribe(EventType::ERROR_OCCURRED, []() {
        cout << "发生错误！" << endl;
    });

    subject.publishDataChanged();
    subject.publishStatusChanged();
    subject.publishErrorOccurred();
    cout << endl;
}

int main() {
    testTraditionalObserver();
    testModernObserver();
    testTypeSafeObserver();
    testObserverManager();
    testMultiEventObserver();

    return 0;
}
