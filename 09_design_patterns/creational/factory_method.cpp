// ============================================================================
// 工厂方法模式 (Factory Method Pattern)
// ============================================================================
// 工厂方法模式定义了一个创建对象的接口，但由子类决定要实例化的类是哪一个。
// 工厂方法让类把实例化推迟到子类。
//
// 适用场景：
// 1. 不知道具体需要创建哪个对象
// 2. 希望通过子类来决定创建哪种对象
// 3. 需要灵活扩展新的产品类型
//
// 示例：日志记录器工厂
// ============================================================================

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// 产品抽象类
// ============================================================================

// 日志记录器接口
class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const string& message) = 0;
};

// ============================================================================
// 具体产品类
// ============================================================================

// 控制台日志记录器
class ConsoleLogger : public Logger {
public:
    void log(const string& message) override {
        cout << "[Console] " << message << endl;
    }
};

// 文件日志记录器
class FileLogger : public Logger {
private:
    string filename;

public:
    explicit FileLogger(const string& file) : filename(file) {}

    void log(const string& message) override {
        cout << "[File:" << filename << "] " << message << endl;
        // 实际项目中会写入文件
    }
};

// 数据库日志记录器
class DatabaseLogger : public Logger {
private:
    string connectionString;

public:
    explicit DatabaseLogger(const string& conn) : connectionString(conn) {}

    void log(const string& message) override {
        cout << "[Database:" << connectionString << "] " << message << endl;
        // 实际项目中会写入数据库
    }
};

// ============================================================================
// 工厂基类（Creator）
// ============================================================================

class LoggerFactory {
public:
    // 工厂方法（纯虚函数）
    virtual unique_ptr<Logger> createLogger() = 0;
    
    virtual ~LoggerFactory() = default;
};

// ============================================================================
// 具体工厂类（Concrete Creators）
// ============================================================================

// 控制台日志记录器工厂
class ConsoleLoggerFactory : public LoggerFactory {
public:
    unique_ptr<Logger> createLogger() override {
        return make_unique<ConsoleLogger>();
    }
};

// 文件日志记录器工厂
class FileLoggerFactory : public LoggerFactory {
private:
    string filename;

public:
    explicit FileLoggerFactory(const string& file) : filename(file) {}

    unique_ptr<Logger> createLogger() override {
        return make_unique<FileLogger>(filename);
    }
};

// 数据库日志记录器工厂
class DatabaseLoggerFactory : public LoggerFactory {
private:
    string connectionString;

public:
    explicit DatabaseLoggerFactory(const string& conn) : connectionString(conn) {}

    unique_ptr<Logger> createLogger() override {
        return make_unique<DatabaseLogger>(connectionString);
    }
};

// ============================================================================
// 客户端代码
// ============================================================================

// 使用工厂创建日志记录器
void processLogs(LoggerFactory& factory) {
    auto logger = factory.createLogger();
    logger->log("系统启动...");
    logger->log("初始化配置...");
    logger->log("系统就绪！");
}

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 工厂方法模式示例 ==========" << endl;

    // 使用控制台日志记录器
    cout << "\n--- 控制台日志记录器 ---" << endl;
    ConsoleLoggerFactory consoleFactory;
    processLogs(consoleFactory);

    // 使用文件日志记录器
    cout << "\n--- 文件日志记录器 ---" << endl;
    FileLoggerFactory fileFactory("app.log");
    processLogs(fileFactory);

    // 使用数据库日志记录器
    cout << "\n--- 数据库日志记录器 ---" << endl;
    DatabaseLoggerFactory dbFactory("localhost:5432");
    processLogs(dbFactory);

    cout << "\n========================================" << endl;

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 factory_method.cpp -o factory_method.exe && ./factory_method.exe
// ============================================================================
