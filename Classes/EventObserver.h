#pragma once

#include "GameEvent.h"
#include <functional>
#include <map>
#include <memory>
#include <set>

class IEventObserver {
public:
    virtual ~IEventObserver() = default;
    virtual void onEvent(const GameEvent& event) = 0;
    virtual bool canHandle(GameEventType eventType) const = 0;
    virtual int getPriority() const { return 0; }
};

class EventObserver : public IEventObserver {
public:
    explicit EventObserver(std::function<void(const GameEvent&)> cb, int prio = 0)
        : _callback(std::move(cb)), _priority(prio) {}

    void onEvent(const GameEvent& event) override {
        if (_callback) {
            _callback(event);
        }
    }

    bool canHandle(GameEventType eventType) const override {
        return _supportedEvents.empty() || _supportedEvents.count(eventType) > 0;
    }

    int getPriority() const override { return _priority; }

    EventObserver& addSupportedEvent(GameEventType type) {
        _supportedEvents.insert(type);
        return *this;
    }

private:
    std::function<void(const GameEvent&)> _callback;
    std::set<GameEventType> _supportedEvents;
    int _priority;
};

class UIEventObserver : public IEventObserver {
public:
    explicit UIEventObserver(cocos2d::Node* owner, int prio = 10)
        : _owner(owner), _priority(prio) {}

    void onEvent(const GameEvent& event) override {
        auto it = _handlers.find(event.type);
        if (it != _handlers.end()) {
            it->second(event);
        }
    }

    bool canHandle(GameEventType eventType) const override {
        return _handlers.count(eventType) > 0;
    }

    int getPriority() const override { return _priority; }

    UIEventObserver& on(GameEventType type, std::function<void(const GameEvent&)> handler) {
        _handlers[type] = std::move(handler);
        return *this;
    }

private:
    cocos2d::Node* _owner;
    int _priority;
    std::map<GameEventType, std::function<void(const GameEvent&)>> _handlers;
};

