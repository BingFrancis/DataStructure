# 09 - Design Patterns（设计模式）

本模块涵盖常用的设计模式及其 C++ 实现。

## 目录结构

```
09_design_patterns/
├── creational/              # 创建型模式
│   ├── singleton.cpp        # 单例模式
│   ├── factory_method.cpp   # 工厂方法模式
│   ├── abstract_factory.cpp # 抽象工厂模式
│   └── builder.cpp          # 建造者模式
├── structural/              # 结构型模式
│   ├── adapter.cpp          # 适配器模式
│   ├── decorator.cpp       # 装饰器模式
│   ├── proxy.cpp            # 代理模式
│   └── facade.cpp           # 外观模式
├── behavioral/              # 行为型模式
│   ├── observer.cpp         # 观察者模式
│   ├── strategy.cpp         # 策略模式
│   ├── command.cpp          # 命令模式
│   └── state.cpp            # 状态模式
└── README.md                # 本文件
```

## 学习目标

- 理解设计模式的基本原理
- 掌握常用设计模式的 C++ 实现
- 学会在实际项目中正确使用设计模式
- 避免过度使用设计模式

## 设计模式分类

### 创建型模式 (Creational Patterns)

处理对象的创建机制，提高创建过程的灵活性和复用性。

| 模式 | 描述 | 适用场景 |
|------|------|---------|
| 单例 | 确保一个类只有一个实例 | 配置管理器、日志系统 |
| 工厂方法 | 定义创建对象的接口，让子类决定 | 需要根据条件创建不同对象 |
| 抽象工厂 | 创建相关对象族 | 需要多个产品族 |
| 建造者 | 分步骤构建复杂对象 | 构建复杂对象 |

### 结构型模式 (Structural Patterns)

处理类或对象的组合，形成更大的结构。

| 模式 | 描述 | 适用场景 |
|------|------|---------|
| 适配器 | 将接口转换为客户端期望的接口 | 接口不兼容 |
| 装饰器 | 动态添加功能 | 需要灵活添加/删除功能 |
| 代理 | 控制对象的访问 | 延迟加载、权限控制 |
| 外观 | 为复杂子系统提供简单接口 | 简化复杂系统 |

### 行为型模式 (Behavioral Patterns)

处理对象之间的通信和职责分配。

| 模式 | 描述 | 适用场景 |
|------|------|---------|
| 观察者 | 一对多依赖，状态变化时通知 | 事件系统、UI 框架 |
| 策略 | 定义算法族，可互换 | 需要多种算法 |
| 命令 | 将请求封装为对象 | 撤销/重做、队列 |
| 状态 | 允许对象在内部状态改变时改变行为 | 游戏角色、工作流 |

## 使用原则

### SOLID 原则

1. **S** - 单一职责原则 (SRP)：一个类只负责一项职责
2. **O** - 开闭原则 (OCP)：对扩展开放，对修改关闭
3. **L** - 里氏替换原则 (LSP)：子类可以替换父类
4. **I** - 接口隔离原则 (ISP)：使用多个小接口而非大接口
5. **D** - 依赖倒置原则 (DIP)：依赖抽象而非具体实现

### 何时使用设计模式

**使用场景：**
- 代码中有重复的逻辑
- 需要灵活性和可扩展性
- 团队协作需要统一规范
- 复杂业务逻辑需要清晰结构

**避免场景：**
- 简单问题不需要复杂模式
- 过度设计增加维护成本
- 性能敏感的代码（虚拟调用开销）

## 示例代码

### 单例模式（现代 C++）

```cpp
class Singleton {
private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    void doSomething() {
        std::cout << "Singleton is working" << std::endl;
    }
};

// 使用
Singleton::getInstance().doSomething();
```

### 观察者模式

```cpp
class Observer {
public:
    virtual void update(int data) = 0;
    virtual ~Observer() = default;
};

class Subject {
private:
    vector<Observer*> observers;
    int data;

public:
    void attach(Observer* obs) { observers.push_back(obs); }
    void notify() {
        for (auto* obs : observers) {
            obs->update(data);
        }
    }
    void setData(int d) { data = d; notify(); }
};
```

## 编译示例

```bash
g++ -std=c++17 singleton.cpp -o singleton.exe
./singleton.exe
```

## 推荐学习顺序

1. 单例模式（最简单）
2. 工厂方法模式
3. 观察者模式
4. 策略模式
5. 其他模式

## 相关模块

- `02_smart_pointers` - 智能指针（用于实现单例等）
- `03_memory_management` - 内存管理
- `04_template_metaprogramming` - 模板编程（泛型设计模式）

## 参考资源

- 《设计模式：可复用面向对象软件的基础》- GoF
- 《Head First 设计模式》
- Refactoring.Guru - 设计模式在线参考
