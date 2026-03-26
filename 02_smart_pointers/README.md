# 02_smart_pointers - 智能指针

本模块学习现代 C++ 智能指针，掌握资源管理（RAII）的核心思想。

## 📁 文件列表

| 文件 | 描述 | 难度 |
|------|------|------|
| `smart_pointers.cpp` | unique_ptr / shared_ptr / weak_ptr 详解 | ⭐⭐⭐ |
| `modern_cpp_project.cpp` | 现代项目中的智能指针应用 | ⭐⭐⭐ |

## 🎯 学习目标

- 理解 RAII（资源获取即初始化）原则
- 掌握三种智能指针的使用场景
- 避免 new/delete 手动管理内存
- 解决循环引用问题

## 📖 核心概念

### 三种智能指针对比

| 类型 | 所有权 | 引用计数 | 使用场景 |
|------|--------|----------|----------|
| `unique_ptr` | 独占 | 无 | 单一所有者 |
| `shared_ptr` | 共享 | 有 | 多个所有者 |
| `weak_ptr` | 不持有 | 无 | 观察 shared_ptr |

### 1. unique_ptr - 独占所有权

```cpp
// 创建
auto ptr = make_unique<int>(42);
auto arr = make_unique<int[]>(10);

// 不能复制，只能移动
// auto ptr2 = ptr;        // ❌ 编译错误
auto ptr2 = move(ptr);      // ✅ 移动所有权

// 自定义删除器
auto deleter = [](FILE* f) { if(f) fclose(f); };
unique_ptr<FILE, decltype(deleter)> file(fopen("test.txt", "r"), deleter);
```

### 2. shared_ptr - 共享所有权

```cpp
// 创建（推荐使用 make_shared）
auto sp1 = make_shared<int>(42);
auto sp2 = sp1;  // 引用计数 +1

cout << sp1.use_count() << endl;  // 2

// 自定义删除器
auto deleter = [](int* p) { delete[] p; };
shared_ptr<int> sp(new int[10], deleter);
```

### 3. weak_ptr - 解决循环引用

```cpp
struct Node {
    shared_ptr<Node> next;
    weak_ptr<Node> prev;  // 使用 weak_ptr 打破循环
};

auto a = make_shared<Node>();
auto b = make_shared<Node>();
a->next = b;
b->prev = a;  // 不会导致内存泄漏
```

## ⚠️ 注意事项

### 1. 优先使用 make_unique / make_shared

```cpp
// ✅ 推荐（性能更好，异常安全）
auto ptr = make_shared<MyClass>(arg1, arg2);

// ❌ 不推荐
auto ptr = shared_ptr<MyClass>(new MyClass(arg1, arg2));
```

### 2. 避免循环引用

```cpp
// ❌ 错误：循环引用导致内存泄漏
struct B;
struct A {
    shared_ptr<B> b;
};
struct B {
    shared_ptr<A> a;  // 循环引用！
};

// ✅ 正确：一方使用 weak_ptr
struct B {
    weak_ptr<A> a;  // 打破循环
};
```

### 3. 不要用裸指针管理智能指针的资源

```cpp
// ❌ 危险
int* raw = new int(42);
shared_ptr<int> sp1(raw);
shared_ptr<int> sp2(raw);  // double free!

// ✅ 正确
auto sp1 = make_shared<int>(42);
```

## 🛠️ 编译运行

```bash
g++ -std=c++17 smart_pointers.cpp -o smart_pointers && ./smart_pointers
g++ -std=c++17 modern_cpp_project.cpp -o modern_cpp_project && ./modern_cpp_project
```

## ✅ 学习检查清单

- [ ] 理解 RAII 原则
- [ ] 能正确使用 unique_ptr
- [ ] 能正确使用 shared_ptr 和 weak_ptr
- [ ] 知道什么时候用哪种智能指针
- [ ] 理解引用计数的工作原理
- [ ] 能解决循环引用问题
- [ ] 了解自定义删除器的用法

## 🔗 相关模块

- `01_fundamentals` - 基础数据结构
- `03_memory_management` - 内存管理进阶

---

**掌握智能指针 = 告别内存泄漏！** 🚀
