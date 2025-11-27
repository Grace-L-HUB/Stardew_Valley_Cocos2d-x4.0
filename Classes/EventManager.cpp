#include "EventManager.h"
#include <algorithm>
#include <chrono>

// 静态成员初始化
EventManager* EventManager::_instance = nullptr;

// 获取单例实例
EventManager* EventManager::getInstance() {
    if (!_instance) {
        _instance = new EventManager();
    }
    return _instance;
}

// 销毁单例实例
void EventManager::destroyInstance() {
    delete _instance;
    _instance = nullptr;
}

// 构造函数
EventManager::EventManager() 
    : _maxEventProcessingTime(0.0f)
    , _isProcessingEvents(false)
    , _needsResort(false) {
}

// 析构函数
EventManager::~EventManager() {
    _observers.clear();
    _eventQueue.clear();
    _tempEventQueue.clear();
}

// 注册观察者
bool EventManager::registerObserver(IEventObserver* observer) {
    if (!observer) {
        return false;
    }

    std::lock_guard<std::mutex> lock(_mutex);

    // 检查是否已经注册
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if (it != _observers.end()) {
        return false; // 已经注册过
    }

    // 添加观察者
    _observers.push_back(observer);
    _needsResort = true;

    return true;
}

// 注销观察者
bool EventManager::unregisterObserver(IEventObserver* observer) {
    if (!observer) {
        return false;
    }

    std::lock_guard<std::mutex> lock(_mutex);

    // 查找并移除观察者
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if (it != _observers.end()) {
        _observers.erase(it);
        return true;
    }

    return false; // 未找到该观察者
}

// 立即发送事件（支持智能指针版本）
void EventManager::dispatchEvent(const std::shared_ptr<GameEvent>& event) {
    if (event) {
        triggerEvent(*event);
    }
}

// 立即发送事件
bool EventManager::triggerEvent(const GameEvent& event) {
    if (!_isProcessingEvents) {
        std::lock_guard<std::mutex> lock(_mutex);
        
        // 如果需要重新排序，先排序
        if (_needsResort) {
            sortObserversByPriority();
            _needsResort = false;
        }

        // 创建观察者的副本，避免在通知过程中修改列表
        std::vector<IEventObserver*> observersCopy = _observers;

        // 释放锁，然后通知观察者
        _mutex.unlock();
        notifyObservers(event);
        _mutex.lock();
    } else {
        // 如果正在处理事件，将新事件加入临时队列
        _tempEventQueue.push_back(event);
    }

    return true;
}

// 队列事件，稍后处理
bool EventManager::queueEvent(const GameEvent& event) {
    std::lock_guard<std::mutex> lock(_mutex);
    _eventQueue.push_back(event);
    return true;
}

// 取消队列中的事件
bool EventManager::abortEvent(GameEventType eventType, bool allOfType) {
    std::lock_guard<std::mutex> lock(_mutex);

    bool success = false;
    auto it = _eventQueue.begin();
    
    while (it != _eventQueue.end()) {
        if (it->type == eventType) {
            it = _eventQueue.erase(it);
            success = true;
            
            if (!allOfType) {
                break; // 只删除第一个匹配的事件
            }
        } else {
            ++it;
        }
    }

    return success;
}

// 更新事件队列
void EventManager::update(float deltaTime) {
    std::lock_guard<std::mutex> lock(_mutex);

    // 如果需要重新排序，先排序
    if (_needsResort) {
        sortObserversByPriority();
        _needsResort = false;
    }

    // 清理无效观察者
    cleanupInvalidObservers();

    // 如果有临时队列中的事件，合并到主队列
    if (!_tempEventQueue.empty()) {
        _eventQueue.insert(_eventQueue.end(), _tempEventQueue.begin(), _tempEventQueue.end());
        _tempEventQueue.clear();
    }

    // 如果队列空，直接返回
    if (_eventQueue.empty()) {
        return;
    }

    // 记录开始处理时间
    auto startTime = std::chrono::high_resolution_clock::now();
    bool timeLimitExceeded = false;

    // 标记正在处理事件
    _isProcessingEvents = true;

    // 释放锁，开始处理事件
    _mutex.unlock();

    // 创建事件队列的副本，避免在处理过程中修改
    std::vector<GameEvent> eventsToProcess;
    {   
        std::lock_guard<std::mutex> tempLock(_mutex);
        eventsToProcess.swap(_eventQueue);
    }

    // 处理事件队列
    for (size_t i = 0; i < eventsToProcess.size() && !timeLimitExceeded; ++i) {
        notifyObservers(eventsToProcess[i]);

        // 检查是否超出最大处理时间
        if (_maxEventProcessingTime > 0.0f) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration<float, std::milli>(currentTime - startTime).count();
            if (elapsed > _maxEventProcessingTime) {
                // 将剩余的事件放回队列
                std::lock_guard<std::mutex> tempLock(_mutex);
                for (size_t j = i + 1; j < eventsToProcess.size(); ++j) {
                    _eventQueue.push_back(eventsToProcess[j]);
                }
                timeLimitExceeded = true;
            }
        }
    }

    // 重新获取锁
    _mutex.lock();
    
    // 处理完事件后，合并临时队列中可能新增的事件
    if (!_tempEventQueue.empty()) {
        _eventQueue.insert(_eventQueue.end(), _tempEventQueue.begin(), _tempEventQueue.end());
        _tempEventQueue.clear();
    }

    // 标记处理完成
    _isProcessingEvents = false;
}

// 清除所有事件队列
void EventManager::clearEvents() {
    std::lock_guard<std::mutex> lock(_mutex);
    _eventQueue.clear();
    _tempEventQueue.clear();
}

// 获取注册的观察者数量
size_t EventManager::getObserverCount() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _observers.size();
}

// 获取待处理事件数量
size_t EventManager::getQueuedEventCount() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _eventQueue.size() + _tempEventQueue.size();
}

// 按优先级排序观察者
void EventManager::sortObserversByPriority() {
    std::sort(_observers.begin(), _observers.end(), 
        [](const IEventObserver* a, const IEventObserver* b) {
            return a->getPriority() > b->getPriority(); // 优先级高的排在前面
        });
}

// 验证观察者是否有效
bool EventManager::isObserverValid(IEventObserver* observer) const {
    // 在实际应用中，这里可以添加更多的有效性检查
    return observer != nullptr;
}

// 批量注册多个观察者
bool EventManager::registerObservers(const std::vector<IEventObserver*>& observers) {
    bool allSuccess = true;
    
    for (auto observer : observers) {
        if (!registerObserver(observer)) {
            allSuccess = false;
        }
    }
    
    return allSuccess;
}

// 批量注销多个观察者
bool EventManager::unregisterObservers(const std::vector<IEventObserver*>& observers) {
    bool allSuccess = true;
    
    for (auto observer : observers) {
        if (!unregisterObserver(observer)) {
            allSuccess = false;
        }
    }
    
    return allSuccess;
}

// 设置事件处理的最大时间限制（毫秒）
void EventManager::setMaxEventProcessingTime(float milliseconds) {
    _maxEventProcessingTime = milliseconds;
}

// 获取事件处理的最大时间限制
float EventManager::getMaxEventProcessingTime() const {
    return _maxEventProcessingTime;
}

// 通知观察者处理事件
void EventManager::notifyObservers(const GameEvent& event) {
    std::vector<IEventObserver*> observersCopy;
    {   
        std::lock_guard<std::mutex> lock(_mutex);
        observersCopy = _observers;
    }

    for (auto observer : observersCopy) {
        if (observer && observer->canHandle(event.type)) {
            observer->onEvent(event);
        }
    }
}

// 检查并清理无效观察者
void EventManager::cleanupInvalidObservers() {
    auto it = std::remove_if(_observers.begin(), _observers.end(),
        [this](const IEventObserver* observer) {
            return !isObserverValid(const_cast<IEventObserver*>(observer));
        });
    
    _observers.erase(it, _observers.end());
}
