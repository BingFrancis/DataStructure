# C++ 继承机制学习指南

## 📚 学习内容概述

本指南涵盖 C++ 继承机制的所有重要概念，从基础到高级，帮助你完全掌握面向对象编程的核心特性。

## 🎯 学习路径

### 第一部分：基础概念 (20%)

1. **public/protected/private 继承**
2. **基类和派生类**
3. **构造函数和析构函数调用顺序**

### 第二部分：多态 (30%)

1. **虚函数**
2. **纯虚函数**
3. **抽象类**
4. **override 关键字**
5. **动态绑定 vs 静态绑定**

### 第三部分：高级特性 (30%)

1. **虚析构函数** ⚠️ 非常重要！
2. **final 关键字**
3. **vtable（虚函数表）**
4. **RTTI（运行时类型识别）**

### 第四部分：多重继承 (20%)

1. **多重继承基础**
2. **菱形继承问题**
3. **虚继承**
4. **继承 vs 组合**

---

## 💡 核心知识点

### 1. 三种继承方式

| 继承方式 | 基类 public 成员 | 基类 protected 成员 | 基类 private 成员 |
|---------|----------------|------------------|------------------|
| public | public | protected | 不可访问 |
| protected | protected | protected | 不可访问 |
| private | private | private | 不可访问 |

```cpp
// public 继承（最常用）
class Dog : public Animal {};

// protected 继承（罕见）
class Dog : protected Animal {};

// private 继承（实现继承，不推荐）
class Dog : private Animal {};
```

### 2. 虚函数 - 多态的基础

```cpp
class Animal {
public:
    virtual void makeSound() {  // 虚函数
        cout << "动物发声" << endl;
    }
};

class Dog : public Animal {
public:
    void makeSound() override {  // 重写
        cout << "汪汪叫" << endl;
    }
};

Animal* p = new Dog();
p->makeSound();  // 输出：汪汪叫（多态）
```

**关键点**：
- 虚函数支持动态绑定（运行时决定调用哪个函数）
- 虚函数增加内存开销（每个对象多一个 vptr 指针）
- 虚函数调用比普通函数调用稍慢

### 3. 纯虚函数和抽象类

```cpp
class Shape {
public:
    virtual double area() const = 0;  // 纯虚函数
    virtual double perimeter() const = 0;
};

// Shape 是抽象类，不能实例化
// Shape shape;  // ❌ 错误

class Circle : public Shape {
public:
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

Circle circle(5.0);  // ✅ 正确
```

**用途**：
- 定义接口规范
- 强制子类实现某些方法
- 实现"编程到接口"的设计原则

### 4. 虚析构函数 - 必须掌握！⚠️

```cpp
class Base {
public:
    virtual ~Base() {  // ✅ 虚析构函数
        cout << "Base 析构" << endl;
    }
};

class Derived : public Base {
public:
    ~Derived() override {
        cout << "Derived 析构" << endl;
    }
};

Base* p = new Derived();
delete p;  // 输出：Derived 析构 -> Base 析构（正确）

// 如果不是虚析构函数：
// 只输出：Base 析构（Derived 析构不会调用！）
```

**规则**：只要基类有虚函数，析构函数就必须是虚函数！

### 5. override 和 final

```cpp
class Base {
public:
    virtual void foo() {}
    virtual void bar() final {}  // final：不能被重写
};

class Derived : public Base {
    void foo() override {}     // ✅ override：编译器检查是否正确重写
    // void bar() override {}  // ❌ 错误：bar 是 final 的
};
```

**好处**：
- `override`：防止拼写错误，编译器检查
- `final`：防止进一步继承或重写

### 6. 多重继承

```cpp
class Flyable {
public:
    virtual void fly() {}
};

class Swimmable {
public:
    virtual void swim() {}
};

class Duck : public Animal, public Flyable, public Swimmable {
public:
    void fly() override { cout << "飞" << endl; }
    void swim() override { cout << "游泳" << endl; }
};
```

**适用场景**：
- 实现多个接口
- 组合多个功能

**问题**：
- 名称冲突
- 菱形继承（虚继承解决）

### 7. 菱形继承和虚继承

```cpp
// 不使用虚继承（有问题）
class LivingBeing {
public:
    string name;
};
class Mammal : public LivingBeing {};
class Bird : public LivingBeing {};
class Platypus : public Mammal, public Bird {};

Platypus p;
p.name;  // ❌ 歧义：Mammal::name 还是 Bird::name？

// 使用虚继承（正确）
class Mammal : virtual public LivingBeing {};
class Bird : virtual public LivingBeing {};
class Platypus : public Mammal, public Bird {};

p.name;  // ✅ 正确：只有一个 LivingBeing 基类
```

### 8. vtable（虚函数表）

```cpp
class Base {
public:
    virtual void foo1() {}
    virtual void foo2() {}
    void bar() {}  // 非虚函数
};

class Derived : public Base {
public:
    void foo1() override {}
    void foo2() override {}
};
```

**内存布局**：
```
Base 对象：
+----------------+
| vptr (8 字节)  | -> vtable: [foo1, foo2]
| 成员变量...    |
+----------------+

Derived 对象：
+----------------+
| vptr (8 字节)  | -> vtable: [Derived::foo1, Derived::foo2]
| Base 成员...   |
| Derived 成员...|
+----------------+
```

**性能影响**：
- 每个有虚函数的对象多 8 字节（vptr）
- 虚函数调用多一次指针解引用
- 常见应用：~10-20% 性能损失（具体场景具体分析）

### 9. RTTI（运行时类型识别）

```cpp
Base* p = new Derived();

// dynamic_cast：安全向下转换
Derived* d = dynamic_cast<Derived*>(p);
if (d) {
    // 转换成功
} else {
    // 转换失败
}

// typeid：获取类型信息
cout << typeid(*p).name() << endl;  // 输出类型名称
```

**注意**：
- 只能用于有虚函数的类
- `dynamic_cast` 会有运行时开销
- 谨慎使用：考虑用虚函数代替

### 10. 继承 vs 组合

| 特性 | 继承 | 组合 |
|------|------|------|
| 关系 | is-a（是一个） | has-a（有一个） |
| 耦合度 | 强 | 弱 |
| 灵活性 | 低 | 高 |
| 多态 | 支持 | 需要接口 |
| 使用场景 | 需要多态 | 复用功能 |

**示例**：

```cpp
// ✅ 正确：继承
class Dog : public Animal {}  // Dog 是一种 Animal

// ✅ 正确：组合
class Car {
    Engine engine;  // Car 有一个 Engine
};

// ❌ 错误：误用继承
class Stack : public vector<int> {};  // Stack 不是 Vector
// 正确做法：
class Stack {
    vector<int> data;  // 组合
};
```

**设计原则**：
- 优先使用组合
- 需要多态时才用继承
- 避免过深的继承层次

---

## 🚀 实战应用

### 1. 图形绘制系统

```cpp
class Shape {
public:
    virtual void draw() = 0;
    virtual double area() = 0;
};

class Circle : public Shape {
public:
    void draw() override { /* ... */ }
    double area() override { /* ... */ }
};

class Rectangle : public Shape {
public:
    void draw() override { /* ... */ }
    double area() override { /* ... */ }
};

// 统一处理
vector<Shape*> shapes = {new Circle(), new Rectangle()};
for (auto shape : shapes) {
    shape->draw();
}
```

### 2. 视频流接口设计（你的项目）

```cpp
class BaseVideoStream {
public:
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool readFrame(uint8_t** data, int* size) = 0;
    virtual ~BaseVideoStream() = default;
};

class H264Stream : public BaseVideoStream {
    // 实现 H264 特定逻辑
};

class JPEGStream : public BaseVideoStream {
    // 实现 JPEG 特定逻辑
};

// 统一接口
BaseVideoStream* stream = new H264Stream();
stream->open();
stream->readFrame(&data, &size);
```

---

## 📊 性能对比

| 操作 | 普通函数 | 虚函数 | 性能损失 |
|------|---------|--------|----------|
| 函数调用 | 1 条指令 | 2-3 条指令 | ~10-20% |
| 对象大小 | 基础大小 | +8 字节 | 内存增加 |
| 编译优化 | 容易内联 | 难内联 | 优化受限 |

---

## 🎯 最佳实践

### ✅ 应该这样做

1. **基类析构函数总是虚函数**
```cpp
class Base {
public:
    virtual ~Base() = default;
};
```

2. **使用 override 明确表示重写**
```cpp
void foo() override {}
```

3. **优先使用组合**
```cpp
class Car {
    Engine engine;  // 组合
};
```

4. **纯虚函数定义接口**
```cpp
virtual void foo() = 0;
```

### ❌ 不要这样做

1. **不要忘记虚析构函数**
```cpp
class Base {
public:
    virtual ~Base() {}  // 必须是虚函数
};
```

2. **不要过度继承**
```cpp
// ❌ 继承层次太深
A -> B -> C -> D -> E -> F
```

3. **不要用继承复用代码**
```cpp
// ❌ 错误：Stack 继承 vector
class Stack : public vector<int> {};

// ✅ 正确：Stack 组合 vector
class Stack {
    vector<int> data;
};
```

4. **不要在构造函数中调用虚函数**
```cpp
class Base {
public:
    Base() {
        foo();  // ❌ 不会调用派生类的 foo
    }
    virtual void foo() {}
};
```

---

## 📝 编译和运行

```bash
# 编译
g++ -std=c++17 inheritance.cpp -o inheritance.exe

# 运行
./inheritance.exe
```

---

## 🔗 相关资源

- `inheritance.cpp` - 完整示例代码
- `02_smart_pointers/README.md` - 智能指针
- `03_memory_management/README.md` - 内存管理
- `04_template_metaprogramming/README.md` - 模板编程

---

## 📚 延伸阅读

- 《Effective C++》条款 07-12
- 《C++ Primer》第 15 章
- 《Inside the C++ Object Model》- 深入理解 C++ 对象模型
