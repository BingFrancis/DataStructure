# 09_design_patterns - 设计模式

本模块学习 GoF 23 种设计模式中的经典模式，掌握可复用面向对象设计的基础。

## 📁 文件列表

```
09_design_patterns/
├── creational/                # 创建型模式
│   ├── singleton.cpp          # 单例模式
│   ├── factory_method.cpp     # 工厂方法模式
│   ├── abstract_factory.cpp   # 抽象工厂模式
│   └── builder.cpp            # 建造者模式
├── structural/                # 结构型模式
│   ├── adapter.cpp            # 适配器模式
│   ├── decorator.cpp          # 装饰器模式
│   ├── proxy.cpp              # 代理模式
│   └── facade.cpp             # 外观模式
└── behavioral/                # 行为型模式
    ├── observer.cpp           # 观察者模式
    ├── strategy.cpp           # 策略模式
    ├── command.cpp            # 命令模式
    └── state.cpp              # 状态模式
```

## 🎯 学习目标

- 理解设计模式的原则（SOLID）
- 掌握创建型、结构型、行为型模式
- 能根据场景选择合适的设计模式

## 📖 前置知识

- `01_fundamentals/inheritance.cpp` - 继承、多态、抽象类
- `04_template_metaprogramming` - 模板编程
- `02_smart_pointers` - 智能指针

## 📖 设计模式总览

### 创建型模式（4 个）

关注对象的**创建**过程，将创建和使用分离。

| 模式 | 意图 | 文件 |
|------|------|------|
| **单例** | 确保一个类只有一个实例 | `singleton.cpp` |
| **工厂方法** | 子类决定实例化哪个类 | `factory_method.cpp` |
| **抽象工厂** | 创建一系列相关对象 | `abstract_factory.cpp` |
| **建造者** | 分步构建复杂对象 | `builder.cpp` |

### 结构型模式（4 个）

关注类和对象的**组合**方式。

| 模式 | 意图 | 文件 |
|------|------|------|
| **适配器** | 转换不兼容接口 | `adapter.cpp` |
| **装饰器** | 动态添加职责 | `decorator.cpp` |
| **代理** | 控制对象访问 | `proxy.cpp` |
| **外观** | 统一简化接口 | `facade.cpp` |

### 行为型模式（4 个）

关注对象之间的**通信**和**职责分配**。

| 模式 | 意图 | 文件 |
|------|------|------|
| **观察者** | 状态变化自动通知 | `observer.cpp` |
| **策略** | 算法族可互相替换 | `strategy.cpp` |
| **命令** | 请求封装为对象 | `command.cpp` |
| **状态** | 状态改变时改变行为 | `state.cpp` |

## 📖 设计原则（SOLID）

| 原则 | 含义 |
|------|------|
| **S** - 单一职责 | 一个类只负责一项职责 |
| **O** - 开闭原则 | 对扩展开放，对修改关闭 |
| **L** - 里氏替换 | 子类可以替换父类 |
| **I** - 接口隔离 | 使用多个专门接口 |
| **D** - 依赖倒置 | 依赖抽象，不依赖具体 |

## 🛠️ 编译运行

```bash
# 创建型模式
g++ -std=c++17 creational/singleton.cpp -o singleton && ./singleton
g++ -std=c++17 creational/factory_method.cpp -o factory && ./factory

# 结构型模式
g++ -std=c++17 structural/decorator.cpp -o decorator && ./decorator

# 行为型模式
g++ -std=c++17 behavioral/observer.cpp -o observer && ./observer
g++ -std=c++17 behavioral/strategy.cpp -o strategy && ./strategy
```

## 📖 推荐学习顺序

1. 单例模式（最简单，最常用）
2. 工厂方法模式（创建型核心）
3. 策略模式（行为型核心）
4. 观察者模式（事件驱动基础）
5. 装饰器模式（结构型核心）
6. 其他模式

## ✅ 学习检查清单

- [ ] 理解 SOLID 设计原则
- [ ] 能实现线程安全的单例模式
- [ ] 理解工厂模式和抽象工厂的区别
- [ ] 能区分策略模式和状态模式
- [ ] 能根据场景选择合适的设计模式

## 🔗 相关模块

- `01_fundamentals/inheritance.cpp` - 继承机制
- `04_template_metaprogramming` - 模板与模式结合
- `08_advanced_cpp` - 现代 C++ 实现模式

---

**设计模式 = 面向对象设计的最佳实践！** 🚀
