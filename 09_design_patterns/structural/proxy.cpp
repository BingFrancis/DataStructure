// ============================================================================
// 代理模式 (Proxy Pattern)
// ============================================================================
// 代理模式为其他对象提供一种代理以控制对这个对象的访问。
// 代理对象在客户端和目标对象之间起到中介作用。
//
// 适用场景：
// 1. 远程代理：为远程对象提供本地代表
// 2. 虚代理：根据需要创建开销大的对象
// 3. 保护代理：控制对原始对象的访问
// 4. 智能引用：取代了裸指针，可以附加额外操作
//
// 示例：图片加载代理（懒加载）
// ============================================================================

#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ============================================================================
// 主题接口（Subject）
// ============================================================================

// 图像接口
class Image {
public:
    virtual ~Image() = default;
    virtual void display() = 0;
    virtual void showDetails() = 0;
};

// ============================================================================
// 真实主题（Real Subject）
// ============================================================================

// 真实的图像类（加载资源）
class RealImage : public Image {
private:
    string filename;
    bool loaded;

public:
    explicit RealImage(const string& file)
        : filename(file), loaded(false) {
        // 模拟从磁盘加载图像（耗时操作）
        cout << "  [加载] 从磁盘加载图像: " << filename << "..." << endl;
        // 假设加载需要时间
        cout << "  [加载] 图像数据加载完成" << endl;
        loaded = true;
    }

    void display() override {
        if (!loaded) {
            cout << "  [警告] 图像尚未加载！" << endl;
            return;
        }
        cout << "  [显示] 显示图像: " << filename << endl;
        cout << "  [显示] 渲染完成" << endl;
    }

    void showDetails() override {
        cout << "  [详情] 文件名: " << filename << endl;
        cout << "  [详情] 状态: " << (loaded ? "已加载" : "未加载") << endl;
    }
};

// ============================================================================
// 代理类（Proxy）
// ============================================================================

// 图像代理（延迟加载）
class ImageProxy : public Image {
private:
    string filename;
    unique_ptr<RealImage> realImage;  // 延迟创建
    bool initialized;

public:
    explicit ImageProxy(const string& file)
        : filename(file), initialized(false) {}

    void display() override {
        if (!initialized) {
            cout << "\n[代理] 首次访问，加载真实图像..." << endl;
            realImage = make_unique<RealImage>(filename);
            initialized = true;
        } else {
            cout << "\n[代理] 图像已缓存，直接显示..." << endl;
        }
        realImage->display();
    }

    void showDetails() override {
        cout << "\n[代理] 代理信息:" << endl;
        cout << "  文件名: " << filename << endl;
        cout << "  状态: " << (initialized ? "已加载" : "未加载（按需加载）") << endl;
        cout << "  代理功能: 懒加载、缓存、访问控制" << endl;
    }
};

// ============================================================================
// 保护代理示例
// ============================================================================

// 安全代理（访问控制）
class SecureImageProxy : public Image {
private:
    string filename;
    unique_ptr<RealImage> realImage;
    bool hasPermission;
    int accessCount;

public:
    SecureImageProxy(const string& file, bool permission)
        : filename(file), hasPermission(permission), accessCount(0) {}

    void display() override {
        accessCount++;
        cout << "\n[安全代理] 访问尝试 #" << accessCount << endl;

        if (!hasPermission) {
            cout << "  [拒绝] 无访问权限！" << endl;
            cout << "  [拒绝] 请联系管理员获取权限" << endl;
            return;
        }

        cout << "  [授权] 访问权限验证通过" << endl;

        if (!realImage) {
            realImage = make_unique<RealImage>(filename);
        }

        realImage->display();
    }

    void showDetails() override {
        cout << "\n[安全代理] 安全信息:" << endl;
        cout << "  文件名: " << filename << endl;
        cout << "  权限: " << (hasPermission ? "已授权" : "未授权") << endl;
        cout << "  访问次数: " << accessCount << endl;
        cout << "  代理功能: 访问控制、权限验证" << endl;
    }
};

// ============================================================================
// 图像库客户端
// ============================================================================

class ImageGallery {
private:
    vector<unique_ptr<Image>> images;

public:
    void addImage(unique_ptr<Image> image) {
        images.push_back(move(image));
    }

    void displayAll() {
        cout << "\n========== 显示所有图像 ==========" << endl;
        for (size_t i = 0; i < images.size(); ++i) {
            cout << "\n--- 图像 " << (i + 1) << " ---";
            images[i]->display();
        }
        cout << "\n====================================" << endl;
    }

    void showAllDetails() {
        cout << "\n========== 图像详细信息 ==========" << endl;
        for (size_t i = 0; i < images.size(); ++i) {
            cout << "\n--- 图像 " << (i + 1) << " ---";
            images[i]->showDetails();
        }
        cout << "\n====================================" << endl;
    }
};

// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "========== 代理模式示例 ==========" << endl;

    // 创建图像库
    ImageGallery gallery;

    // 示例 1：懒加载代理
    cout << "\n\n========== 懒加载代理 ==========" << endl;
    gallery.addImage(make_unique<ImageProxy>("photo1.jpg"));
    gallery.addImage(make_unique<ImageProxy>("photo2.jpg"));
    gallery.addImage(make_unique<ImageProxy>("photo3.jpg"));

    // 显示图像详细信息（不加载真实图像）
    gallery.showAllDetails();

    // 显示图像（会触发加载）
    gallery.displayAll();

    // 再次显示（使用缓存）
    cout << "\n\n========== 再次显示（使用缓存）==========";
    gallery.displayAll();

    // 示例 2：安全代理
    cout << "\n\n========== 安全代理 ==========" << endl;
    gallery.addImage(make_unique<SecureImageProxy>("secret1.jpg", false));  // 无权限
    gallery.addImage(make_unique<SecureImageProxy>("secret2.jpg", true));   // 有权限

    cout << "\n--- 尝试访问受保护的图像 ---";
    gallery.displayAll();

    return 0;
}

// ============================================================================
// 运行示例
// ============================================================================
// g++ -std=c++17 proxy.cpp -o proxy.exe && ./proxy.exe
// ============================================================================
