/**
 * C++ 程序内存布局详解
 *
 * 学习目标：
 * 1. 理解C++程序的内存分区
 * 2. 了解各分区的特点和用途
 * 3. 掌握变量存储位置的决定因素
 */

#include <iostream>
#include <string>
#include <vector>
#include <thread>

// ==================== 全局/静态区 ====================
// 存储位置：全局/静态区（数据段）
// 生命周期：整个程序运行期间
// 特点：程序启动时分配，程序结束时释放

// 全局变量 - 已初始化数据段
int global_initialized = 100;

// 全局变量 - 未初始化数据段（BSS段）
int global_uninitialized;

// 静态变量 - 全局/静态区
static int static_variable = 200;

// 常量 - 只读数据段（.rodata）
const int const_global = 300;

// 字符串字面量 - 只读数据段
const char* string_literal = "Hello World";

// ==================== 函数体外的代码区 ====================
// 存储位置：代码区（.text）
// 特点：只读，存储程序指令

void demonstrateMemoryLayout() {
    std::cout << "=== C++ 程序内存布局演示 ===\n\n";

    // ==================== 栈区 ====================
    // 存储位置：栈
    // 生命周期：函数调用开始到结束
    // 特点：自动分配释放，LIFO（后进先出）
    // 大小限制：通常几MB（Windows 1-8MB，Linux 8-10MB）

    int stack_variable = 10;           // 栈变量
    int* stack_ptr = nullptr;          // 栈上的指针
    std::string stack_string = "test"; // 栈上的对象（内部可能堆分配）

    std::cout << "=== 栈区变量 ===\n";
    std::cout << "栈变量地址: " << &stack_variable << "\n";
    std::cout << "栈指针地址: " << &stack_ptr << "\n";
    std::cout << "栈字符串地址: " << &stack_string << "\n";
    std::cout << "栈字符串内容地址: " << (void*)stack_string.c_str() << "\n\n";

    // ==================== 堆区 ====================
    // 存储位置：堆（自由存储区）
    // 生命周期：手动管理（new/delete 或智能指针）
    // 特点：动态分配，大小灵活，但效率较低
    // 大小限制：虚拟内存大小

    int* heap_variable = new int(20);           // 堆分配
    int* heap_array = new int[100];             // 堆数组
    std::string* heap_string = new std::string("heap"); // 堆对象

    std::cout << "=== 堆区变量 ===\n";
    std::cout << "堆变量地址: " << heap_variable << "\n";
    std::cout << "堆数组地址: " << heap_array << "\n";
    std::cout << "堆对象地址: " << heap_string << "\n\n";



    

    // 演示栈溢出
    std::cout << "=== 栈溢出演示 ===\n";
    std::cout << "注意：不要在栈上分配过大的数组！\n";
    std::cout << "例如：int large_array[10000000]; 会导致栈溢出\n";
    std::cout << "解决：使用堆分配：int* large_array = new int[10000000];\n\n";

    // 演示堆泄漏
    std::cout << "=== 内存泄漏演示 ===\n";
    std::cout << "忘记 delete 会导致内存泄漏！\n";
    std::cout << "建议：使用智能指针自动管理内存\n\n";

    // 清理堆内存
    delete heap_variable;
    delete[] heap_array;
    delete heap_string;

    // ==================== 静态局部变量 ====================
    // 存储位置：全局/静态区
    // 生命周期：整个程序运行期间
    // 特点：只初始化一次

    static int static_local = 0;
    static_local++;

    std::cout << "=== 静态局部变量 ===\n";
    std::cout << "静态局部变量地址: " << &static_local << "\n";
    std::cout << "静态局部变量值: " << static_local << "\n\n";
}

void showMemoryAddresses() {
    int local_var = 42;

    std::cout << "=== 各区域地址对比 ===\n";
    std::cout << "代码区（函数地址）:    " << (void*)showMemoryAddresses << "\n";
    std::cout << "全局变量（已初始化）:   " << &global_initialized << "\n";
    std::cout << "全局变量（未初始化）:   " << &global_uninitialized << "\n";
    std::cout << "静态变量:             " << &static_variable << "\n";
    std::cout << "常量:                 " << &const_global << "\n";
    std::cout << "字符串字面量:          " << (void*)string_literal << "\n";
    std::cout << "栈变量:               " << &local_var << "\n";

    // 堆地址
    int* heap_var = new int(99);
    std::cout << "堆变量:                " << heap_var << "\n";
    delete heap_var;
}

// ==================== 内存对齐演示 ====================
struct UnalignedStruct {
    char a;    // 1字节
    int b;     // 4字节
    char c;    // 1字节
};

struct AlignedStruct {
    int b;     // 4字节
    char a;    // 1字节
    char c;    // 1字节
    // 自动填充2字节
};

void demonstrateMemoryAlignment() {
    std::cout << "\n=== 内存对齐演示 ===\n";
    std::cout << "未对齐结构体大小: " << sizeof(UnalignedStruct) << " 字节\n";
    std::cout << "对齐结构体大小:   " << sizeof(AlignedStruct) << " 字节\n";
    std::cout << "\n内存对齐原则：\n";
    std::cout << "1. CPU按特定边界读取内存（通常是4或8字节）\n";
    std::cout << "2. 未对齐会导致多次内存访问，降低性能\n";
    std::cout << "3. 编译器会自动插入填充字节对齐\n";
    std::cout << "4. 可以按大小降序排列成员减少填充\n";
}

// ==================== 线程局部存储演示 ====================
thread_local int thread_local_var = 0;

void threadFunction(int id) {
    thread_local_var = id;
    std::cout << "线程 " << id << " 的 thread_local_var: " << thread_local_var
              << ", 地址: " << &thread_local_var << "\n";
}

void demonstrateThreadLocalStorage() {
    std::cout << "\n=== 线程局部存储（TLS）演示 ===\n";
    std::cout << "thread_local 变量每个线程有独立副本\n";

    std::thread t1(threadFunction, 1);
    std::thread t2(threadFunction, 2);
    std::thread t3(threadFunction, 3);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "主线程的 thread_local_var: " << thread_local_var
              << ", 地址: " << &thread_local_var << "\n";
}

// ==================== 内存分配性能对比 =================///
#include <chrono>

void benchmarkMemoryAllocation() {
    std::cout << "\n=== 内存分配性能对比 ===\n";

    const int iterations = 1000000;

    // 栈分配测试
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        int data[100];  // 栈分配
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto stack_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "栈分配 " << iterations << " 次: " << stack_duration.count() << " 微秒\n";

    // 堆分配测试
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        int* data = new int[100];  // 堆分配
        delete[] data;
    }
    end = std::chrono::high_resolution_clock::now();
    auto heap_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "堆分配 " << iterations << " 次: " << heap_duration.count() << " 微秒\n";

    std::cout << "\n结论：\n";
    std::cout << "1. 栈分配比堆分配快 10-100 倍\n";
    std::cout << "2. 栈分配只需移动栈指针（加/减指令）\n";
    std::cout << "3. 堆分配需要复杂的内存管理算法\n";
    std::cout << "4. 频繁分配释放应优先使用栈或内存池\n";
}

// ==================== 缓存友好设计演示 =================///
void demonstrateCacheFriendlyDesign() {
    std::cout << "\n=== 缓存友好设计演示 ===\n";

    const int size = 10000;

    // 二维数组
    int array[size][size];

    std::cout << "数组访问顺序对性能的影响：\n";

    // 顺序访问（缓存友好）
    auto start = std::chrono::high_resolution_clock::now();
    long sum = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            sum += array[i][j];  // 连续内存访问
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto sequential_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "顺序访问时间: " << sequential_time.count() << " 毫秒\n";

    // 跨步访问（缓存不友好）
    start = std::chrono::high_resolution_clock::now();
    sum = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            sum += array[j][i];  // 跨越缓存行访问
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto strided_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "跨步访问时间: " << strided_time.count() << " 毫秒\n";

    std::cout << "\n性能差异: " << (double)strided_time.count() / sequential_time.count() << " 倍\n";
    std::cout << "\n原因：\n";
    std::cout << "1. CPU缓存按块（缓存行，通常64字节）加载数据\n";
    std::cout << "2. 顺序访问可以利用缓存行的预取\n";
    std::cout << "3. 跨步访问导致频繁缓存未命中\n";
    std::cout << "4. 缓存命中时只需1-3个时钟周期\n";
    std::cout << "5. 缓存未命中需要从内存加载（100+个时钟周期）\n";
}

int main() {
    demonstrateMemoryLayout();
    showMemoryAddresses();
    demonstrateMemoryAlignment();
    demonstrateThreadLocalStorage();
    benchmarkMemoryAllocation();
    demonstrateCacheFriendlyDesign();

    std::cout << "\n=== 内存配置关键要点总结 ===\n";
    std::cout << "1. 栈：快速、自动管理，但大小有限\n";
    std::cout << "2. 堆：灵活、大容量，但分配慢且需手动管理\n";
    std::cout << "3. 全局/静态区：整个程序生命周期，只读数据放这里\n";
    std::cout << "4. 代码区：只读，存储程序指令\n";
    std::cout << "5. 合理使用内存对齐提升性能\n";
    std::cout << "6. 注意缓存友好的数据访问模式\n";
    std::cout << "7. 优先使用栈或智能指针避免内存泄漏\n";

    return 0;
}
