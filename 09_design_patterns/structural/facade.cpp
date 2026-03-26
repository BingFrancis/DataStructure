// ============================================================================
// 外观模式 (Facade Pattern)
// ============================================================================
// 外观模式为子系统中的一组接口提供一个一致的界面，
// 外观模式定义了一个高层接口，这个接口使得这一子系统更加容易使用。
//
// 适用场景：
// 1. 要为一个复杂子系统提供一个简单接口
// 2. 客户程序与抽象类的实现部分之间存在着很大的依赖性
// 3. 需要构建一个层次结构的子系统
//
// 示例：家庭影院系统
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// 子系统组件（Subsystems）
// ============================================================================

// 电视
class TV {
public:
    void on() { cout << "  [TV] 电视已打开" << endl; }
    void off() { cout << "  [TV] 电视已关闭" << endl; }
    void setInput(string input) { cout << "  [TV] 输入源设置为: " << input << endl; }
};

// 音响
class SoundSystem {
public:
    void on() { cout << "  [音响] 音响已打开" << endl; }
    void off() { cout << "  [音响] 音响已关闭" << endl; }
    void setVolume(int level) { cout << "  [音响] 音量设置为: " << level << endl; }
    void setSurroundSound() { cout << "  [音响] 开启环绕声模式" << endl; }
};

// DVD 播放器
class DVDPlayer {
public:
    void on() { cout << "  [DVD] DVD 播放器已打开" << endl; }
    void off() { cout << "  [DVD] DVD 播放器已关闭" << endl; }
    void play(string movie) { cout << "  [DVD] 正在播放: " << movie << endl; }
    void stop() { cout << "  [DVD] 停止播放" << endl; }
    void eject() { cout << "  [DVD] 弹出光盘" << endl; }
};

// 投影仪
class Projector {
public:
    void on() { cout << "  [投影仪] 投影仪已打开" << endl; }
    void off() { cout << "  [投影仪] 投影仪已关闭" << endl; }
    void setInput(string input) { cout << "  [投影仪] 输入源设置为: " << input << endl; }
    void wideScreenMode() { cout << "  [投影仪] 切换到宽屏模式" << endl; }
};

// 灯光
class TheaterLights {
public:
    void on() { cout << "  [灯光] 灯光已打开" << endl; }
    void off() { cout << "  [灯光] 灯光已关闭" << endl; }
    void dim(int level) { cout << "  [灯光] 灯光调暗到: " << level << "%" << endl; }
};

// 屏幕升降
class Screen {
public:
    void up() { cout << "  [屏幕] 屏幕升起" << endl; }
    void down() { cout << "  [屏幕] 屏幕降落" << endl; }
};

// 爆米花
class PopcornPopper {
public:
    void on() { cout << "  [爆米花] 爆米花机已打开" << endl; }
    void off() { cout << "  [爆米花] 爆米花机已关闭" << endl; }
    void pop() { cout << "  [爆米花] 正在爆米花..." << endl; }
};

// ============================================================================
// 外观类（Facade）
// ============================================================================

// 家庭影院外观
class HomeTheaterFacade {
private:
    // 子系统组件
    Amplifier amp;
    Tuner tuner;
    DVDPlayer dvd;
    CDPlayer cd;
    Projector projector;
    TheaterLights lights;
    Screen screen;
    PopcornPopper popper;

public:
    HomeTheaterFacade(Amplifier a, Tuner t, DVDPlayer d,
                    CDPlayer c, Projector p,
                    TheaterLights l, Screen s,
                    PopcornPopper pp)
        : amp(a), tuner(t), dvd(d), cd(c),
          projector(p), lights(l), screen(s), popper(pp) {}

    // 简化的接口：观看电影
    void watchMovie(string movie) {
        cout << "\n========== 准备观看电影 ==========" << endl;
        cout << "电影: " << movie << endl;

        popper.on();
        popper.pop();

        lights.dim(10);

        screen.down();

        projector.on();
        projector.setInput("DVD");
        projector.wideScreenMode();

        amp.on();
        amp.setDvd(dvd);
        amp.setSurroundSound();
        amp.setVolume(5);

        dvd.on();
        dvd.play(movie);

        cout << "\n====================================" << endl;
    }

    // 简化的接口：结束观看
    void endMovie() {
        cout << "\n========== 关闭家庭影院 ==========" << endl;

        popper.off();
        lights.on();

        screen.up();

        projector.off();
        amp.off();

        dvd.stop();
        dvd.off();

        cout << "\n====================================" << endl;
    }

    // 简化的接口：听 CD
    void listenToCD(string cdTitle) {
        cout << "\n========== 准备听 CD ==========" << endl;
        cout << "CD: " << cdTitle << endl;

        lights.on();
        amp.on();
        amp.setCd(cd);
        amp.setVolume(3);
        cd.on();
        cd.play(cdTitle);

        cout << "\n====================================" << endl;
    }

    // 简化的接口：听收音机
    void listenToRadio(double frequency) {
        cout << "\n========== 准备听收音机 ==========" << endl;
        cout << "频率: " << frequency << " MHz" << endl;

        lights.on();
        amp.on();
        amp.setTuner(tuner);
        tuner.setFrequency(frequency);
        amp.setVolume(4);

        cout << "\n====================================" << endl;
    }
};

// ============================================================================
// 放大器（子系统中的复杂组件）
// ============================================================================

class Amplifier {
private:
    string description;
    Tuner* tuner;
    DVDPlayer* dvd;
    CDPlayer* cd;

public:
    Amplifier() : description("放大器"), tuner(nullptr), dvd(nullptr), cd(nullptr) {}

    void on() { cout << "  [放大器] 已打开" << endl; }
    void off() { cout << "  [放大器] 已关闭" << endl; }

    void setStereoSound() { cout << "  [放大器] 设置立体声模式" << endl; }
    void setSurroundSound() { cout << "  [放大器] 设置环绕声模式" << endl; }

    void setVolume(int level) { cout << "  [放大器] 音量设置为: " << level << endl; }

    void setTuner(Tuner& t) { tuner = &t; }
    void setDvd(DVDPlayer& d) { dvd = &d; }
    void setCd(CDPlayer& c) { cd = &c; }

    string toString() const { return description; }
};

// 调谐器
class Tuner {
private:
    string description;
    double frequency;

public:
    Tuner() : description("调谐器"), frequency(87.5) {}

    void on() { cout << "  [调谐器] 已打开" << endl; }
    void off() { cout << "  [调谐器] 已关闭" << endl; }

    void setFrequency(double freq) {
        frequency = freq;
        cout << "  [调谐器] 频率设置为: " << freq << " MHz" << endl;
    }

    double getFrequency() const { return frequency; }

    string toString() const { return description; }
};

// CD 播放器
class CDPlayer {
private:
    string description;
    string currentTrack;

public:
    CDPlayer() : description("CD 播放器"), currentTrack("") {}

    void on() { cout << "  [CD] 已打开" << endl; }
    void off() { cout << "  [CD] 已关闭" << endl; }

    void play(string title) {
        currentTrack = title;
        cout << "  [CD] 正在播放: " << title << endl;
    }

    void stop() { cout << "  [CD] 停止播放" << endl; }

    void eject() { cout << "  [CD] 弹出光盘" << endl; }

    string toString() const { return description; }
};

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 外观模式示例 ==========" << endl;

    // 创建所有子系统组件
    Amplifier amp;
    Tuner tuner;
    DVDPlayer dvd;
    CDPlayer cd;
    Projector projector;
    TheaterLights lights;
    Screen screen;
    PopcornPopper popper;

    // 创建外观（家庭影院控制器）
    cout << "\n--- 创建家庭影院外观 ---" << endl;
    HomeTheaterFacade homeTheater(amp, tuner, dvd, cd, projector, lights, screen, popper);

    // 使用外观的简化接口

    // 示例 1：观看电影
    homeTheater.watchMovie("复仇者联盟 4");

    // 模拟观看过程
    cout << "\n--- 电影正在播放中（1小时）..." << endl;
    cout << "  🎬 享受电影！" << endl;

    // 结束观看
    homeTheater.endMovie();

    // 示例 2：听 CD
    homeTheater.listenToCD("周杰伦 - 范特西");

    // 示例 3：听收音机
    homeTheater.listenToRadio(101.5);

    // 对比：不使用外观的复杂操作
    cout << "\n\n========== 不使用外观的操作（复杂）==========";
    cout << "\n如果直接使用子系统组件，客户端需要：" << endl;
    cout << "  1. popper.on() + popper.pop()" << endl;
    cout << "  2. lights.dim()" << endl;
    cout << "  3. screen.down()" << endl;
    cout << "  4. projector.on() + projector.setInput() + ..." << endl;
    cout << "  5. amp.on() + amp.setDvd() + amp.setVolume() + ..." << endl;
    cout << "  6. dvd.on() + dvd.play()" << endl;
    cout << "\n而使用外观，只需：" << endl;
    cout << "  homeTheater.watchMovie(\"电影名\")" << endl;
    cout << "\n客户端代码大大简化！✨" << endl;

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 facade.cpp -o facade.exe && ./facade.exe
// ============================================================================
