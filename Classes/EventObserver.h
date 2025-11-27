#ifndef EVENT_OBSERVER_H
#define EVENT_OBSERVER_H

#include "GameEvent.h"
#include <functional>
#include <set>
#include <map>
#include <cocos2d.h>

// 观察者接口
class IEventObserver {
public:
    virtual ~IEventObserver() = default;
    
    // 处理事件
    virtual void onEvent(const GameEvent& event) = 0;
    
    // 检查是否可以处理特定类型的事件
    virtual bool canHandle(GameEventType eventType) const = 0;
    
    // 获取优先级，数字越大优先级越高
    virtual int getPriority() const { return 0; }
};

// 通用事件观察者实现
class EventObserver : public IEventObserver {
public:
    using EventCallback = std::function<void(const GameEvent&)>;
    
    // 使用单个回调函数的构造函数
    explicit EventObserver(EventCallback callback)
        : _callback(std::move(callback)), _priority(0) {}
    
    // 使用回调函数和支持的事件类型的构造函数
    EventObserver(EventCallback callback, const std::set<GameEventType>& supportedEvents)
        : _callback(std::move(callback)), _supportedEvents(supportedEvents), _priority(0) {}
    
    // 使用回调函数、支持的事件类型和优先级的构造函数
    EventObserver(EventCallback callback, const std::set<GameEventType>& supportedEvents, int priority)
        : _callback(std::move(callback)), _supportedEvents(supportedEvents), _priority(priority) {}
    
    // 处理事件
    void onEvent(const GameEvent& event) override {
        if (_callback) {
            _callback(event);
        }
    }
    
    // 检查是否可以处理特定类型的事件
    bool canHandle(GameEventType eventType) const override {
        // 如果没有指定支持的事件类型，则处理所有事件
        if (_supportedEvents.empty()) {
            return true;
        }
        return _supportedEvents.find(eventType) != _supportedEvents.end();
    }
    
    // 获取优先级
    int getPriority() const override {
        return _priority;
    }
    
    // 设置优先级
    void setPriority(int priority) {
        _priority = priority;
    }
    
    // 添加支持的事件类型
    void addSupportedEvent(GameEventType eventType) {
        _supportedEvents.insert(eventType);
    }
    
    // 移除支持的事件类型
    void removeSupportedEvent(GameEventType eventType) {
        _supportedEvents.erase(eventType);
    }
    
private:
    EventCallback _callback;
    std::set<GameEventType> _supportedEvents;
    int _priority;
};

// UI专用事件观察者实现
class UIEventObserver : public IEventObserver {
public:
    using EventHandler = std::function<void(const GameEvent&)>;
    
    // 构造函数，关联到一个cocos2d::Node
    explicit UIEventObserver(cocos2d::Node* owner, int priority = 10)
        : _owner(owner), _priority(priority) {}
    
    // 析构函数
    ~UIEventObserver() override {
        _handlers.clear();
    }
    
    // 处理事件
    void onEvent(const GameEvent& event) override {
        // 检查所有者节点是否有效
        if (!_owner || _owner->isRunning() == false) {
            return;
        }
        
        // 查找对应的处理函数
        auto it = _handlers.find(event.type);
        if (it != _handlers.end()) {
            // 在UI线程中执行回调
            _owner->scheduleOnce([event, handler = it->second](float) {
                handler(event);
            }, 0, "event_handler");
        }
    }
    
    // 检查是否可以处理特定类型的事件
    bool canHandle(GameEventType eventType) const override {
        return _handlers.find(eventType) != _handlers.end();
    }
    
    // 获取优先级
    int getPriority() const override {
        return _priority;
    }
    
    // 设置优先级
    void setPriority(int priority) {
        _priority = priority;
    }
    
    // 注册事件处理器，支持链式调用
    UIEventObserver& on(GameEventType eventType, EventHandler handler) {
        _handlers[eventType] = std::move(handler);
        return *this;
    }
    
    // 移除事件处理器
    void off(GameEventType eventType) {
        _handlers.erase(eventType);
    }
    
    // 移除所有事件处理器
    void offAll() {
        _handlers.clear();
    }
    
    // 检查所有者是否有效
    bool isOwnerValid() const {
        return _owner != nullptr && _owner->isRunning();
    }
    
    // 获取所有者节点
    cocos2d::Node* getOwner() const {
        return _owner;
    }
    
private:
    cocos2d::Node* _owner; // 弱引用，不负责生命周期管理
    int _priority;
    std::map<GameEventType, EventHandler> _handlers;
};

#endif // EVENT_OBSERVER_H
