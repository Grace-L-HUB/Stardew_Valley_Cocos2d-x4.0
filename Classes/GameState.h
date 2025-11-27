#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>
#include <memory>
#include "cocos2d.h"

/**
 * 游戏状态类型枚举
 */
enum class GameStateType {
    MAIN_MENU,          // 主菜单状态
    CHARACTER_CREATION, // 角色创建状态
    FARM,               // 农场状态
    TOWN,               // 小镇状态
    SHOP,               // 商店状态
    MYHOUSE,            // 家状态
    FOREST,             // 森林状态
    BEACH,              // 沙滩状态
    CAVE,               // 洞穴状态
    BARN,               // 畜棚状态
    SUPERMARKET,        // 超市状态
    PAUSED              // 暂停状态
};

/**
 * 状态转换信息
 */
struct StateTransitionInfo {
    GameStateType fromState;      // 源状态
    GameStateType toState;        // 目标状态
    std::string transitionType;   // 转场效果类型 ("fade", "slide_left", "slide_right", "door_open"等)
    float duration;              // 转场持续时间（秒）
    std::string transitionData;  // 额外的转场数据
    bool shouldSaveState;        // 是否保存当前状态

    StateTransitionInfo(GameStateType from = GameStateType::MAIN_MENU,
                       GameStateType to = GameStateType::MAIN_MENU,
                       const std::string& type = "fade",
                       float dur = 0.5f,
                       const std::string& data = "",
                       bool saveState = false)
        : fromState(from), toState(to), transitionType(type),
          duration(dur), transitionData(data), shouldSaveState(saveState) {}
};

/**
 * 游戏状态抽象接口
 */
class IGameState : public std::enable_shared_from_this<IGameState> {
public:
    virtual ~IGameState() = default;

    /**
     * 进入状态时调用
     */
    virtual void enter() = 0;

    /**
     * 退出状态时调用
     */
    virtual void exit() = 0;

    /**
     * 暂停状态时调用
     */
    virtual void pause() = 0;

    /**
     * 恢复状态时调用
     */
    virtual void resume() = 0;

    /**
     * 更新状态逻辑
     */
    virtual void update(float dt) = 0;

    /**
     * 渲染状态
     */
    virtual void render() = 0;

    /**
     * 处理输入
     */
    virtual bool handleInput(cocos2d::Event* event) = 0;

    /**
     * 获取状态类型
     */
    virtual GameStateType getType() const = 0;

    /**
     * 获取关联的场景
     */
    virtual cocos2d::Scene* getScene() const = 0;

    /**
     * 保存状态数据
     */
    virtual std::string saveState() const = 0;

    /**
     * 加载状态数据
     */
    virtual void loadState(const std::string& stateData) = 0;
};

#endif // GAME_STATE_H