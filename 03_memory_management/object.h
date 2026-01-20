#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

/**
 * 对象池模板
 * 适用于频繁创建销毁对象的场景，如游戏粒子系统 网络连接池等
 */
template <typename T, size_t PoolSize = 1000>
class ObjectPool {
private:
    struct Node {
        alignas(T) unsigned char storage[sizeof(T)];
        Node* next;
    };

    Node* free_list;
    Node pool[PoolSize];
    size_t active_count = 0;

public:
    ObjectPool() {
        // 初始化空闲链表
        for (size_t i = 0; i < PoolSize; ++i) {
            pool[i].next = (i < PoolSize - 1) ? &pool[i + 1] : nullptr;
        }
        free_list = pool;
    }

    // 获取对象
    template <typename... Args>
    T* acquire(Args&&... args) {
        if (free_list == nullptr) {
            throw std::runtime_error("Object pool exhausted");
        }

        Node* node = free_list;
        free_list = free_list->next;

        // 使用 placement new 构造对象
        T* obj = reinterpret_cast<T*>(node->storage);
        new (obj) T(std::forward<Args>(args)...);

        active_count++;
        return obj;
    }

    // 归还对象
    void release(T* obj) {
        if (obj == nullptr) return;

        // 显式调用析构函数
        obj->~T();

        // 将内存放回池中
        Node* node = reinterpret_cast<Node*>(obj);
        node->next = free_list;
        free_list = node;

        active_count--;
    }

    size_t getActiveCount() const { return active_count; }
    size_t getAvailableCount() const { return PoolSize - active_count; }
};

#endif