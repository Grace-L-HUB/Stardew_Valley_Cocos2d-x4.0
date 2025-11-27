#pragma once

#include "EventObserver.h"
#include <algorithm>
#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class EventManager {
public:
    static EventManager& getInstance();

    void addObserver(const std::shared_ptr<IEventObserver>& observer);
    void removeObserver(const std::shared_ptr<IEventObserver>& observer);

    void publishEvent(const GameEvent& event);
    void publishEvents(const std::vector<GameEvent>& events);

    void publishPlayerLevelUp(int oldLevel, int newLevel, int skillPoints);
    void publishGoldChanged(int oldAmount, int newAmount, const std::string& reason);
    void publishItemAdded(const std::string& itemName, int quantity, int slotIndex);
    void publishItemRemoved(const std::string& itemName, int quantity, int slotIndex);
    void publishItemBought(const std::string& itemName, int cost);
    void publishItemSold(const std::string& itemName, int value);
    void publishTaskCreated(const std::string& name, const std::string& npc);
    void publishTaskAccepted(const std::string& name, const std::string& npc);
    void publishTaskCompleted(const std::string& name,
                              const std::string& npc,
                              int reward,
                              float relationshipPoints);
    void publishCropEvent(GameEventType type,
                          const std::string& crop,
                          const cocos2d::Vec2& pos,
                          int stage);
    void publishRelationshipChanged(const std::string& npcA,
                                    const std::string& npcB,
                                    double value);
    void publishStateChanged(GameStateType oldState,
                            GameStateType newState,
                            const std::string& transitionType,
                            float duration = 0.0f);
    void publishTimeChanged(int day, int hour, const std::string& season);
    void publishWeatherChanged(const std::string& oldWeather, const std::string& newWeather);
    void publishPlayerEnergyChanged(int currentEnergy);

    void enableAsyncMode(bool enable);

    void addEventFilter(GameEventType type, std::function<bool(const GameEvent&)> filter);
    int getEventCount(GameEventType type) const;
    std::vector<GameEvent> getEventHistory(GameEventType type = static_cast<GameEventType>(-1)) const;
    void clearHistory();

private:
    EventManager() = default;
    ~EventManager();
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    void publishEventSync(const GameEvent& event);
    void publishEventAsync(const GameEvent& event);
    void notifyObservers(const GameEvent& event);
    void recordEvent(const GameEvent& event);
    void startAsyncProcessing();
    void stopAsyncProcessing();

    std::vector<std::shared_ptr<IEventObserver>> _observers;
    std::queue<GameEvent> _eventQueue;
    mutable std::mutex _observerMutex;
    std::mutex _queueMutex;
    std::condition_variable _eventCondition;
    bool _asyncMode = false;
    bool _running = false;
    std::thread _eventThread;

    std::map<GameEventType, std::function<bool(const GameEvent&)>> _filters;
    std::map<GameEventType, int> _eventCounts;
    std::vector<GameEvent> _eventHistory;
    size_t _maxHistorySize = 1000;

    static std::unique_ptr<EventManager> s_instance;
};

