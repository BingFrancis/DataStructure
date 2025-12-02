#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <functional>
#include <chrono>
#include <atomic>
#include <unordered_map>
#include <stack>
#include <type_traits>
#include <exception>

using namespace std;

// 简单的协程状态枚举
enum class CoroutineState {
    CREATED,
    RUNNING,
    SUSPENDED,
    COMPLETED,
    FAILED
};

// 协程异常类
class CoroutineException : public exception {
private:
    string message;
    
public:
    CoroutineException(const string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// 协程上下文，用于保存和恢复协程状态
template<typename T>
class CoroutineContext {
public:
    using CoroutineFunc = function<void(CoroutineContext<T>&)>;
    
private:
    CoroutineState state;
    T result;
    exception_ptr exceptionPtr;
    stack<function<void()>> resumeStack;  // 保存恢复点
    CoroutineFunc coroutineFunc;
    
    // 协程ID和调度器引用
    size_t coroutineId;
    
public:
    CoroutineContext(size_t id, CoroutineFunc func) 
        : state(CoroutineState::CREATED), coroutineFunc(move(func)), coroutineId(id) {}
    
    // 恢复协程执行
    void resume() {
        if (state == CoroutineState::COMPLETED || state == CoroutineState::FAILED) {
            return;
        }
        
        state = CoroutineState::RUNNING;
        
        try {
            if (coroutineFunc) {
                coroutineFunc(*this);
            }
        } catch (...) {
            exceptionPtr = current_exception();
            state = CoroutineState::FAILED;
        }
        
        if (state == CoroutineState::RUNNING) {
            state = CoroutineState::COMPLETED;
        }
    }
    
    // 挂起协程
    void suspend() {
        state = CoroutineState::SUSPENDED;
    }
    
    // 设置结果
    void setResult(const T& value) {
        result = value;
        state = CoroutineState::COMPLETED;
    }
    
    // 获取结果
    const T& getResult() const {
        if (exceptionPtr) {
            rethrow_exception(exceptionPtr);
        }
        return result;
    }
    
    // 获取状态
    CoroutineState getState() const {
        return state;
    }
    
    // 获取ID
    size_t getId() const {
        return coroutineId;
    }
    
    // 检查是否已完成
    bool isCompleted() const {
        return state == CoroutineState::COMPLETED;
    }
    
    // 检查是否失败
    bool isFailed() const {
        return state == CoroutineState::FAILED;
    }
    
    // 推送恢复点
    void pushResumePoint(function<void()> resumePoint) {
        resumeStack.push(move(resumePoint));
    }
    
    // 弹出并执行恢复点
    bool popAndExecuteResumePoint() {
        if (resumeStack.empty()) {
            return false;
        }
        
        auto resumePoint = move(resumeStack.top());
        resumeStack.pop();
        resumePoint();
        return true;
    }
};

// 协程调度器
template<typename T>
class CoroutineScheduler {
private:
    // 使用shared_ptr管理协程上下文
    using CoroutinePtr = shared_ptr<CoroutineContext<T>>;
    
    // 协程队列
    queue<CoroutinePtr> readyQueue;
    unordered_map<size_t, CoroutinePtr> runningCoroutines;
    unordered_map<size_t, CoroutinePtr> suspendedCoroutines;
    
    // 线程安全
    mutable mutex schedulerMutex;
    condition_variable condition;
    atomic<bool> stopped;
    
    // 工作线程
    thread schedulerThread;
    
    // 协程ID生成器
    atomic<size_t> nextId;
    
    // 调度器主循环
    void schedulerLoop() {
        while (!stopped) {
            unique_lock<mutex> lock(schedulerMutex);
            
            // 等待条件：调度器停止或有可运行的协程
            condition.wait(lock, [this] {
                return stopped || !readyQueue.empty() || !runningCoroutines.empty();
            });
            
            // 如果调度器停止且所有协程完成，退出
            if (stopped && readyQueue.empty() && runningCoroutines.empty()) {
                break;
            }
            
            // 处理就绪队列中的协程
            while (!readyQueue.empty()) {
                auto coroutine = readyQueue.front();
                readyQueue.pop();
                
                if (coroutine->isCompleted() || coroutine->isFailed()) {
                    // 从运行队列中移除已完成的协程
                    runningCoroutines.erase(coroutine->getId());
                } else {
                    // 恢复协程执行
                    lock.unlock();
                    coroutine->resume();
                    lock.lock();
                    
                    // 检查协程状态并处理
                    if (coroutine->isCompleted() || coroutine->isFailed()) {
                        runningCoroutines.erase(coroutine->getId());
                    } else if (coroutine->getState() == CoroutineState::SUSPENDED) {
                        // 协程被挂起，转移到挂起队列
                        suspendedCoroutines[coroutine->getId()] = coroutine;
                        runningCoroutines.erase(coroutine->getId());
                    } else {
                        // 协程仍在运行，放回就绪队列
                        readyQueue.push(coroutine);
                    }
                }
            }
            
            // 检查挂起的协程是否可以恢复
            for (auto it = suspendedCoroutines.begin(); it != suspendedCoroutines.end(); ) {
                auto& coroutine = it->second;
                
                // 这里可以添加恢复条件检查，例如基于时间或事件
                // 简化起见，我们立即恢复所有挂起的协程
                if (coroutine->popAndExecuteResumePoint()) {
                    readyQueue.push(coroutine);
                    it = suspendedCoroutines.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
    
public:
    CoroutineScheduler() : stopped(false), nextId(0) {
        schedulerThread = thread(&CoroutineScheduler::schedulerLoop, this);
    }
    
    ~CoroutineScheduler() {
        shutdown();
    }
    
    // 创建协程
    future<T> createCoroutine(function<void(CoroutineContext<T>&)> coroutineFunc) {
        auto id = nextId.fetch_add(1);
        auto context = make_shared<CoroutineContext<T>>(id, move(coroutineFunc));
        auto promise = make_shared<promise<T>>();
        auto future = promise->get_future();
        
        {
            lock_guard<mutex> lock(schedulerMutex);
            
            // 设置协程完成时的回调
            context->pushResumePoint([promise, context]() {
                try {
                    if (context->isCompleted()) {
                        promise->set_value(context->getResult());
                    } else if (context->isFailed()) {
                        try {
                            context->getResult(); // 这会重新抛出异常
                        } catch (...) {
                            promise->set_exception(current_exception());
                        }
                    }
                } catch (...) {
                    promise->set_exception(current_exception());
                }
            });
            
            runningCoroutines[id] = context;
            readyQueue.push(context);
        }
        
        condition.notify_one();
        return future;
    }
    
    // 恢复特定协程
    void resumeCoroutine(size_t coroutineId) {
        lock_guard<mutex> lock(schedulerMutex);
        
        auto it = suspendedCoroutines.find(coroutineId);
        if (it != suspendedCoroutines.end()) {
            auto coroutine = it->second;
            suspendedCoroutines.erase(it);
            readyQueue.push(coroutine);
            condition.notify_one();
        }
    }
    
    // 获取协程状态
    CoroutineState getCoroutineState(size_t coroutineId) const {
        lock_guard<mutex> lock(schedulerMutex);
        
        auto it = runningCoroutines.find(coroutineId);
        if (it != runningCoroutines.end()) {
            return it->second->getState();
        }
        
        it = suspendedCoroutines.find(coroutineId);
        if (it != suspendedCoroutines.end()) {
            return CoroutineState::SUSPENDED;
        }
        
        return CoroutineState::COMPLETED; // 默认返回已完成
    }
    
    // 关闭调度器
    void shutdown() {
        {
            lock_guard<mutex> lock(schedulerMutex);
            stopped = true;
        }
        
        condition.notify_all();
        
        if (schedulerThread.joinable()) {
            schedulerThread.join();
        }
    }
    
    // 等待所有协程完成
    void waitForAllCoroutines() {
        while (true) {
            {
                lock_guard<mutex> lock(schedulerMutex);
                if (readyQueue.empty() && runningCoroutines.empty() && suspendedCoroutines.empty()) {
                    break;
                }
            }
            
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
};

// 协程辅助函数，用于简化协程编写
namespace CoroutineUtils {
    // 暂停协程执行指定时间
    template<typename T>
    void delay(CoroutineContext<T>& context, chrono::milliseconds delayTime) {
        // 添加恢复点，在指定时间后恢复
        context.pushResumePoint([delayTime]() {
            this_thread::sleep_for(delayTime);
        });
        
        // 挂起协程
        context.suspend();
    }
    
    // 产生值并暂停协程
    template<typename T>
    void yield(CoroutineContext<T>& context, const T& value) {
        context.setResult(value);
        context.suspend();
    }
    
    // 等待条件满足
    template<typename T>
    void waitFor(CoroutineContext<T>& context, function<bool()> condition) {
        context.pushResumePoint([condition]() {
            while (!condition()) {
                this_thread::sleep_for(chrono::milliseconds(10));
            }
        });
        
        context.suspend();
    }
}

// 演示函数
void basicCoroutineDemo() {
    cout << "\n=== 基本协程演示 ===" << endl;
    
    CoroutineScheduler<int> scheduler;
    
    // 创建简单协程
    auto future = scheduler.createCoroutine([](CoroutineContext<int>& ctx) {
        cout << "协程开始执行" << endl;
        
        // 模拟工作
        for (int i = 0; i < 5; ++i) {
            cout << "协程工作中: " << i << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        
        cout << "协程完成" << endl;
        ctx.setResult(42);
    });
    
    // 等待协程完成并获取结果
    try {
        int result = future.get();
        cout << "协程结果: " << result << endl;
    } catch (const exception& e) {
        cout << "协程异常: " << e.what() << endl;
    }
}

void suspendResumeDemo() {
    cout << "\n=== 挂起/恢复协程演示 ===" << endl;
    
    CoroutineScheduler<int> scheduler;
    
    // 创建可挂起的协程
    auto future = scheduler.createCoroutine([](CoroutineContext<int>& ctx) {
        cout << "协程开始" << endl;
        
        cout << "第一次工作" << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        
        // 添加恢复点并挂起
        ctx.pushResumePoint([]() {
            cout << "第一次恢复" << endl;
        });
        ctx.suspend();
        
        cout << "第二次工作" << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        
        // 再次添加恢复点并挂起
        ctx.pushResumePoint([]() {
            cout << "第二次恢复" << endl;
        });
        ctx.suspend();
        
        cout << "协程完成" << endl;
        ctx.setResult(100);
    });
    
    // 获取协程ID
    this_thread::sleep_for(chrono::milliseconds(300)); // 等待协程开始并挂起
    
    // 手动恢复协程
    scheduler.resumeCoroutine(1); // 假设ID为1
    
    this_thread::sleep_for(chrono::milliseconds(300)); // 等待协程再次挂起
    
    // 再次恢复协程
    scheduler.resumeCoroutine(1);
    
    try {
        int result = future.get();
        cout << "协程最终结果: " << result << endl;
    } catch (const exception& e) {
        cout << "协程异常: " << e.what() << endl;
    }
}

void asyncCoroutineDemo() {
    cout << "\n=== 异步协程演示 ===" << endl;
    
    CoroutineScheduler<int> scheduler;
    
    // 创建多个协程
    vector<future<int>> futures;
    
    for (int i = 0; i < 3; ++i) {
        futures.push_back(scheduler.createCoroutine([i](CoroutineContext<int>& ctx) {
            cout << "协程 " << i << " 开始" << endl;
            
            // 模拟不同的工作负载
            this_thread::sleep_for(chrono::milliseconds(100 * (i + 1)));
            
            cout << "协程 " << i << " 完成" << endl;
            ctx.setResult(i * 10);
        }));
    }
    
    // 等待所有协程完成
    cout << "等待所有协程完成..." << endl;
    
    int total = 0;
    for (size_t i = 0; i < futures.size(); ++i) {
        try {
            int result = futures[i].get();
            cout << "协程 " << i << " 结果: " << result << endl;
            total += result;
        } catch (const exception& e) {
            cout << "协程 " << i << " 异常: " << e.what() << endl;
        }
    }
    
    cout << "结果总和: " << total << endl;
}

void errorHandlingDemo() {
    cout << "\n=== 错误处理演示 ===" << endl;
    
    CoroutineScheduler<int> scheduler;
    
    // 创建会抛出异常的协程
    auto future1 = scheduler.createCoroutine([](CoroutineContext<int>& ctx) {
        cout << "正常协程开始" << endl;
        ctx.setResult(42);
    });
    
    auto future2 = scheduler.createCoroutine([](CoroutineContext<int>& ctx) {
        cout << "异常协程开始" << endl;
        throw runtime_error("协程中发生错误");
    });
    
    // 等待协程完成
    try {
        int result1 = future1.get();
        cout << "正常协程结果: " << result1 << endl;
    } catch (const exception& e) {
        cout << "正常协程异常: " << e.what() << endl;
    }
    
    try {
        int result2 = future2.get();
        cout << "异常协程结果: " << result2 << endl;
    } catch (const exception& e) {
        cout << "异常协程异常: " << e.what() << endl;
    }
}

int main() {
    cout << "C++11 协程式任务调度器演示" << endl;
    
    try {
        basicCoroutineDemo();
        suspendResumeDemo();
        asyncCoroutineDemo();
        errorHandlingDemo();
        
        cout << "\n程序结束" << endl;
    } catch (const exception& e) {
        cerr << "异常: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}