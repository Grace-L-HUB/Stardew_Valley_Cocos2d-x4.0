#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "GameState.h"
#include <unordered_map>
#include <functional>
#include <stack>

/**
 * 游戏状态管理器
 * 负责管理游戏状态的创建、转换、暂停和恢复
 */
class GameStateManager {
public:
    // 单例模式实现
    static GameStateManager* getInstance();
    static void destroyInstance();

    // 状态转换函数类型
    using StateCreator = std::function<std::shared_ptr<IGameState>()>;

    /**
     * 注册状态创建器
     */
    void registerStateCreator(GameStateType type, const StateCreator& creator);

    /**
     * 切换到指定状态
     */
    void changeState(GameStateType type, const std::string& transitionData = "");

    /**
     * 压入新状态（暂停当前状态）
     */
    void pushState(GameStateType type, const std::string& transitionData = "");

    /**
     * 弹出当前状态（恢复上一个状态）
     */
    void popState(const std::string& transitionData = "");

    /**
     * 获取当前状态
     */
    std::shared_ptr<IGameState> getCurrentState() const;

    /**
     * 获取当前状态类型
     */
    GameStateType getCurrentStateType() const;

    /**
     * 更新当前状态
     */
    void update(float dt);

    /**
     * 渲染当前状态
     */
    void render();

    /**
     * 处理输入事件
     */
    bool handleInput(cocos2d::Event* event);

    /**
     * 设置转场效果
     */
    void setTransitionType(const std::string& type, float duration = 0.5f);

    /**
     * 保存当前状态数据
     */
    bool saveCurrentState(const std::string& saveName);

    /**
     * 加载状态数据
     */
    bool loadStateData(const std::string& saveName, GameStateType type);

    /**
     * 清空所有状态
     */
    void clearAllStates();

    /**
     * 获取状态栈的大小
     */
    size_t getStateStackSize() const;

    /**
     * 检查状态是否在栈中
     */
    bool isStateInStack(GameStateType type) const;

    /**
     * 设置状态转换回调
     */
    void setStateTransitionCallback(std::function<void(const StateTransitionInfo&)> callback);

private:
    // 私有构造函数
    GameStateManager();
    ~GameStateManager();

    // 实现状态转换
    void performStateTransition(const StateTransitionInfo& transitionInfo);

    // 创建状态实例
    std::shared_ptr<IGameState> createState(GameStateType type);

    // 执行转场效果
    void executeTransition(const StateTransitionInfo& info);

    // 单例实例
    static GameStateManager* _instance;

    // 状态创建器映射
    std::unordered_map<GameStateType, StateCreator> _stateCreators;

    // 状态栈
    std::stack<std::shared_ptr<IGameState>> _stateStack;

    // 当前状态
    std::shared_ptr<IGameState> _currentState;

    // 默认转场类型
    std::string _defaultTransitionType;
    float _defaultTransitionDuration;

    // 状态转换回调
    std::function<void(const StateTransitionInfo&)> _transitionCallback;

    // 保存的状态数据
    std::unordered_map<std::string, std::string> _savedStates;

    // 是否正在进行状态转换
    bool _isTransitioning;
};

#endif // GAME_STATE_MANAGER_H