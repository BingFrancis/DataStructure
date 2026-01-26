# 10 - Practical Projects（实战项目）

本模块通过实际项目综合运用前面所学知识。

## 目录结构

```
10_practical_projects/
├── 01_simple_database/      # 简单的键值数据库
├── 02_memory_pool/          # 高性能内存池
├── 03_thread_pool/          # 线程池实现
├── 04_logger/              # 日志系统
├── 05_http_server/          # 简单 HTTP 服务器
├── 06_json_parser/          # JSON 解析器
├── 07_cache_system/        # 缓存系统（LRU/LFU）
├── 08_event_loop/          # 事件循环（epoll/select）
└── README.md                # 本文件
```

## 项目概述

### 1. 简单的键值数据库
- 使用哈希表存储键值对
- 支持 CRUD 操作
- 持久化到文件
- 支持过期时间

### 2. 高性能内存池
- 固定大小内存块分配
- 对象池模式
- 避免内存碎片
- 性能对比测试

### 3. 线程池
- 任务队列
- 工作线程管理
- 支持不同优先级
- 线程安全保证

### 4. 日志系统
- 多级别日志（DEBUG, INFO, WARN, ERROR）
- 异步写入
- 日志文件轮转
- 线程安全

### 5. 简单 HTTP 服务器
- 解析 HTTP 请求
- 路由处理
- 静态文件服务
- 支持 GET/POST

### 6. JSON 解析器
- 词法分析
- 语法分析
- 构建 JSON 对象
- 支持 JSON 序列化

### 7. 缓存系统
- LRU (Least Recently Used) 缓存
- LFU (Least Frequently Used) 缓存
- 哈希表 + 双向链表
- 缓存淘汰策略

### 8. 事件循环
- 支持 select/epoll
- 定时器管理
- IO 多路复用
- 非阻塞 IO

## 学习目标

- 综合运用数据结构和算法
- 掌握系统编程基础
- 理解并发编程
- 学会性能优化
- 培养工程思维

## 编译和运行

### 编译单个项目
```bash
cd 10_practical_projects/01_simple_database
g++ -std=c++17 *.cpp -o database.exe
./database.exe
```

### 编译所有项目
```bash
cd 10_practical_projects
for dir in */; do
    cd "$dir"
    g++ -std=c++17 *.cpp -o project.exe
    cd ..
done
```

## 推荐学习顺序

1. 简单数据库（综合哈希表、文件 I/O）
2. 缓存系统（综合哈希表、链表、LRU）
3. 日志系统（综合多线程、队列）
4. 线程池（综合多线程、队列、任务调度）
5. 内存池（综合内存管理、对象池）
6. JSON 解析器（综合字符串处理、递归）
7. HTTP 服务器（综合网络编程、字符串处理）
8. 事件循环（综合 IO 多路复用、并发）

## 技术要点

### 内存管理
- 使用智能指针管理资源
- 对象池减少内存分配
- 自定义分配器优化性能

### 并发编程
- 互斥锁保证线程安全
- 条件变量同步线程
- 原子操作优化性能
- RAII 管理锁资源

### 性能优化
- 减少不必要的拷贝（使用移动语义）
- 预分配容器大小
- 使用引用传递大对象
- 缓存友好数据布局

### 错误处理
- 使用异常处理错误
- 错误码返回机制
- 资源清理保证（RAII）
- 防御性编程

## 相关模块

- `01_fundamentals` - 基础数据结构
- `03_memory_management` - 内存管理
- `06_multithreading` - 多线程编程
- `07_algorithms` - 算法实现

## 扩展方向

完成基础项目后，可以考虑：

1. **网络编程**
   - Socket 编程
   - TCP/UDP 协议
   - 高性能网络框架（如 libuv）

2. **分布式系统**
   - RPC 框架
   - 一致性哈希
   - 分布式锁

3. **数据库**
   - B+ 树索引
   - WAL 日志
   - MVCC 并发控制

4. **图形编程**
   - OpenGL/Vulkan
   - 游戏引擎
   - 渲染管线

5. **机器学习**
   - 数值计算库
   - 神经网络
   - 深度学习框架

## 参考资源

- 《高性能服务器编程》- 游双
- 《Linux 多线程服务端编程》- 陈硕
- 《TCP/IP 网络编程》- 尹圣雨
- GitHub 开源项目学习
