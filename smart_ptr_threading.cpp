#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <queue>
#include <condition_variable>
#include <future>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <set>

using namespace std;

// 资源管理器，展示智能指针与线程安全的结合
class ThreadSafeResourceManager {
private:
    // 使用shared_ptr管理资源
    struct Resource {
        string name;
        int data;
        mutex resourceMutex;  // 每个资源有自己的互斥锁
        
        Resource(const string& n, int d) : name(n), data(d) {}
    };
    
    // 使用shared_ptr管理资源，weak_ptr用于观察
    unordered_map<string, shared_ptr<Resource>> resources;
    mutable mutex resourcesMutex;
    
    // 使用观察者模式追踪资源引用
    unordered_map<string, vector<weak_ptr<Resource>>> observers;
    mutable mutex observersMutex;
    
public:
    // 创建资源并返回shared_ptr
    shared_ptr<Resource> createResource(const string& name, int data) {
        lock_guard<mutex> lock(resourcesMutex);
        
        auto resource = make_shared<Resource>(name, data);
        resources[name] = resource;
        
        cout << "创建资源: " << name << endl;
        return resource;
    }
    
    // 获取资源
    shared_ptr<Resource> getResource(const string& name) {
        lock_guard<mutex> lock(resourcesMutex);
        
        auto it = resources.find(name);
        if (it != resources.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    // 注册观察者（使用weak_ptr避免循环引用）
    void addObserver(const string& resourceName, weak_ptr<Resource> observer) {
        lock_guard<mutex> lock(observersMutex);
        observers[resourceName].push_back(observer);
    }
    
    // 通知所有观察者资源变化
    void notifyObservers(const string& resourceName) {
        lock_guard<mutex> lock(observersMutex);
        
        auto it = observers.find(resourceName);
        if (it != observers.end()) {
            // 清理失效的weak_ptr
            auto& observersList = it->second;
            auto newEnd = remove_if(observersList.begin(), observersList.end(), 
                [](const weak_ptr<Resource>& weakPtr) {
                    return weakPtr.expired();
                });
            observersList.erase(newEnd, observersList.end());
            
            // 通知剩余的观察者
            for (auto& weakPtr : observersList) {
                if (auto sharedPtr = weakPtr.lock()) {
                    cout << "通知观察者: 资源 " << resourceName << " 已变化" << endl;
                }
            }
        }
    }
    
    // 线程安全地修改资源
    bool modifyResource(const string& name, int newData) {
        auto resource = getResource(name);
        if (!resource) {
            return false;
        }
        
        // 锁定特定资源而不是整个管理器
        lock_guard<mutex> resourceLock(resource->resourceMutex);
        resource->data = newData;
        
        cout << "修改资源: " << name << "，新值: " << newData << endl;
        
        // 通知观察者
        notifyObservers(name);
        
        return true;
    }
    
    // 列出所有活跃资源
    void listActiveResources() {
        lock_guard<mutex> lock(resourcesMutex);
        
        cout << "活跃资源列表:" << endl;
        for (const auto& pair : resources) {
            auto& resource = pair.second;
            if (resource.use_count() > 1) {  // 除了map中的引用外还有其他引用
                cout << "  " << resource->name << " (引用计数: " << resource.use_count() << ")" << endl;
            }
        }
    }
};

// 共享数据结构，展示atomic与智能指针的结合
template<typename T>
class ThreadSafeSharedPtr {
private:
    shared_ptr<T> ptr;
    mutable mutex ptrMutex;
    
public:
    ThreadSafeSharedPtr() = default;
    
    ThreadSafeSharedPtr(const T& value) : ptr(make_shared<T>(value)) {}
    
    ThreadSafeSharedPtr(shared_ptr<T> sharedPtr) : ptr(move(sharedPtr)) {}
    
    // 线程安全访问
    shared_ptr<T> get() const {
        lock_guard<mutex> lock(ptrMutex);
        return ptr;
    }
    
    // 线程安全设置
    void set(const T& value) {
        lock_guard<mutex> lock(ptrMutex);
        ptr = make_shared<T>(value);
    }
    
    // 线程安全设置shared_ptr
    void setSharedPtr(shared_ptr<T> newPtr) {
        lock_guard<mutex> lock(ptrMutex);
        ptr = move(newPtr);
    }
    
    // 比较操作
    bool operator==(const ThreadSafeSharedPtr& other) const {
        lock_guard<mutex> lock1(ptrMutex);
        lock_guard<mutex> lock2(other.ptrMutex);
        return ptr == other.ptr;
    }
    
    bool operator!=(const ThreadSafeSharedPtr& other) const {
        return !(*this == other);
    }
};

// 异步任务队列，使用智能指针管理任务
template<typename T>
class AsyncTaskQueue {
private:
    // 任务包装器，包含数据和函数
    struct AsyncTask {
        T data;
        function<void(const T&)> task;
        promise<void> completionPromise;
        shared_ptr<atomic<bool>> cancelledFlag;
        
        AsyncTask(const T& d, function<void(const T&)> t) 
            : data(d), task(move(t)), cancelledFlag(make_shared<atomic<bool>>(false)) {}
        
        void execute() {
            if (!*cancelledFlag) {
                task(data);
                completionPromise.set_value();
            }
        }
        
        void cancel() {
            *cancelledFlag = true;
            completionPromise.set_value();
        }
    };
    
    queue<shared_ptr<AsyncTask>> taskQueue;
    mutable mutex queueMutex;
    condition_variable condition;
    atomic<bool> stopped;
    thread workerThread;
    
    void workerLoop() {
        while (true) {
            shared_ptr<AsyncTask> task;
            {
                unique_lock<mutex> lock(queueMutex);
                
                condition.wait(lock, [this] { return stopped || !taskQueue.empty(); });
                
                if (stopped && taskQueue.empty()) {
                    break;
                }
                
                task = taskQueue.front();
                taskQueue.pop();
            }
            
            if (task) {
                task->execute();
            }
        }
    }
    
public:
    AsyncTaskQueue() : stopped(false) {
        workerThread = thread(&AsyncTaskQueue::workerLoop, this);
    }
    
    ~AsyncTaskQueue() {
        shutdown();
    }
    
    // 添加任务并返回future
    future<void> enqueue(const T& data, function<void(const T&)> task) {
        auto asyncTask = make_shared<AsyncTask>(data, move(task));
        auto future = asyncTask->completionPromise.get_future();
        
        {
            lock_guard<mutex> lock(queueMutex);
            if (stopped) {
                throw runtime_error("队列已停止，无法添加任务");
            }
            taskQueue.push(asyncTask);
        }
        
        condition.notify_one();
        return future;
    }
    
    // 关闭队列
    void shutdown() {
        {
            lock_guard<mutex> lock(queueMutex);
            stopped = true;
            
            // 取消所有待处理任务
            while (!taskQueue.empty()) {
                auto task = taskQueue.front();
                taskQueue.pop();
                task->cancel();
            }
        }
        
        condition.notify_all();
        
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
};

// 线程安全的对象池，使用智能指针管理对象生命周期
template<typename T>
class ThreadSafeObjectPool {
private:
    // 对象包装器，包含对象本身和重置函数
    struct PooledObject {
        unique_ptr<T> object;
        function<void(T&)> resetFunc;
        
        PooledObject(unique_ptr<T> obj, function<void(T&)> reset) 
            : object(move(obj)), resetFunc(move(reset)) {}
            
        void reset() {
            if (object && resetFunc) {
                resetFunc(*object);
            }
        }
    };
    
    // 对象池
    vector<shared_ptr<PooledObject>> availableObjects;
    queue<weak_ptr<PooledObject>> waitingObjects;  // 等待重新归队的对象
    
    mutable mutex poolMutex;
    condition_variable condition;
    
    // 工厂函数
    function<unique_ptr<T>()> factory;
    function<void(T&)> resetFunction;
    
    // 清理失效的对象
    void cleanupExpiredObjects() {
        // 清理等待队列中的失效对象
        queue<weak_ptr<PooledObject>> validObjects;
        while (!waitingObjects.empty()) {
            auto weakObj = waitingObjects.front();
            waitingObjects.pop();
            
            if (auto sharedObj = weakObj.lock()) {
                validObjects.push(weakObj);
            }
        }
        waitingObjects = move(validObjects);
    }
    
public:
    ThreadSafeObjectPool(size_t initialSize = 0, 
                       function<unique_ptr<T>()> fact = []() { return make_unique<T>(); },
                       function<void(T&)> reset = [](T&) {})
        : factory(move(fact)), resetFunction(move(reset)) {
        
        // 预创建对象
        for (size_t i = 0; i < initialSize; ++i) {
            auto obj = factory();
            if (obj) {
                auto pooledObj = make_shared<PooledObject>(move(obj), resetFunction);
                availableObjects.push_back(pooledObj);
            }
        }
    }
    
    // 获取对象
    class PooledObjectHandle {
    private:
        shared_ptr<PooledObject> pooledObject;
        ThreadSafeObjectPool* pool;
        
    public:
        PooledObjectHandle(shared_ptr<PooledObject> obj, ThreadSafeObjectPool* p) 
            : pooledObject(move(obj)), pool(p) {}
            
        PooledObjectHandle(const PooledObjectHandle&) = delete;
        PooledObjectHandle& operator=(const PooledObjectHandle&) = delete;
        
        PooledObjectHandle(PooledObjectHandle&& other) noexcept 
            : pooledObject(move(other.pooledObject)), pool(other.pool) {
            other.pool = nullptr;
        }
        
        PooledObjectHandle& operator=(PooledObjectHandle&& other) noexcept {
            if (this != &other) {
                if (pooledObject && pool) {
                    pool->returnObject(pooledObject);
                }
                
                pooledObject = move(other.pooledObject);
                pool = other.pool;
                other.pool = nullptr;
            }
            return *this;
        }
        
        ~PooledObjectHandle() {
            if (pooledObject && pool) {
                pool->returnObject(pooledObject);
            }
        }
        
        // 重载*和->操作符
        T& operator*() { return *pooledObject->object; }
        const T& operator*() const { return *pooledObject->object; }
        
        T* operator->() { return pooledObject->object.get(); }
        const T* operator->() const { return pooledObject->object.get(); }
        
        // 检查是否有效
        explicit operator bool() const { return pooledObject && pooledObject->object; }
    };
    
    PooledObjectHandle acquire() {
        unique_lock<mutex> lock(poolMutex);
        
        // 清理失效对象
        cleanupExpiredObjects();
        
        // 如果没有可用对象，创建新对象
        if (availableObjects.empty()) {
            auto newObj = factory();
            if (!newObj) {
                throw runtime_error("无法创建新对象");
            }
            
            auto pooledObj = make_shared<PooledObject>(move(newObj), resetFunction);
            return PooledObjectHandle(pooledObj, this);
        }
        
        // 返回现有对象
        auto pooledObj = availableObjects.back();
        availableObjects.pop_back();
        return PooledObjectHandle(pooledObj, this);
    }
    
    // 尝试获取对象（非阻塞）
    bool tryAcquire(PooledObjectHandle& handle) {
        unique_lock<mutex> lock(poolMutex);
        
        cleanupExpiredObjects();
        
        if (availableObjects.empty()) {
            return false;
        }
        
        auto pooledObj = availableObjects.back();
        availableObjects.pop_back();
        handle = PooledObjectHandle(pooledObj, this);
        return true;
    }
    
private:
    // 归还对象
    void returnObject(shared_ptr<PooledObject> pooledObj) {
        if (!pooledObj) return;
        
        // 重置对象状态
        pooledObj->reset();
        
        {
            unique_lock<mutex> lock(poolMutex);
            availableObjects.push_back(pooledObj);
        }
        condition.notify_one();
    }
    
    // 友元类，用于访问私有成员
    friend class PooledObjectHandle;
};

// 演示函数
void resourceManagerDemo() {
    cout << "\n=== 资源管理器演示 ===" << endl;
    
    ThreadSafeResourceManager manager;
    
    // 创建资源
    auto resource1 = manager.createResource("资源1", 42);
    auto resource2 = manager.createResource("资源2", 100);
    
    // 在不同线程中使用资源
    vector<thread> threads;
    
    threads.emplace_back([&manager] {
        for (int i = 0; i < 5; ++i) {
            manager.modifyResource("资源1", i * 10);
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    });
    
    threads.emplace_back([&manager] {
        for (int i = 0; i < 5; ++i) {
            manager.modifyResource("资源2", i * 20);
            this_thread::sleep_for(chrono::milliseconds(150));
        }
    });
    
    // 等待线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    manager.listActiveResources();
}

void threadSafeSharedPtrDemo() {
    cout << "\n=== 线程安全共享指针演示 ===" << endl;
    
    ThreadSafeSharedPtr<vector<int>> safeVec({1, 2, 3, 4, 5});
    
    // 多线程读写
    vector<thread> threads;
    
    // 写线程
    threads.emplace_back([&safeVec] {
        for (int i = 0; i < 10; ++i) {
            auto vec = safeVec.get();
            vec->push_back(i * 10);
            safeVec.set(*vec);
            this_thread::sleep_for(chrono::milliseconds(50));
        }
    });
    
    // 读线程
    threads.emplace_back([&safeVec] {
        for (int i = 0; i < 20; ++i) {
            auto vec = safeVec.get();
            cout << "当前向量大小: " << vec->size() << endl;
            this_thread::sleep_for(chrono::milliseconds(25));
        }
    });
    
    // 等待线程完成
    for (auto& t : threads) {
        t.join();
    }
}

void asyncTaskQueueDemo() {
    cout << "\n=== 异步任务队列演示 ===" << endl;
    
    AsyncTaskQueue<string> queue;
    
    // 添加任务
    vector<future<void>> futures;
    
    for (int i = 0; i < 5; ++i) {
        futures.push_back(queue.enqueue("任务" + to_string(i + 1), [](const string& task) {
            cout << "执行任务: " << task << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }));
    }
    
    // 等待所有任务完成
    for (auto& future : futures) {
        future.wait();
    }
}

void objectPoolDemo() {
    cout << "\n=== 对象池演示 ===" << endl;
    
    // 创建对象池，预分配3个对象
    ThreadSafeObjectPool<vector<int>> pool(3, 
        []() { return make_unique<vector<int>>(); },
        [](vector<int>& vec) { 
            vec.clear();
            cout << "重置向量对象" << endl;
        });
    
    // 多线程获取和释放对象
    vector<thread> threads;
    
    threads.emplace_back([&pool] {
        for (int i = 0; i < 3; ++i) {
            auto handle = pool.acquire();
            if (handle) {
                handle->push_back(i);
                handle->push_back(i * 2);
                cout << "线程1 处理向量，大小: " << handle->size() << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    });
    
    threads.emplace_back([&pool] {
        for (int i = 0; i < 3; ++i) {
            auto handle = pool.acquire();
            if (handle) {
                handle->push_back(i * 10);
                handle->push_back(i * 20);
                cout << "线程2 处理向量，大小: " << handle->size() << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(150));
        }
    });
    
    // 等待线程完成
    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    cout << "C++11 智能指针与线程高级用法演示" << endl;
    
    try {
        resourceManagerDemo();
        threadSafeSharedPtrDemo();
        asyncTaskQueueDemo();
        objectPoolDemo();
        
        cout << "\n程序结束" << endl;
    } catch (const exception& e) {
        cerr << "异常: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}