#include "EventManager.h"

#include "cocos2d.h"

extern std::string Season;

std::unique_ptr<EventManager> EventManager::s_instance = nullptr;

EventManager& EventManager::getInstance() {
    if (!s_instance) {
        s_instance = std::unique_ptr<EventManager>(new EventManager());
    }
    return *s_instance;
}

EventManager::~EventManager() {
    stopAsyncProcessing();
}

void EventManager::addObserver(const std::shared_ptr<IEventObserver>& observer) {
    if (!observer) {
        return;
    }
    std::lock_guard<std::mutex> lock(_observerMutex);
    _observers.push_back(observer);
    std::sort(_observers.begin(), _observers.end(),
              [](const std::shared_ptr<IEventObserver>& a,
                 const std::shared_ptr<IEventObserver>& b) {
                  if (!a || !b) {
                      return false;
                  }
                  return a->getPriority() < b->getPriority();
              });
}

void EventManager::removeObserver(const std::shared_ptr<IEventObserver>& observer) {
    if (!observer) {
        return;
    }
    std::lock_guard<std::mutex> lock(_observerMutex);
    _observers.erase(std::remove_if(_observers.begin(),
                                    _observers.end(),
                                    [&](const std::shared_ptr<IEventObserver>& entry) {
                                        return entry == observer;
                                    }),
                     _observers.end());
}

void EventManager::publishEvent(const GameEvent& event) {
    if (_asyncMode) {
        publishEventAsync(event);
    } else {
        publishEventSync(event);
    }
}

void EventManager::publishEvents(const std::vector<GameEvent>& events) {
    for (const auto& event : events) {
        publishEvent(event);
    }
}

void EventManager::publishPlayerLevelUp(int oldLevel, int newLevel, int skillPoints) {
    auto data = std::make_shared<PlayerLevelUpEventData>(oldLevel, newLevel, skillPoints);
    publishEvent(GameEvent(GameEventType::PLAYER_LEVEL_UP, data, "Player"));
}

void EventManager::publishGoldChanged(int oldAmount, int newAmount, const std::string& reason) {
    auto data = std::make_shared<GoldChangedEventData>(oldAmount, newAmount, reason);
    publishEvent(GameEvent(GameEventType::GOLD_AMOUNT_CHANGED, data, "EconomicSystem"));
}

void EventManager::publishItemAdded(const std::string& itemName, int quantity, int slotIndex) {
    auto data = std::make_shared<ItemEventData>(itemName, quantity, slotIndex);
    publishEvent(GameEvent(GameEventType::ITEM_ADDED, data, "Inventory"));
}

void EventManager::publishItemRemoved(const std::string& itemName, int quantity, int slotIndex) {
    auto data = std::make_shared<ItemEventData>(itemName, quantity, slotIndex);
    publishEvent(GameEvent(GameEventType::ITEM_REMOVED, data, "Inventory"));
}

void EventManager::publishItemBought(const std::string& itemName, int cost) {
    auto data = std::make_shared<ItemEventData>(itemName, 1, -1, -cost);
    publishEvent(GameEvent(GameEventType::ITEM_BOUGHT, data, "Store"));
}

void EventManager::publishItemSold(const std::string& itemName, int value) {
    auto data = std::make_shared<ItemEventData>(itemName, 1, -1, value);
    publishEvent(GameEvent(GameEventType::ITEM_SOLD, data, "Inventory"));
}

void EventManager::publishTaskCreated(const std::string& name, const std::string& npc) {
    auto data = std::make_shared<TaskEventData>(name, npc);
    publishEvent(GameEvent(GameEventType::TASK_CREATED, data, "TaskManagement"));
}

void EventManager::publishTaskAccepted(const std::string& name, const std::string& npc) {
    auto data = std::make_shared<TaskEventData>(name, npc);
    publishEvent(GameEvent(GameEventType::TASK_ACCEPTED, data, "TaskManagement"));
}

void EventManager::publishTaskCompleted(const std::string& name,
                                        const std::string& npc,
                                        int reward,
                                        float relationshipPoints) {
    auto data = std::make_shared<TaskEventData>(name, npc, reward, relationshipPoints);
    publishEvent(GameEvent(GameEventType::TASK_COMPLETED, data, "TaskManagement"));
}

void EventManager::publishCropEvent(GameEventType type,
                                    const std::string& crop,
                                    const cocos2d::Vec2& pos,
                                    int stage) {
    auto data = std::make_shared<CropEventData>(crop, pos, Season, stage);
    publishEvent(GameEvent(type, data, "Farm"));
}

void EventManager::publishRelationshipChanged(const std::string& npcA,
                                              const std::string& npcB,
                                              double value) {
    auto data = std::make_shared<RelationshipEventData>(npcA, npcB, value);
    publishEvent(GameEvent(GameEventType::NPC_RELATIONSHIP_CHANGED, data, "NpcRelationship"));
}

void EventManager::enableAsyncMode(bool enable) {
    if (enable && !_asyncMode) {
        startAsyncProcessing();
    } else if (!enable && _asyncMode) {
        stopAsyncProcessing();
    }
    _asyncMode = enable;
}

void EventManager::addEventFilter(GameEventType type, std::function<bool(const GameEvent&)> filter) {
    _filters[type] = std::move(filter);
}

int EventManager::getEventCount(GameEventType type) const {
    auto it = _eventCounts.find(type);
    return it != _eventCounts.end() ? it->second : 0;
}

std::vector<GameEvent> EventManager::getEventHistory(GameEventType type) const {
    if (type == static_cast<GameEventType>(-1)) {
        return _eventHistory;
    }

    std::vector<GameEvent> filtered;
    for (const auto& event : _eventHistory) {
        if (event.type == type) {
            filtered.push_back(event);
        }
    }
    return filtered;
}

void EventManager::clearHistory() {
    _eventHistory.clear();
    _eventCounts.clear();
}

void EventManager::publishEventSync(const GameEvent& event) {
    auto filterIt = _filters.find(event.type);
    if (filterIt != _filters.end() && !filterIt->second(event)) {
        return;
    }
    recordEvent(event);
    notifyObservers(event);
}

void EventManager::publishEventAsync(const GameEvent& event) {
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        _eventQueue.push(event);
    }
    _eventCondition.notify_one();
}

void EventManager::notifyObservers(const GameEvent& event) {
    std::lock_guard<std::mutex> lock(_observerMutex);
    _observers.erase(std::remove_if(_observers.begin(),
                                    _observers.end(),
                                    [&](const std::shared_ptr<IEventObserver>& observer) {
                                        if (!observer) {
                                            return true;
                                        }
                                        if (!observer->canHandle(event.type)) {
                                            return false;
                                        }
                                        try {
                                            observer->onEvent(event);
                                        } catch (const std::exception& ex) {
                                            cocos2d::log("Event observer failed: %s", ex.what());
                                        }
                                        return false;
                                    }),
                     _observers.end());
}

void EventManager::publishStateChanged(GameStateType oldState, GameStateType newState, 
                                     const std::string& transitionType, float duration) {
    auto data = std::make_shared<GameStateChangedEventData>(oldState, newState, transitionType, duration);
    publishEvent(GameEvent(GameEventType::GAME_STATE_CHANGED, data, "GameStateManager"));
}

void EventManager::publishTimeChanged(int day, int hour, const std::string& season) {
    auto data = std::make_shared<TimeChangedEventData>(day, hour, season);
    publishEvent(GameEvent(GameEventType::TIME_CHANGED, data, "TimeSystem"));
}

void EventManager::publishWeatherChanged(const std::string& oldWeather, const std::string& newWeather) {
    auto data = std::make_shared<WeatherChangedEventData>(oldWeather, newWeather);
    publishEvent(GameEvent(GameEventType::WEATHER_CHANGED, data, "WeatherSystem"));
}

void EventManager::publishPlayerEnergyChanged(int currentEnergy) {
    auto data = std::make_shared<PlayerEnergyChangedEventData>(currentEnergy);
    publishEvent(GameEvent(GameEventType::PLAYER_ENERGY_CHANGED, data, "Player"));
}

void EventManager::recordEvent(const GameEvent& event) {
    _eventCounts[event.type]++;
    if (_eventHistory.size() >= _maxHistorySize) {
        _eventHistory.erase(_eventHistory.begin());
    }
    _eventHistory.push_back(event);
}

void EventManager::startAsyncProcessing() {
    if (_running) {
        return;
    }
    _running = true;
    _eventThread = std::thread([this]() {
        while (_running) {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _eventCondition.wait(lock, [this]() { return !_eventQueue.empty() || !_running; });
            while (!_eventQueue.empty()) {
                auto event = _eventQueue.front();
                _eventQueue.pop();
                lock.unlock();
                publishEventSync(event);
                lock.lock();
            }
        }
    });
}

void EventManager::stopAsyncProcessing() {
    _running = false;
    _eventCondition.notify_all();
    if (_eventThread.joinable()) {
        _eventThread.join();
    }
}

