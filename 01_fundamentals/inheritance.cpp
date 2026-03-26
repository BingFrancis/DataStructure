// ============================================================================
// 09 - Design Patterns
// ============================================================================
// 1. 基础继承 (Basic Inheritance)
// ============================================================================
// 继承是面向对象编程的核心特性之一，允许一个类（子类/派生类）
// 继承另一个类（父类/基类）的成员变量和方法。
//
// 本文件涵盖：
// - public/protected/private 继承
// - 构造函数和析构函数调用顺序
// - 虚函数和多态
// - 纯虚函数和抽象类
// - 多重继承和虚继承
// - override 和 final 关键字
// - 运行时类型识别 (RTTI)
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// =============================================================================
// 1. 基础继承示例
// =============================================================================

// 基类 (父类)
class Animal {
private:
    string name;
    int age;

protected:
    // protected: 子类可以访问，外部不能
    int weight;

public:
    Animal(const string& n, int a, int w) 
        : name(n), age(a), weight(w) {
        cout << "Animal 构造函数: " << name << endl;
    }

    virtual ~Animal() {  // 虚析构函数！非常重要！
        cout << "Animal 析构函数: " << name << endl;
    }

    // 普通成员函数
    void eat() {
        cout << name << " 正在吃东西" << endl;
    }

    void sleep() {
        cout << name << " 正在睡觉" << endl;
    }

    // 虚函数：子类可以覆盖（重写）
    virtual void makeSound() {
        cout << name << " 发出声音" << endl;
    }

    // 访问器
    string getName() const { return name; }
    int getAge() const { return age; }
    int getWeight() const { return weight; }
};

// 派生类 (子类)
class Dog : public Animal {
private:
    string breed;  // 品种

public:
    Dog(const string& n, int a, int w, const string& b)
        : Animal(n, a, w), breed(b) {
        cout << "Dog 构造函数" << endl;
    }

    ~Dog() override {
        cout << "Dog 析构函数" << endl;
    }

    // 重写虚函数
    void makeSound() override {
        cout << getName() << " 汪汪叫！" << endl;
    }

    // 新增方法
    void fetch() {
        cout << getName() << " 去捡球了" << endl;
    }

    string getBreed() const { return breed; }
};

// 另一个派生类
class Cat : public Animal {
private:
    bool isIndoor;  // 是否室内猫

public:
    Cat(const string& n, int a, int w, bool indoor)
        : Animal(n, a, w), isIndoor(indoor) {
        cout << "Cat 构造函数" << endl;
    }

    ~Cat() override {
        cout << "Cat 析构函数" << endl;
    }

    void makeSound() override {
        cout << getName() << " 喵喵叫~" << endl;
    }

    void scratch() {
        cout << getName() << " 在抓挠沙发" << endl;
    }
};

void testBasicInheritance() {
    cout << "\n========== 基础继承测试 ==========" << endl;

    Dog dog("旺财", 3, 15, "金毛");
    Cat cat("咪咪", 2, 5, true);

    // 访问基类方法
    dog.eat();
    cat.sleep();

    // 访问子类特有方法
    dog.fetch();
    cat.scratch();

    // 多态：基类指针调用子类方法
    Animal* animals[] = {&dog, &cat};
    for (auto animal : animals) {
        animal->makeSound();  // 调用各自的重写版本
    }
}

// =============================================================================
// 2. 继承访问权限
// =============================================================================

class Base {
public:
    int publicVar = 1;
protected:
    int protectedVar = 2;
private:
    int privateVar = 3;
};

// public 继承：保持访问级别
class PublicDerived : public Base {
public:
    void accessMembers() {
        publicVar = 10;      // ✅ 可以访问
        protectedVar = 20;   // ✅ 可以访问
        // privateVar = 30;  // ❌ 不能访问
    }
};

// protected 继承：public 和 protected 变成 protected
class ProtectedDerived : protected Base {
public:
    void accessMembers() {
        publicVar = 10;      // ✅ 可以访问（但在子类中是 protected）
        protectedVar = 20;   // ✅ 可以访问
        // privateVar = 30;  // ❌ 不能访问
    }
};

// private 继承：所有都变成 private
class PrivateDerived : private Base {
public:
    void accessMembers() {
        publicVar = 10;      // ✅ 可以访问（但变成 private）
        protectedVar = 20;   // ✅ 可以访问（但变成 private）
        // privateVar = 30;  // ❌ 不能访问
    }
};

void testInheritanceAccess() {
    cout << "\n========== 继承访问权限测试 ==========" << endl;

    PublicDerived pubD;
    pubD.publicVar = 100;      // ✅ public 继承：仍然是 public
    // pubD.protectedVar = 200; // ❌ 外部不能访问 protected

    ProtectedDerived protD;
    // protD.publicVar = 100;  // ❌ protected 继承：变成 protected

    PrivateDerived privD;
    // privD.publicVar = 100;  // ❌ private 继承：变成 private

    cout << "publicVar (PublicDerived): " << pubD.publicVar << endl;
}

// =============================================================================
// 3. 纯虚函数和抽象类
// =============================================================================

// 抽象类：包含纯虚函数，不能实例化
class Shape {
public:
    // 纯虚函数：没有实现，子类必须实现
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // 普通虚函数：有默认实现
    virtual void display() const {
        cout << "这是一个形状" << endl;
    }

    virtual ~Shape() = default;  // 虚析构函数
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double area() const override {
        return 3.14159 * radius * radius;
    }

    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }

    void display() const override {
        cout << "圆形，半径: " << radius << endl;
    }
};

class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override {
        return width * height;
    }

    double perimeter() const override {
        return 2 * (width + height);
    }

    void display() const override {
        cout << "矩形，宽: " << width << "，高: " << height << endl;
    }
};

void testAbstractClass() {
    cout << "\n========== 抽象类和纯虚函数测试 ==========" << endl;

    // Shape shape;  // ❌ 不能实例化抽象类

    Circle circle(5.0);
    Rectangle rect(4.0, 3.0);

    vector<Shape*> shapes = {&circle, &rect};

    for (auto shape : shapes) {
        shape->display();
        cout << "面积: " << shape->area() << endl;
        cout << "周长: " << shape->perimeter() << endl;
        cout << "---" << endl;
    }
}

// =============================================================================
// 4. final 关键字
// =============================================================================

class FinalClass final {  // final 类：不能被继承
public:
    void method() {}
};

// ❌ 编译错误：不能继承 final 类
// class DerivedFromFinal : public FinalClass {};

class BaseWithFinal {
public:
    virtual void normalMethod() {}
    virtual void finalMethod() final {}  // final 方法：不能被重写
};

class DerivedFromBase : public BaseWithFinal {
public:
    void normalMethod() override {}  // ✅ 可以重写
    // void finalMethod() override {}  // ❌ 不能重写 final 方法
};

// =============================================================================
// 5. override 关键字
// =============================================================================

class BaseForOverride {
public:
    virtual void foo() {}
    virtual void bar(int x) {}
};

class GoodDerived : public BaseForOverride {
public:
    void foo() override {  // ✅ 正确重写
        cout << "重写了 foo()" << endl;
    }

    // void bar(double x) override {}  // ❌ 编译错误：签名不匹配
};

// =============================================================================
// 6. 多重继承
// =============================================================================

class Flyable {
public:
    virtual void fly() {
        cout << "在空中飞翔" << endl;
    }
    virtual ~Flyable() = default;
};

class Swimmable {
public:
    virtual void swim() {
        cout << "在水中游泳" << endl;
    }
    virtual ~Swimmable() = default;
};

// 多重继承
class Duck : public Animal, public Flyable, public Swimmable {
public:
    Duck(const string& n, int a, int w)
        : Animal(n, a, w) {
        cout << "Duck 构造函数" << endl;
    }

    void makeSound() override {
        cout << getName() << " 嘎嘎叫" << endl;
    }

    void fly() override {
        cout << getName() << " 扑腾翅膀飞翔" << endl;
    }

    void swim() override {
        cout << getName() << " 在水面游泳" << endl;
    }
};

void testMultipleInheritance() {
    cout << "\n========== 多重继承测试 ==========" << endl;

    Duck duck("唐老鸭", 5, 2);
    duck.makeSound();
    duck.fly();
    duck.swim();
}

// =============================================================================
// 7. 虚继承（解决菱形继承问题）
// =============================================================================

class LivingBeing {
public:
    string name = "生物";

    virtual void breathe() {
        cout << "呼吸" << endl;
    }

    virtual ~LivingBeing() = default;
};

// 虚继承：虚基类
class Mammal : virtual public LivingBeing {
public:
    void giveBirth() {
        cout << "哺乳" << endl;
    }
};

class Bird : virtual public LivingBeing {
public:
    void layEggs() {
        cout << "产卵" << endl;
    }
};

// 菱形继承：只有一个 LivingBeing 基类实例
class Platypus : public Mammal, public Bird {
public:
    void displayInfo() {
        cout << "鸭嘴兽: " << name << endl;  // ✅ 没有 name 歧义
        breathe();
        giveBirth();
        layEggs();
    }
};

// 如果不用虚继承，会有问题：
class A {
public:
    int x = 1;
};

class B1 : public A {};
class B2 : public A {};

class C : public B1, public B2 {
public:
    void show() {
        // cout << x;  // ❌ 歧义：B1::x 还是 B2::x？
        cout << B1::x << endl;  // ✅ 必须明确指定
    }
};

void testVirtualInheritance() {
    cout << "\n========== 虚继承测试 ==========" << endl;

    Platypus platypus;
    platypus.displayInfo();

    cout << "\n菱形继承问题（非虚继承）:" << endl;
    C c;
    c.show();
}

// =============================================================================
// 8. 虚函数表（vtable）和动态绑定
// =============================================================================

class BaseWithVTable {
public:
    int x = 10;
    virtual void foo1() { cout << "Base::foo1" << endl; }
    virtual void foo2() { cout << "Base::foo2" << endl; }
    void nonVirtual() { cout << "Base::nonVirtual" << endl; }
};

class DerivedWithVTable : public BaseWithVTable {
public:
    int y = 20;
    void foo1() override { cout << "Derived::foo1" << endl; }
    void foo2() override { cout << "Derived::foo2" << endl; }
};

void testVTable() {
    cout << "\n========== 虚函数表和动态绑定测试 ==========" << endl;

    BaseWithVTable base;
    DerivedWithVTable derived;

    BaseWithVTable* ptr = &derived;

    // 动态绑定：根据实际对象类型调用
    ptr->foo1();        // 调用 Derived::foo1
    ptr->foo2();        // 调用 Derived::foo2

    // 静态绑定：根据指针类型调用
    ptr->nonVirtual();  // 调用 Base::nonVirtual

    cout << "BaseWithVTable 大小: " << sizeof(BaseWithVTable) << endl;
    cout << "DerivedWithVTable 大小: " << sizeof(DerivedWithVTable) << endl;
    cout << "（多出的 8 字节是虚函数表指针 vptr）" << endl;
}

// =============================================================================
// 9. 运行时类型识别 (RTTI)
// =============================================================================

class BaseRTTI {
public:
    virtual ~BaseRTTI() = default;
};

class DerivedRTTI1 : public BaseRTTI {};
class DerivedRTTI2 : public BaseRTTI {};

void testRTTI() {
    cout << "\n========== RTTI 测试 ==========" << endl;

    BaseRTTI* ptr1 = new DerivedRTTI1();
    BaseRTTI* ptr2 = new DerivedRTTI2();

    // dynamic_cast：安全向下转换
    DerivedRTTI1* d1 = dynamic_cast<DerivedRTTI1*>(ptr1);
    if (d1) {
        cout << "ptr1 可以转换为 DerivedRTTI1" << endl;
    }

    DerivedRTTI1* d2 = dynamic_cast<DerivedRTTI1*>(ptr2);
    if (!d2) {
        cout << "ptr2 不能转换为 DerivedRTTI1" << endl;
    }

    // typeid：获取类型信息
    cout << "ptr1 类型: " << typeid(*ptr1).name() << endl;
    cout << "ptr2 类型: " << typeid(*ptr2).name() << endl;

    delete ptr1;
    delete ptr2;
}

// =============================================================================
// 10. 实战案例：图形绘制系统
// =============================================================================

class Graphic {
public:
    virtual void draw() = 0;
    virtual void move(int dx, int dy) = 0;
    virtual ~Graphic() = default;
};

class Point : public Graphic {
private:
    int x, y;

public:
    Point(int x, int y) : x(x), y(y) {}

    void draw() override {
        cout << "绘制点 (" << x << ", " << y << ")" << endl;
    }

    void move(int dx, int dy) override {
        x += dx;
        y += dy;
    }
};

class Line : public Graphic {
private:
    Point start;
    Point end;

public:
    Line(const Point& s, const Point& e) : start(s), end(e) {}

    void draw() override {
        cout << "绘制线段" << endl;
        start.draw();
        end.draw();
    }

    void move(int dx, int dy) override {
        start.move(dx, dy);
        end.move(dx, dy);
    }
};

class ShapeWithColor : public Shape {
private:
    string color;

public:
    ShapeWithColor(const string& c) : color(c) {}

    string getColor() const { return color; }

    void setColor(const string& c) { color = c; }
};

class ColoredCircle : public Circle, public ShapeWithColor {
public:
    ColoredCircle(double r, const string& color)
        : Circle(r), ShapeWithColor(color) {}

    void display() const override {
        cout << getColor() << " 的圆形，半径: " 
             << /* 注意：这里访问 Circle 的成员，可能有歧义 */
             /* 实际需要 Circle::radius，但 radius 是 private */
             /* 这里只是为了演示多重继承组合 */
             "圆形" << endl;
    }
};

void testGraphicSystem() {
    cout << "\n========== 图形系统实战 ==========" << endl;

    vector<unique_ptr<Graphic>> graphics;
    graphics.push_back(make_unique<Point>(10, 20));
    graphics.push_back(make_unique<Line>(Point(0, 0), Point(100, 100)));

    for (auto& g : graphics) {
        g->draw();
        g->move(5, 5);
        cout << "--- 移动后 ---" << endl;
        g->draw();
    }
}

// =============================================================================
// 11. 构造函数和析构函数调用顺序
// =============================================================================

class GrandParent {
public:
    GrandParent() { cout << "GrandParent 构造函数" << endl; }
    ~GrandParent() { cout << "GrandParent 析构函数" << endl; }
};

class Parent : public GrandParent {
public:
    Parent() { cout << "Parent 构造函数" << endl; }
    ~Parent() { cout << "Parent 析构函数" << endl; }
};

class Child : public Parent {
public:
    Child() { cout << "Child 构造函数" << endl; }
    ~Child() { cout << "Child 析构函数" << endl; }
};

void testConstructorDestructorOrder() {
    cout << "\n========== 构造/析构顺序测试 ==========" << endl;

    {
        Child child;
        cout << "--- 作用域结束 ---" << endl;
    }
}

// =============================================================================
// 12. 委托构造和成员初始化
// =============================================================================

class BaseWithMembers {
protected:
    int a;
    double b;
    string c;

public:
    // 委托构造函数
    BaseWithMembers(int x) : BaseWithMembers(x, 0.0, "") {}

    BaseWithMembers(int x, double y, const string& z)
        : a(x), b(y), c(z) {
        cout << "BaseWithMembers 构造: a=" << a << ", b=" << b << endl;
    }
};

class DerivedWithMembers : public BaseWithMembers {
private:
    int d;

public:
    // 成员初始化列表 + 基类构造
    DerivedWithMembers(int x, int y)
        : BaseWithMembers(x), d(y) {
        cout << "DerivedWithMembers 构造: d=" << d << endl;
    }
};

// =============================================================================
// 主函数
// =============================================================================

int main() {
    cout << "========================================" << endl;
    cout << "  C++ 继承机制全面学习" << endl;
    cout << "========================================" << endl;

    testBasicInheritance();
    testInheritanceAccess();
    testAbstractClass();
    testMultipleInheritance();
    testVirtualInheritance();
    testVTable();
    testRTTI();
    testGraphicSystem();
    testConstructorDestructorOrder();

    cout << "\n========================================" << endl;
    cout << "  所有测试完成！" << endl;
    cout << "========================================" << endl;

    return 0;
}

// ============================================================================
// 编译和运行
// ============================================================================
// g++ -std=c++17 inheritance.cpp -o inheritance.exe
// ./inheritance.exe
// ============================================================================
