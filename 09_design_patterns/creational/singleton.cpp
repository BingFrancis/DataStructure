/**
 * 单例模式 (Singleton Pattern)
 *
 * 确保一个类只有一个实例，并提供全局访问点
 *
 * 应用场景：
 * - 配置管理器
 * - 日志系统
 * - 数据库连接池
 * - 线程池
 */

#include <iostream>
#include <mutex>
#include <memory>

using namespace std;

// ========================================
// 方法1：懒汉模式（线程安全版本）
// ========================================
class SingletonLazy {
private:
    static SingletonLazy* instance;
    static mutex mtx;

    SingletonLazy() {
        cout << "SingletonLazy 构造" << endl;
    }

public:
    // 删除拷贝构造和赋值运算符
    SingletonLazy(const SingletonLazy&) = delete;
    SingletonLazy& operator=(const SingletonLazy&) = delete;

    static SingletonLazy* getInstance() {
        if (!instance) {  // 双重检查锁定
            lock_guard<mutex> lock(mtx);
            if (!instance) {
                instance = new SingletonLazy();
            }
        }
        return instance;
    }

    void doSomething() {
        cout << "SingletonLazy is working" << endl;
    }
};

SingletonLazy* SingletonLazy::instance = nullptr;
mutex SingletonLazy::mtx;

// ========================================
// 方法2：饿汉模式（线程安全，但不是懒加载）
// ========================================
class SingletonEager {
private:
    static SingletonEager instance;

    SingletonEager() {
        cout << "SingletonEager 构造" << endl;
    }

public:
    SingletonEager(const SingletonEager&) = delete;
    SingletonEager& operator=(const SingletonEager&) = delete;

    static SingletonEager& getInstance() {
        return instance;
    }

    void doSomething() {
        cout << "SingletonEager is working" << endl;
    }
};

SingletonEager SingletonEager::instance;

// ========================================
// 方法3：Meyers' Singleton（推荐，现代 C++）
// ========================================
class SingletonMeyers {
private:
    SingletonMeyers() {
        cout << "SingletonMeyers 构造" << endl;
    }

public:
    SingletonMeyers(const SingletonMeyers&) = delete;
    SingletonMeyers& operator=(const SingletonMeyers&) = delete;

    static SingletonMeyers& getInstance() {
        static SingletonMeyers instance;  // C++11 保证线程安全
        return instance;
    }

    void doSomething() {
        cout << "SingletonMeyers is working" << endl;
    }
};

// ========================================
// 方法4：使用智能指针
// ========================================
class SingletonSmartPtr {
private:
    static unique_ptr<SingletonSmartPtr> instance;
    static once_flag initFlag;

    SingletonSmartPtr() {
        cout << "SingletonSmartPtr 构造" << endl;
    }

public:
    SingletonSmartPtr(const SingletonSmartPtr&) = delete;
    SingletonSmartPtr& operator=(const SingletonSmartPtr&) = delete;

    static SingletonSmartPtr& getInstance() {
        call_once(initFlag, []() {
            instance.reset(new SingletonSmartPtr());
        });
        return *instance;
    }

    void doSomething() {
        cout << "SingletonSmartPtr is working" << endl;
    }
};

unique_ptr<SingletonSmartPtr> SingletonSmartPtr::instance;
once_flag SingletonSmartPtr::initFlag;

// ========================================
// 方法5：模板单例（可复用的单例基类）
// ========================================
template<typename T>
class SingletonBase {
protected:
    SingletonBase() = default;
    ~SingletonBase() = default;

public:
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;

    static T& getInstance() {
        static T instance;
        return instance;
    }
};

// 使用模板基类
class MyManager : public SingletonBase<MyManager> {
private:
    MyManager() {
        cout << "MyManager 构造" << endl;
    }

    friend class SingletonBase<MyManager>;

public:
    void work() {
        cout << "MyManager is working" << endl;
    }
};

// ========================================
// 方法6：带销毁控制的单例
// ========================================
class SingletonWithDestroy {
private:
    static SingletonWithDestroy* instance;
    static bool destroyed;

    SingletonWithDestroy() {
        cout << "SingletonWithDestroy 构造" << endl;
    }

    ~SingletonWithDestroy() {
        instance = nullptr;
        destroyed = true;
        cout << "SingletonWithDestroy 析构" << endl;
    }

    static void destroy() {
        if (!destroyed) {
            delete instance;
            instance = nullptr;
        }
    }

public:
    SingletonWithDestroy(const SingletonWithDestroy&) = delete;
    SingletonWithDestroy& operator=(const SingletonWithDestroy&) = delete;

    static SingletonWithDestroy& getInstance() {
        static bool registered = false;
        if (!registered) {
            atexit(destroy);
            registered = true;
        }

        if (!instance) {
            instance = new SingletonWithDestroy();
        }
        return *instance;
    }

    void doSomething() {
        cout << "SingletonWithDestroy is working" << endl;
    }
};

SingletonWithDestroy* SingletonWithDestroy::instance = nullptr;
bool SingletonWithDestroy::destroyed = false;

// ========================================
// 测试
// ========================================
int main() {
    cout << "=== 1. 懒汉模式（线程安全） ===" << endl;
    SingletonLazy* lazy1 = SingletonLazy::getInstance();
    SingletonLazy* lazy2 = SingletonLazy::getInstance();
    cout << "lazy1 == lazy2: " << (lazy1 == lazy2) << endl;
    lazy1->doSomething();
    cout << endl;

    cout << "=== 2. 饿汉模式 ===" << endl;
    SingletonEager& eager1 = SingletonEager::getInstance();
    SingletonEager& eager2 = SingletonEager::getInstance();
    cout << "eager1 == eager2: " << (&eager1 == &eager2) << endl;
    eager1.doSomething();
    cout << endl;

    cout << "=== 3. Meyers' Singleton（推荐） ===" << endl;
    SingletonMeyers& meyers1 = SingletonMeyers::getInstance();
    SingletonMeyers& meyers2 = SingletonMeyers::getInstance();
    cout << "meyers1 == meyers2: " << (&meyers1 == &meyers2) << endl;
    meyers1.doSomething();
    cout << endl;

    cout << "=== 4. 智能指针单例 ===" << endl;
    SingletonSmartPtr& smart1 = SingletonSmartPtr::getInstance();
    SingletonSmartPtr& smart2 = SingletonSmartPtr::getInstance();
    cout << "smart1 == smart2: " << (&smart1 == &smart2) << endl;
    smart1.doSomething();
    cout << endl;

    cout << "=== 5. 模板单例 ===" << endl;
    MyManager& mgr1 = MyManager::getInstance();
    MyManager& mgr2 = MyManager::getInstance();
    cout << "mgr1 == mgr2: " << (&mgr1 == &mgr2) << endl;
    mgr1.work();
    cout << endl;

    cout << "=== 6. 带销毁控制的单例 ===" << endl;
    SingletonWithDestroy& destroy1 = SingletonWithDestroy::getInstance();
    SingletonWithDestroy& destroy2 = SingletonWithDestroy::getInstance();
    cout << "destroy1 == destroy2: " << (&destroy1 == &destroy2) << endl;
    destroy1.doSomething();
    cout << endl;

    cout << "程序退出时，SingletonWithDestroy 将自动析构" << endl;

    return 0;
}
