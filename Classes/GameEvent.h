#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <string>
#include <memory>
#include <vector>
#include "cocos2d.h"

// 游戏事件类型枚举
enum class GameEventType {
    // 玩家相关事件
    PLAYER_LEVEL_UP,
    PLAYER_ENERGY_CHANGED,
    PLAYER_HEALTH_CHANGED,
    PLAYER_MONEY_CHANGED,
    PLAYER_LOCATION_CHANGED,
    PLAYER_POSITION_CHANGED,
    PLAYER_STATE_CHANGED,
    
    // 经济系统事件
    GOLD_AMOUNT_CHANGED,
    ITEM_BOUGHT,
    ITEM_SOLD,
    BUY_FAILED,
    
    // 物品系统事件
    ITEM_ADDED,
    ITEM_REMOVED,
    ITEM_USED,
    ITEM_EQUIPPED,
    ITEM_UNEQUIPPED,
    
    // 任务系统事件
    TASK_CREATED,
    TASK_COMPLETED,
    TASK_STATUS_CHANGED,
    
    // 作物系统事件
    CROP_PLANTED,
    CROP_GROWN,
    CROP_HARVESTED,
    
    // NPC关系事件
    RELATIONSHIP_CHANGED,
    NPC_TALKED,
    GIFT_GIVEN,
    
    // 时间和天气事件
    TIME_CHANGED,
    SEASON_CHANGED,
    WEATHER_CHANGED,
    
    // 作物相关事件
    CROP_PHASE_CHANGED,
    CROP_WATERED,
    CROP_DEAD,
    
    // 场景和UI事件
    SCENE_CHANGED,
    UI_OPENED,
    UI_CLOSED,
    
    // 成就事件
    ACHIEVEMENT_UNLOCKED,
    
    // 其他事件
    GAME_SAVED,
    GAME_LOADED,
    GAME_STARTED,
    GAME_PAUSED,
    GAME_RESUMED
};

// 事件数据基类
class EventData {
public:
    virtual ~EventData() = default;
    virtual std::string toString() const = 0;
};

// 玩家升级事件数据
class PlayerLevelUpEventData : public EventData {
public:
    int oldLevel;
    int newLevel;
    int currentExp;
    int expToNextLevel;
    
    PlayerLevelUpEventData(int oldLevel, int newLevel, int currentExp, int expToNextLevel)
        : oldLevel(oldLevel), newLevel(newLevel), currentExp(currentExp), expToNextLevel(expToNextLevel) {}
    
    std::string toString() const override {
        return "PlayerLevelUp: " + std::to_string(oldLevel) + " -> " + std::to_string(newLevel) + 
               ", Exp: " + std::to_string(currentExp) + "/" + std::to_string(expToNextLevel);
    }
};

// 金钱变化事件数据
class GoldChangedEventData : public EventData {
public:
    int oldAmount;
    int newAmount;
    std::string reason;
    
    // 支持两个参数的构造函数（默认reason为空）
    GoldChangedEventData(int oldAmount, int newAmount, const std::string& reason = "")
        : oldAmount(oldAmount), newAmount(newAmount), reason(reason) {}
    
    std::string toString() const override {
        return "GoldChanged: " + std::to_string(oldAmount) + " -> " + std::to_string(newAmount) + 
               (reason.empty() ? "" : ", Reason: " + reason);
    }
};

// 物品事件数据
class ItemEventData : public EventData {
public:
    std::string itemId;
    int amount;
    int slot;
    
    ItemEventData(const std::string& itemId, int amount, int slot = -1)
        : itemId(itemId), amount(amount), slot(slot) {}
    
    std::string toString() const override {
        return "ItemEvent: " + itemId + ", Amount: " + std::to_string(amount) + 
               (slot != -1 ? ", Slot: " + std::to_string(slot) : "");
    }
};

// 任务事件数据
class TaskEventData : public EventData {
public:
    std::string taskId;
    std::string taskStatus;
    std::string taskDetails;
    
    TaskEventData(const std::string& taskId, const std::string& taskStatus, const std::string& taskDetails = "")
        : taskId(taskId), taskStatus(taskStatus), taskDetails(taskDetails) {}
    
    std::string toString() const override {
        return "TaskEvent: " + taskId + ", Status: " + taskStatus + 
               (!taskDetails.empty() ? ", Details: " + taskDetails : "");
    }
};

// 作物事件数据
class CropEventData : public EventData {
public:
    std::string cropId;
    int x, y;
    std::string cropStage;
    
    CropEventData(const std::string& cropId, int x, int y, const std::string& cropStage = "")
        : cropId(cropId), x(x), y(y), cropStage(cropStage) {}
    
    std::string toString() const override {
        return "CropEvent: " + cropId + ", Position: (" + std::to_string(x) + ", " + 
               std::to_string(y) + ")" + 
               (!cropStage.empty() ? ", Stage: " + cropStage : "");
    }
};

// 关系事件数据
class RelationshipEventData : public EventData {
public:
    std::string npcId;
    int changeAmount;
    int currentLevel;
    
    RelationshipEventData(const std::string& npcId, int changeAmount, int currentLevel)
        : npcId(npcId), changeAmount(changeAmount), currentLevel(currentLevel) {}
    
    std::string toString() const override {
        return "RelationshipEvent: " + npcId + ", Change: " + std::to_string(changeAmount) + 
               ", Level: " + std::to_string(currentLevel);
    }
};

// 玩家能量变化事件数据
class PlayerEnergyChangedEventData : public EventData {
public:
    int oldEnergy;
    int newEnergy;
    int maxEnergy;
    
    // 支持两个参数的构造函数（默认maxEnergy为100）
    PlayerEnergyChangedEventData(int oldEnergy, int newEnergy, int maxEnergy = 100)
        : oldEnergy(oldEnergy), newEnergy(newEnergy), maxEnergy(maxEnergy) {}
    
    std::string toString() const override {
        return "PlayerEnergyChanged: " + std::to_string(oldEnergy) + " -> " + 
               std::to_string(newEnergy) + "/" + std::to_string(maxEnergy);
    }
};

// 玩家位置变化事件数据
class PlayerPositionChangedEventData : public EventData {
public:
    cocos2d::Vec2 position;
    
    PlayerPositionChangedEventData(const cocos2d::Vec2& position)
        : position(position) {}
    
    std::string toString() const override {
        return "PlayerPositionChanged: (" + std::to_string(position.x) + ", " + 
               std::to_string(position.y) + ")";
    }
};

// 玩家状态变化事件数据
class PlayerStateChangedEventData : public EventData {
public:
    std::string state;
    bool value;
    
    PlayerStateChangedEventData(const std::string& state, bool value)
        : state(state), value(value) {}
    
    std::string toString() const override {
        return "PlayerStateChanged: " + state + " = " + (value ? "true" : "false");
    }
};

// 物品添加事件数据
class ItemAddedEventData : public EventData {
public:
    std::shared_ptr<Item> item;
    int count;
    
    ItemAddedEventData(std::shared_ptr<Item> item, int count)
        : item(item), count(count) {}
    
    std::string toString() const override {
        if (item) {
            return "ItemAdded: " + item->GetName() + ", Count: " + std::to_string(count);
        }
        return "ItemAdded: (null), Count: " + std::to_string(count);
    }
};

// 选中物品变化事件数据
class SelectedItemChangedEventData : public EventData {
public:
    int _oldPosition;
    int _newPosition;
    
    SelectedItemChangedEventData(int oldPosition, int newPosition)
        : _oldPosition(oldPosition), _newPosition(newPosition) {}
    
    std::string toString() const override {
        return "SelectedItemChanged: " + std::to_string(_oldPosition) + " -> " + std::to_string(_newPosition);
    }
};

// 物品移除事件数据
class ItemRemovedEventData : public EventData {
public:
    std::shared_ptr<Item> item;
    int count;
    bool isCompletelyRemoved;
    
    ItemRemovedEventData(std::shared_ptr<Item> item, int count, bool isCompletelyRemoved)
        : item(item), count(count), isCompletelyRemoved(isCompletelyRemoved) {}
    
    std::string toString() const override {
        if (item) {
            return "ItemRemoved: " + item->GetName() + ", Count: " + std::to_string(count) +
                   ", CompletelyRemoved: " + (isCompletelyRemoved ? "true" : "false");
        }
        return "ItemRemoved: (null), Count: " + std::to_string(count) +
               ", CompletelyRemoved: " + (isCompletelyRemoved ? "true" : "false");
    }
};


// 作物阶段变化事件数据
class CropPhaseChangedEventData : public EventData {
public:
    const class Crop& crop;
    int oldPhase;
    int newPhase;
    
    CropPhaseChangedEventData(const Crop& crop, int oldPhase, int newPhase)
        : crop(crop), oldPhase(oldPhase), newPhase(newPhase) {}
    
    std::string toString() const override {
        return "CropPhaseChanged: " + crop.GetName() + " (" + 
               std::to_string(oldPhase) + " -> " + std::to_string(newPhase) + ")";
    }
};

// 作物浇水事件数据
class CropWateredEventData : public EventData {
public:
    const class Crop& crop;
    
    CropWateredEventData(const Crop& crop)
        : crop(crop) {}
    
    std::string toString() const override {
        return "CropWatered: " + crop.GetName();
    }
};

// 作物收获事件数据
class CropHarvestedEventData : public EventData {
public:
    const class Crop& crop;
    int harvestedCount;
    
    CropHarvestedEventData(const Crop& crop, int harvestedCount)
        : crop(crop), harvestedCount(harvestedCount) {}
    
    std::string toString() const override {
        return "CropHarvested: " + crop.GetName() + ", Count: " + 
               std::to_string(harvestedCount);
    }
};

// 作物死亡事件数据
class CropDeadEventData : public EventData {
public:
    const class Crop& crop;
    std::string reason;
    
    CropDeadEventData(const Crop& crop, const std::string& reason)
        : crop(crop), reason(reason) {}
    
    std::string toString() const override {
        return "CropDead: " + crop.GetName() + ", Reason: " + reason;
    }
};

// 日变化事件数据
class DayChangedEventData : public EventData {
public:
    int newDay;
    std::string season;
    
    DayChangedEventData(int newDay, const std::string& season)
        : newDay(newDay), season(season) {}
    
    std::string toString() const override {
        return "DayChanged: Day " + std::to_string(newDay) + ", Season: " + season;
    }
};

// 季节变化事件数据
class SeasonChangedEventData : public EventData {
public:
    std::string oldSeason;
    std::string newSeason;
    int day;
    
    SeasonChangedEventData(const std::string& oldSeason, const std::string& newSeason, int day)
        : oldSeason(oldSeason), newSeason(newSeason), day(day) {}
    
    std::string toString() const override {
        return "SeasonChanged: " + oldSeason + " -> " + newSeason + ", Day: " + std::to_string(day);
    }
};

// 天气变化事件数据
class WeatherChangedEventData : public EventData {
public:
    std::string oldWeather;
    std::string newWeather;
    int duration;
    
    WeatherChangedEventData(const std::string& oldWeather, const std::string& newWeather, int duration = 1)
        : oldWeather(oldWeather), newWeather(newWeather), duration(duration) {}
    
    std::string toString() const override {
        return "WeatherChanged: " + oldWeather + " -> " + newWeather + 
               ", Duration: " + std::to_string(duration);
    }
};

// 时间变化事件数据
class TimeChangedEventData : public EventData {
public:
    int oldHour;
    int newHour;
    int day;
    int month;
    std::string season;
    
    TimeChangedEventData(int oldHour, int newHour, int day, int month, const std::string& season)
        : oldHour(oldHour), newHour(newHour), day(day), month(month), season(season) {}
    
    std::string toString() const override {
        return "TimeChanged: " + std::to_string(oldHour) + ":00 -> " + 
               std::to_string(newHour) + ":00, Day: " + std::to_string(day) + 
               ", Month: " + std::to_string(month) + ", Season: " + season;
    }
};

// 游戏事件包装类
class GameEvent {
public:
    GameEventType type;
    std::shared_ptr<EventData> data;
    float timestamp;
    std::string source;
    
    GameEvent(GameEventType type, std::shared_ptr<EventData> data, 
              const std::string& source = "unknown")
        : type(type), data(std::move(data)), timestamp(0.0f), source(source) {
        // 实际应用中，这里应该获取当前时间戳
    }
    
    template<typename T>
    std::shared_ptr<T> getData() const {
        return std::dynamic_pointer_cast<T>(data);
    }
};

#endif // GAME_EVENT_H
