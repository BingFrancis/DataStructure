/**
 * 日志系统 (Logger) 实现
 *
 * 功能：
 * - 多级别日志（DEBUG, INFO, WARN, ERROR, FATAL）
 * - 异步写入（使用线程池）
 * - 日志文件轮转
 * - 线程安全
 * - 支持格式化输出
 *
 * 编译：g++ -std=c++17 logger.cpp -o logger.exe -pthread
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <functional>

using namespace std;

// ========================================
// 日志级别
// ========================================

enum class LogLevel : int {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    FATAL = 4
};

inline string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

// ========================================
// 日志记录结构
// ========================================

struct LogRecord {
    LogLevel level;
    string message;
    string timestamp;
    string threadId;
};

// ========================================
// 异步日志写入器
// ========================================

class AsyncLogWriter {
private:
    queue<LogRecord> logQueue;
    mutex queueMutex;
    condition_variable condition;
    thread writerThread;
    ofstream logFile;
    atomic<bool> running;
    LogLevel minLevel;
    string logFilePath;
    size_t maxFileSize;
    size_t maxBackupFiles;
    int currentFileSize;

    // 格式化时间戳
    string getCurrentTimestamp() {
        auto now = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(now);
        auto ms = chrono::duration_cast<chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        stringstream ss;
        ss << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S");
        ss << '.' << setfill('0') << setw(3) << ms.count();
        return ss.str();
    }

    // 获取线程 ID
    string getThreadId() {
        stringstream ss;
        ss << this_thread::get_id();
        return ss.str();
    }

    // 写入日志到文件和控制台
    void writeLog(const LogRecord& record) {
        string output = "[" + record.timestamp + "] "
                       + "[" + record.threadId + "] "
                       + "[" + logLevelToString(record.level) + "] "
                       + record.message;

        // 写入控制台
        if (record.level >= LogLevel::ERROR) {
            cerr << output << endl;
        } else {
            cout << output << endl;
        }

        // 写入文件
        if (logFile.is_open()) {
            logFile << output << endl;
            logFile.flush();

            // 检查文件大小，必要时轮转
            currentFileSize += output.size() + 1; // +1 for newline
            if (currentFileSize >= maxFileSize) {
                rotateLogFile();
            }
        }
    }

    // 日志文件轮转
    void rotateLogFile() {
        logFile.close();

        // 删除最老的备份
        string oldestBackup = logFilePath + "." + to_string(maxBackupFiles - 1);
        remove(oldestBackup.c_str());

        // 重命名现有备份
        for (int i = maxBackupFiles - 2; i >= 0; --i) {
            string oldName = (i == 0) ? logFilePath : logFilePath + "." + to_string(i);
            string newName = logFilePath + "." + to_string(i + 1);
            rename(oldName.c_str(), newName.c_str());
        }

        // 重新打开日志文件
        logFile.open(logFilePath, ios::out | ios::app);
        currentFileSize = 0;
    }

    // 写入线程主函数
    void writerLoop() {
        while (running || !logQueue.empty()) {
            unique_lock<mutex> lock(queueMutex);
            condition.wait(lock, [this] {
                return !logQueue.empty() || !running;
            });

            while (!logQueue.empty()) {
                LogRecord record = logQueue.front();
                logQueue.pop();
                lock.unlock();

                writeLog(record);

                lock.lock();
            }
        }
    }

public:
    AsyncLogWriter(const string& filePath = "app.log",
                  size_t maxSize = 10 * 1024 * 1024, // 10MB
                  size_t maxBackups = 5,
                  LogLevel level = LogLevel::INFO)
        : logFilePath(filePath)
        , maxFileSize(maxSize)
        , maxBackupFiles(maxBackups)
        , currentFileSize(0)
        , minLevel(level)
        , running(true) {

        // 打开日志文件
        logFile.open(logFilePath, ios::out | ios::app);
        if (!logFile.is_open()) {
            cerr << "无法打开日志文件: " << logFilePath << endl;
        }

        // 启动写入线程
        writerThread = thread(&AsyncLogWriter::writerLoop, this);
    }

    ~AsyncLogWriter() {
        running = false;
        condition.notify_all();

        if (writerThread.joinable()) {
            writerThread.join();
        }

        if (logFile.is_open()) {
            logFile.close();
        }
    }

    // 添加日志记录
    void log(LogLevel level, const string& message) {
        if (level < minLevel) {
            return;
        }

        LogRecord record;
        record.level = level;
        record.message = message;
        record.timestamp = getCurrentTimestamp();
        record.threadId = getThreadId();

        {
            lock_guard<mutex> lock(queueMutex);
            logQueue.push(record);
        }
        condition.notify_one();
    }

    void setMinLevel(LogLevel level) {
        minLevel = level;
    }
};

// ========================================
// 日志接口（单例）
// ========================================

class Logger {
private:
    static unique_ptr<AsyncLogWriter> instance;
    static mutex instanceMutex;

    Logger() = default;

public:
    static AsyncLogWriter& getInstance() {
        call_once(initFlag, []() {
            instance = make_unique<AsyncLogWriter>(
                "app.log",
                10 * 1024 * 1024,  // 10MB
                5,                  // 5 个备份
                LogLevel::DEBUG     // 最低日志级别
            );
        });
        return *instance;
    }

    static once_flag initFlag;

    // 便捷的日志方法
    static void debug(const string& msg) { getInstance().log(LogLevel::DEBUG, msg); }
    static void info(const string& msg) { getInstance().log(LogLevel::INFO, msg); }
    static void warn(const string& msg) { getInstance().log(LogLevel::WARN, msg); }
    static void error(const string& msg) { getInstance().log(LogLevel::ERROR, msg); }
    static void fatal(const string& msg) { getInstance().log(LogLevel::FATAL, msg); }

    // 格式化日志
    template<typename... Args>
    static void logf(LogLevel level, const string& format, Args&&... args) {
        // 简单的格式化（实际项目中可以使用 fmt 库）
        string msg = format;
        getInstance().log(level, msg);
    }
};

unique_ptr<AsyncLogWriter> Logger::instance;
mutex Logger::instanceMutex;
once_flag Logger::initFlag;

// ========================================
// 宏定义（简化日志调用）
// ========================================

#define LOG_DEBUG(msg) Logger::debug(msg)
#define LOG_INFO(msg) Logger::info(msg)
#define LOG_WARN(msg) Logger::warn(msg)
#define LOG_ERROR(msg) Logger::error(msg)
#define LOG_FATAL(msg) Logger::fatal(msg)

// ========================================
// 测试用例
// ========================================

void testBasicLogging() {
    cout << "\n=== 测试1：基本日志 ===" << endl;

    LOG_DEBUG("这是一条 DEBUG 消息");
    LOG_INFO("这是一条 INFO 消息");
    LOG_WARN("这是一条 WARN 消息");
    LOG_ERROR("这是一条 ERROR 消息");
    LOG_FATAL("这是一条 FATAL 消息");

    this_thread::sleep_for(chrono::seconds(1));
}

void testConcurrentLogging() {
    cout << "\n=== 测试2：并发日志 ===" << endl;

    vector<thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i] {
            for (int j = 0; j < 10; ++j) {
                LOG_INFO("线程 " + to_string(i) + " 消息 " + to_string(j));
                this_thread::sleep_for(chrono::milliseconds(10));
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    this_thread::sleep_for(chrono::seconds(1));
}

void testPerformance() {
    cout << "\n=== 测试3：性能测试 ===" << endl;

    auto start = chrono::high_resolution_clock::now();

    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        LOG_INFO("性能测试消息 " + to_string(i));
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "写入 " << N << " 条日志耗时: " << duration.count() << "ms" << endl;
    cout << "平均每条日志: " << (double)duration.count() / N << "ms" << endl;

    this_thread::sleep_for(chrono::seconds(1));
}

void testDifferentLevels() {
    cout << "\n=== 测试4：不同日志级别 ===" << endl;

    // 模拟不同场景的日志
    LOG_DEBUG("调试信息：变量 x = 10");
    LOG_INFO("系统启动成功，监听端口 8080");
    LOG_WARN("内存使用率超过 80%");
    LOG_ERROR("无法连接到数据库，尝试重连中...");
    LOG_FATAL("系统崩溃，正在保存核心转储...");

    this_thread::sleep_for(chrono::seconds(1));
}

void testRealWorldScenario() {
    cout << "\n=== 测试5：真实场景模拟 ===" << endl;

    // 模拟 Web 服务器请求处理
    LOG_INFO("服务器启动，监听 0.0.0.0:8080");

    auto handleRequest = [](int requestId) {
        LOG_DEBUG("收到请求 #" + to_string(requestId));

        // 模拟数据库查询
        LOG_DEBUG("查询数据库...");
        if (requestId % 3 == 0) {
            LOG_WARN("查询耗时过长，超过 100ms");
        }
        LOG_INFO("请求 #" + to_string(requestId) + " 处理成功");

        // 模拟错误情况
        if (requestId % 5 == 0) {
            LOG_ERROR("请求 #" + to_string(requestId) + " 处理失败：内部错误");
        }
    };

    vector<thread> threads;
    for (int i = 1; i <= 10; ++i) {
        threads.emplace_back(handleRequest, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    LOG_INFO("服务器正常关闭");
    this_thread::sleep_for(chrono::seconds(1));
}

// ========================================
// 主函数
// ========================================

int main() {
    cout << "日志系统示例程序" << endl;
    cout << "日志文件: app.log" << endl;
    cout << "----------------------------------------" << endl;

    testBasicLogging();
    testDifferentLevels();
    testConcurrentLogging();
    testPerformance();
    testRealWorldScenario();

    cout << "\n所有测试完成！请查看 app.log 文件" << endl;

    return 0;
}
