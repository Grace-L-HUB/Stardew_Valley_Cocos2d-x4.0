#pragma once

#include "cocos2d.h"
#include <map>
#include <memory>
#include <string>

enum class GameStateType {
    STARTUP,
    MAIN_MENU,
    LOADING,
    SAVING,
    PAUSED,
    SETTINGS,
    CHARACTER_CREATION,
    GAME_INTRO,
    FARM,
    TOWN,
    MYHOUSE,
    SHOP,
    BEACH,
    FOREST,
    CAVE,
    BARN,
    FISHING,
    DIALOGUE,
    INVENTORY,
    CRAFTING,
    SKILL_TREE,
    RELATIONSHIP,
    GAME_OVER,
    EXIT
};

struct StateTransitionInfo {
    GameStateType fromState;
    GameStateType toState;
    std::string transitionType;
    float duration;
    bool saveCurrentState;
    std::map<std::string, std::string> parameters;

    StateTransitionInfo(GameStateType from = GameStateType::STARTUP,
                        GameStateType to = GameStateType::STARTUP,
                        const std::string& type = "immediate",
                        float dur = 0.0f,
                        bool save = false)
        : fromState(from),
          toState(to),
          transitionType(type),
          duration(dur),
          saveCurrentState(save) {}
};

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void enter(const StateTransitionInfo& transition) = 0;
    virtual void exit(const StateTransitionInfo& transition) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;

    virtual GameStateType getType() const = 0;
    virtual std::string getName() const = 0;
    virtual bool canTransitionTo(GameStateType targetState) const = 0;

    virtual void saveState(std::map<std::string, std::string>&) const {}
    virtual void loadState(const std::map<std::string, std::string>&) {}
    virtual bool handleInput(cocos2d::Event*) { return false; }
};

