#pragma once

#include "GameState.h"
#include "cocos2d.h"
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

class GameStateManager {
public:
    static GameStateManager& getInstance();

    template <typename StateClass>
    void registerState(GameStateType type) {
        _stateFactories[type] = []() -> std::unique_ptr<IGameState> {
            return std::make_unique<StateClass>();
        };
    }

    void addValidTransition(GameStateType from, GameStateType to);
    void setTransitionCallback(const std::function<void(GameStateType, GameStateType)>& callback);

    bool changeState(GameStateType newStateType,
                     const std::string& transitionType = "immediate",
                     float duration = 0.0f,
                     const std::map<std::string, std::string>& parameters = {},
                     bool savePrevious = false);

    void pushState(GameStateType newStateType,
                   const std::string& transitionType = "immediate",
                   float duration = 0.0f,
                   const std::map<std::string, std::string>& parameters = {});

    void popState(const std::string& transitionType = "immediate",
                  float duration = 0.0f);

    void queueTransition(const StateTransitionInfo& transition);
    void processQueuedTransitions();

    GameStateType getCurrentStateType() const;
    IGameState* getCurrentState() const;

    std::vector<GameStateType> getStateHistory() const;
    bool canTransitionTo(GameStateType targetState) const;

    void update(float deltaTime);
    void render();
    bool handleInput(cocos2d::Event* event);

    void saveCurrentState(const std::string& saveSlot = "default");
    bool loadState(const std::string& saveSlot = "default");

    std::string getDebugInfo() const;
    void printStateHistory() const;

private:
    GameStateManager() = default;

    std::unique_ptr<IGameState> createState(GameStateType type);
    void recordStateChange(GameStateType newState);
    bool isValidTransition(GameStateType from, GameStateType to) const;

    bool applyState(std::unique_ptr<IGameState> nextState,
                    const StateTransitionInfo& transition,
                    bool exitCurrent);

    static std::unique_ptr<GameStateManager> s_instance;

    std::unique_ptr<IGameState> _currentState;
    std::vector<std::unique_ptr<IGameState>> _pausedStates;
    std::queue<StateTransitionInfo> _pendingTransitions;
    std::map<GameStateType, std::function<std::unique_ptr<IGameState>()>> _stateFactories;
    std::map<GameStateType, std::vector<GameStateType>> _validTransitions;
    std::vector<GameStateType> _history;
    size_t _maxHistorySize = 64;
    std::function<void(GameStateType, GameStateType)> _transitionCallback;
    std::map<std::string, std::map<std::string, std::string>> _savedSnapshots;
};

