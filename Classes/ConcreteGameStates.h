#pragma once

#include "GameState.h"
#include "StateTransitionEffects.h"
#include "cocos2d.h"

class SceneStateBase : public IGameState {
public:
    void exit(const StateTransitionInfo& transition) override;
    void pause() override;
    void resume() override;
    void update(float) override {}
    void render() override {}

protected:
    void runSceneWithTransition(cocos2d::Scene* scene, const StateTransitionInfo& transition);
};

class MainMenuState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::MAIN_MENU; }
    std::string getName() const override { return "Main Menu"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class CharacterCreationState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::CHARACTER_CREATION; }
    std::string getName() const override { return "Character Creation"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class FarmState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::FARM; }
    std::string getName() const override { return "Farm"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class TownState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::TOWN; }
    std::string getName() const override { return "Town"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class ShopState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::SHOP; }
    std::string getName() const override { return "Shop"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class MyHouseState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::MYHOUSE; }
    std::string getName() const override { return "My House"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class BeachState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::BEACH; }
    std::string getName() const override { return "Beach"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class ForestState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::FOREST; }
    std::string getName() const override { return "Forest"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class CaveState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::CAVE; }
    std::string getName() const override { return "Cave"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class BarnState final : public SceneStateBase {
public:
    void enter(const StateTransitionInfo& transition) override;
    GameStateType getType() const override { return GameStateType::BARN; }
    std::string getName() const override { return "Barn"; }
    bool canTransitionTo(GameStateType targetState) const override;
};

class PausedState final : public IGameState {
public:
    void enter(const StateTransitionInfo& transition) override;
    void exit(const StateTransitionInfo& transition) override;
    void pause() override {}
    void resume() override {}
    void update(float) override {}
    void render() override {}
    bool handleInput(cocos2d::Event* event) override;

    GameStateType getType() const override { return GameStateType::PAUSED; }
    std::string getName() const override { return "Paused"; }
    bool canTransitionTo(GameStateType targetState) const override;

private:
    cocos2d::Layer* _overlay = nullptr;
    GameStateType _pausedFrom = GameStateType::STARTUP;
};

