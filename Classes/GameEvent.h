#pragma once

#include "cocos2d.h"
#include "GameState.h"
#include <any>
#include <map>
#include <memory>
#include <set>
#include <string>

// Enumerates all supported gameplay events.
enum class GameEventType {
    PLAYER_LEVEL_UP,
    PLAYER_HEALTH_CHANGED,
    PLAYER_ENERGY_CHANGED,
    PLAYER_POSITION_CHANGED,

    GOLD_AMOUNT_CHANGED,
    ITEM_BOUGHT,
    ITEM_SOLD,

    ITEM_ADDED,
    ITEM_REMOVED,
    ITEM_USED,
    INVENTORY_FULL,

    TASK_CREATED,
    TASK_ACCEPTED,
    TASK_COMPLETED,
    TASK_FAILED,

    CROP_PLANTED,
    CROP_WATERED,
    CROP_HARVESTED,
    CROP_GROWN,
    SEASON_CHANGED,
    DAY_CHANGED,
    TIME_CHANGED,
    WEATHER_CHANGED,

    NPC_RELATIONSHIP_CHANGED,
    NPC_DIALOG_STARTED,
    NPC_DIALOG_ENDED,

    UI_OPENED,
    UI_CLOSED,
    BUTTON_CLICKED,

    GAME_SAVED,
    GAME_LOADED,
    SETTINGS_CHANGED,
    GAME_STATE_CHANGED,

    CUSTOM_EVENT
};

class EventData {
public:
    virtual ~EventData() = default;
    virtual std::string toString() const { return "EventData"; }
};

class PlayerLevelUpEventData : public EventData {
public:
    int oldLevel;
    int newLevel;
    int skillPoints;

    PlayerLevelUpEventData(int oldLv, int newLv, int points)
        : oldLevel(oldLv), newLevel(newLv), skillPoints(points) {}

    std::string toString() const override {
        return "PlayerLevelUp: " + std::to_string(oldLevel) + " -> " + std::to_string(newLevel);
    }
};

class GoldChangedEventData : public EventData {
public:
    int oldAmount;
    int newAmount;
    int change;
    std::string reason;

    GoldChangedEventData(int oldAmt, int newAmt, const std::string& r)
        : oldAmount(oldAmt), newAmount(newAmt), change(newAmt - oldAmt), reason(r) {}

    std::string toString() const override {
        return "GoldChanged: " + std::to_string(change) + " (" + reason + ")";
    }
};

class ItemEventData : public EventData {
public:
    std::string itemName;
    int quantity;
    int slotIndex;
    int valueChange;

    ItemEventData(const std::string& name, int qty, int slot = -1, int valueDelta = 0)
        : itemName(name), quantity(qty), slotIndex(slot), valueChange(valueDelta) {}

    std::string toString() const override {
        return "Item: " + itemName + " x" + std::to_string(quantity);
    }
};

class PlayerEnergyChangedEventData : public EventData {
public:
    int energy;

    PlayerEnergyChangedEventData(int e)
        : energy(e) {}

    std::string toString() const override {
        return "PlayerEnergyChanged: " + std::to_string(energy);
    }
};

class GameStateChangedEventData : public EventData {
public:
    GameStateType oldState;
    GameStateType newState;
    std::string transitionType;
    float duration;

    GameStateChangedEventData(GameStateType oldSt, GameStateType newSt, const std::string& transType, float dur)
        : oldState(oldSt), newState(newSt), transitionType(transType), duration(dur) {}

    std::string toString() const override {
        return "GameStateChanged: " + std::to_string(static_cast<int>(oldState)) + " -> " + std::to_string(static_cast<int>(newState));
    }
};

class TimeChangedEventData : public EventData {
public:
    int day;
    int hour;
    std::string season;

    TimeChangedEventData(int d, int h, const std::string& s)
        : day(d), hour(h), season(s) {}

    std::string toString() const override {
        return "TimeChanged: Day " + std::to_string(day) + ", " + std::to_string(hour) + ":00, " + season;
    }
};

class WeatherChangedEventData : public EventData {
public:
    std::string oldWeather;
    std::string newWeather;

    WeatherChangedEventData(const std::string& oldW, const std::string& newW)
        : oldWeather(oldW), newWeather(newW) {}

    std::string toString() const override {
        return "WeatherChanged: " + oldWeather + " -> " + newWeather;
    }
};

class TaskEventData : public EventData {
public:
    std::string taskName;
    std::string npcName;
    int rewardCoins;
    float relationshipPoints;

    TaskEventData(const std::string& task,
                  const std::string& npc = "",
                  int reward = 0,
                  float points = 0.f)
        : taskName(task), npcName(npc), rewardCoins(reward), relationshipPoints(points) {}

    std::string toString() const override {
        return "Task: " + taskName + " NPC: " + npcName;
    }
};

class CropEventData : public EventData {
public:
    std::string cropType;
    cocos2d::Vec2 position;
    std::string season;
    int growthStage;

    CropEventData(const std::string& type,
                  const cocos2d::Vec2& pos,
                  const std::string& s = "",
                  int stage = 0)
        : cropType(type), position(pos), season(s), growthStage(stage) {}

    std::string toString() const override {
        return "Crop: " + cropType + " stage: " + std::to_string(growthStage);
    }
};

class RelationshipEventData : public EventData {
public:
    std::string npcA;
    std::string npcB;
    double newValue;

    RelationshipEventData(const std::string& a, const std::string& b, double value)
        : npcA(a), npcB(b), newValue(value) {}

    std::string toString() const override {
        return "Relationship: " + npcA + " <-> " + npcB + " = " + std::to_string(newValue);
    }
};

class GameEvent {
public:
    GameEventType type;
    std::shared_ptr<EventData> data;
    float timestamp;
    std::string source;

    GameEvent(GameEventType t,
              std::shared_ptr<EventData> d = nullptr,
              const std::string& src = "")
        : type(t), data(std::move(d)), source(src) {
        timestamp = cocos2d::utils::gettime();
    }

    template <typename T>
    std::shared_ptr<T> getData() const {
        return std::dynamic_pointer_cast<T>(data);
    }
};

