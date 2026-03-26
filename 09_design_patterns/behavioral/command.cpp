// ============================================================================
// 命令模式 (Command Pattern)
// ============================================================================
// 命令模式将请求封装为对象，从而允许用不同的请求对客户进行参数化，
// 对请求排队或记录请求日志，以及支持可撤销的操作。
//
// 适用场景：
// 1. 需要将调用请求的对象与执行请求的对象解耦
// 2. 需要在不同的时间指定请求、排队和执行请求
// 3. 需要支持撤销操作
//
// 示例：智能遥控器
// ============================================================================

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// 命令接口
// ============================================================================

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual string getName() const = 0;
};

// ============================================================================
// 接收者（Receiver）- 实际执行命令的对象
// ============================================================================

// 电视机
class TV {
private:
    bool isOn = false;
    int volume = 10;
    int channel = 1;

public:
    void turnOn() {
        isOn = true;
        cout << "电视机已打开" << endl;
    }

    void turnOff() {
        isOn = false;
        cout << "电视机已关闭" << endl;
    }

    void setVolume(int level) {
        volume = level;
        cout << "电视音量设置为: " << volume << endl;
    }

    void setChannel(int ch) {
        channel = ch;
        cout << "电视频道切换到: " << channel << endl;
    }

    bool getOnState() const { return isOn; }
    int getVolume() const { return volume; }
    int getChannel() const { return channel; }
};

// 音响
class Stereo {
private:
    bool isOn = false;
    int volume = 5;

public:
    void turnOn() {
        isOn = true;
        cout << "音响已打开" << endl;
    }

    void turnOff() {
        isOn = false;
        cout << "音响已关闭" << endl;
    }

    void setVolume(int level) {
        volume = level;
        cout << "音响音量设置为: " << volume << endl;
    }

    bool getOnState() const { return isOn; }
    int getVolume() const { return volume; }
};

// ============================================================================
// 具体命令类
// ============================================================================

// 打开电视命令
class TVOnCommand : public Command {
private:
    TV& tv;
    bool previousState;

public:
    explicit TVOnCommand(TV& t) : tv(t), previousState(false) {}

    void execute() override {
        previousState = tv.getOnState();
        tv.turnOn();
    }

    void undo() override {
        if (!previousState) {
            tv.turnOff();
        }
    }

    string getName() const override { return "打开电视"; }
};

// 关闭电视命令
class TVOffCommand : public Command {
private:
    TV& tv;
    bool previousState;

public:
    explicit TVOffCommand(TV& t) : tv(t), previousState(false) {}

    void execute() override {
        previousState = tv.getOnState();
        tv.turnOff();
    }

    void undo() override {
        if (previousState) {
            tv.turnOn();
        }
    }

    string getName() const override { return "关闭电视"; }
};

// 设置电视音量命令
class TVVolumeCommand : public Command {
private:
    TV& tv;
    int newVolume;
    int previousVolume;

public:
    TVVolumeCommand(TV& t, int volume) : tv(t), newVolume(volume), previousVolume(0) {}

    void execute() override {
        previousVolume = tv.getVolume();
        tv.setVolume(newVolume);
    }

    void undo() override {
        tv.setVolume(previousVolume);
    }

    string getName() const override {
        return "设置电视音量到 " + to_string(newVolume);
    }
};

// 打开音响命令
class StereoOnCommand : public Command {
private:
    Stereo& stereo;
    bool previousState;

public:
    explicit StereoOnCommand(Stereo& s) : stereo(s), previousState(false) {}

    void execute() override {
        previousState = stereo.getOnState();
        stereo.turnOn();
    }

    void undo() override {
        if (!previousState) {
            stereo.turnOff();
        }
    }

    string getName() const override { return "打开音响"; }
};

// ============================================================================
// 调用者（Invoker）- 触发命令的对象
// ============================================================================

class RemoteControl {
private:
    vector<unique_ptr<Command>> slots;
    stack<unique_ptr<Command>> undoStack;

public:
    RemoteControl(size_t numSlots = 7) {
        slots.resize(numSlots);
    }

    void setCommand(int slot, unique_ptr<Command> command) {
        slots[slot] = move(command);
    }

    void buttonPressed(int slot) {
        if (slot >= 0 && slot < slots.size() && slots[slot]) {
            cout << "\n[遥控器] 按下按钮 " << slot << endl;
            slots[slot]->execute();
            // 保存到撤销栈（需要克隆命令，简化处理）
            cout << "命令已执行，可以撤销" << endl;
        }
    }

    void undoButtonPressed() {
        if (!undoStack.empty()) {
            auto command = move(undoStack.top());
            undoStack.pop();
            cout << "\n[遥控器] 撤销操作" << endl;
            command->undo();
        } else {
            cout << "\n[遥控器] 没有可撤销的操作" << endl;
        }
    }
};

// ============================================================================
// 宏命令（Macro Command）- 组合多个命令
// ============================================================================

class MacroCommand : public Command {
private:
    vector<Command*> commands;

public:
    explicit MacroCommand(const vector<Command*>& cmds) : commands(cmds) {}

    void execute() override {
        cout << "\n--- 执行宏命令 ---" << endl;
        for (auto* cmd : commands) {
            if (cmd) {
                cout << "执行: " << cmd->getName() << endl;
                cmd->execute();
            }
        }
        cout << "--- 宏命令完成 ---" << endl;
    }

    void undo() override {
        cout << "\n--- 撤销宏命令 ---" << endl;
        // 反向撤销
        for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
            if (*it) {
                (*it)->undo();
            }
        }
    }

    string getName() const override { return "宏命令（一键启动）"; }
};

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 命令模式示例 ==========" << endl;

    // 创建接收者
    TV tv;
    Stereo stereo;

    // 创建命令对象
    auto tvOn = make_unique<TVOnCommand>(tv);
    auto tvOff = make_unique<TVOffCommand>(tv);
    auto tvVol30 = make_unique<TVVolumeCommand>(tv, 30);
    auto tvVol15 = make_unique<TVVolumeCommand>(tv, 15);
    auto stereoOn = make_unique<StereoOnCommand>(stereo);

    // 创建遥控器并设置命令
    RemoteControl remote;

    remote.setCommand(0, move(tvOn));
    remote.setCommand(1, move(tvOff));
    remote.setCommand(2, move(tvVol30));
    remote.setCommand(3, move(tvVol15));
    remote.setCommand(4, move(stereoOn));

    // 测试遥控器
    cout << "\n--- 测试遥控器 ---" << endl;
    remote.buttonPressed(0);  // 打开电视
    remote.buttonPressed(2);  // 音量30
    remote.buttonPressed(3);  // 音量15
    remote.buttonPressed(4);  // 打开音响

    // 创建宏命令（一键观影模式）
    vector<Command*> partyModeCommands = {
        tvOn.get(),
        tvVol30.get(),
        stereoOn.get()
    };

    auto partyMode = make_unique<MacroCommand>(partyModeCommands);

    cout << "\n--- 设置宏命令到按钮 5 ---" << endl;
    remote.setCommand(5, move(partyMode));
    remote.buttonPressed(5);  // 执行宏命令

    cout << "\n========================================" << endl;

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 command.cpp -o command.exe && ./command.exe
// ============================================================================
