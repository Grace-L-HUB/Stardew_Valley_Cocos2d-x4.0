#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "EventObserver.h"
#include <mutex>
#include <vector>
#include <unordered_map>
#include <functional>

// 事件管理器接口
class IEventManager {
public:
    virtual ~IEventManager() = default;
    
    // 注册观察者
    virtual bool registerObserver(IEventObserver* observer) = 0;
    
    // 注销观察者
    virtual bool unregisterObserver(IEventObserver* observer) = 0;
    
    // 立即发送事件
    virtual bool triggerEvent(const GameEvent& event) = 0;
    
    // 立即发送事件（支持智能指针版本）
    virtual void dispatchEvent(const std::shared_ptr<GameEvent>& event) = 0;
    
    // 队列事件，稍后处理
    virtual bool queueEvent(const GameEvent& event) = 0;
    
    // 取消队列中的事件
    virtual bool abortEvent(GameEventType eventType, bool allOfType = false) = 0;
    
    // 更新事件队列
    virtual void update(float deltaTime) = 0;
    
    // 清除所有事件队列
    virtual void clearEvents() = 0;
    
    // 获取注册的观察者数量
    virtual size_t getObserverCount() const = 0;
    
    // 获取待处理事件数量
    virtual size_t getQueuedEventCount() const = 0;
};

// 事件管理器实现（单例模式）
class EventManager : public IEventManager {
public:
    // 获取单例实例
    static EventManager* getInstance();
    
    // 销毁单例实例
    static void destroyInstance();
    
    // 构造函数
    EventManager();
    
    // 析构函数
    ~EventManager() override;
    
    // 注册观察者
    bool registerObserver(IEventObserver* observer) override;
    
    // 注销观察者
    bool unregisterObserver(IEventObserver* observer) override;
    
    // 立即发送事件
    bool triggerEvent(const GameEvent& event) override;
    
    // 立即发送事件（支持智能指针版本）
    void dispatchEvent(const std::shared_ptr<GameEvent>& event) override;
    
    // 队列事件，稍后处理
    bool queueEvent(const GameEvent& event) override;
    
    // 取消队列中的事件
    bool abortEvent(GameEventType eventType, bool allOfType = false) override;
    
    // 更新事件队列
    void update(float deltaTime) override;
    
    // 清除所有事件队列
    void clearEvents() override;
    
    // 获取注册的观察者数量
    size_t getObserverCount() const override;
    
    // 获取待处理事件数量
    size_t getQueuedEventCount() const override;
    
    // 按优先级排序观察者
    void sortObserversByPriority();
    
    // 验证观察者是否有效
    bool isObserverValid(IEventObserver* observer) const;
    
    // 批量注册多个观察者
    bool registerObservers(const std::vector<IEventObserver*>& observers);
    
    // 批量注销多个观察者
    bool unregisterObservers(const std::vector<IEventObserver*>& observers);
    
    // 设置事件处理的最大时间限制（毫秒）
    void setMaxEventProcessingTime(float milliseconds);
    
    // 获取事件处理的最大时间限制
    float getMaxEventProcessingTime() const;
    
private:
    // 单例实例
    static EventManager* _instance;
    
    // 观察者列表，按优先级排序
    std::vector<IEventObserver*> _observers;
    
    // 事件队列
    std::vector<GameEvent> _eventQueue;
    
    // 用于事件处理的临时队列（避免在处理过程中修改原队列）
    std::vector<GameEvent> _tempEventQueue;
    
    // 互斥锁，保证线程安全
    mutable std::mutex _mutex;
    
    // 最大事件处理时间（毫秒）
    float _maxEventProcessingTime;
    
    // 标记是否正在处理事件
    bool _isProcessingEvents;
    
    // 通知观察者处理事件
    void notifyObservers(const GameEvent& event);
    
    // 检查并清理无效观察者
    void cleanupInvalidObservers();
    
    // 检查是否需要按优先级重新排序观察者
    bool _needsResort;
};

// 方便使用的宏定义
#define EVENT_MANAGER EventManager::getInstance()

// 事件触发辅助函数
inline bool TRIGGER_EVENT(GameEventType type, std::shared_ptr<EventData> data = nullptr) {
    return EVENT_MANAGER->triggerEvent(GameEvent(type, std::move(data)));
}

// 事件队列辅助函数
inline bool QUEUE_EVENT(GameEventType type, std::shared_ptr<EventData> data = nullptr) {
    return EVENT_MANAGER->queueEvent(GameEvent(type, std::move(data)));
}

// 便捷注册观察者的函数模板
template<typename ObserverType, typename... Args>
ObserverType* CREATE_AND_REGISTER_OBSERVER(Args&&... args) {
    ObserverType* observer = new ObserverType(std::forward<Args>(args)...);
    if (EVENT_MANAGER->registerObserver(observer)) {
        return observer;
    }
    delete observer;
    return nullptr;
}

#endif // EVENT_MANAGER_H
