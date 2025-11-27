#include "ConcreteGameStates.h"

#include "GameStateManager.h"
#include "GameBeginUI.h"
#include "CreateCharacterUI.h"
#include "farm.h"
#include "Town.h"
#include "supermarket.h"
#include "Myhouse.h"
#include "Beach.h"
#include "Forest.h"
#include "Cave.h"
#include "Barn.h"

using namespace cocos2d;

void SceneStateBase::exit(const StateTransitionInfo&) {}

void SceneStateBase::pause() {
    auto scene = Director::getInstance()->getRunningScene();
    if (scene) {
        scene->pause();
    }
}

void SceneStateBase::resume() {
    auto scene = Director::getInstance()->getRunningScene();
    if (scene) {
        scene->resume();
    }
}

void SceneStateBase::runSceneWithTransition(Scene* scene, const StateTransitionInfo& transition) {
    if (!scene) {
        return;
    }

    auto director = Director::getInstance();
    auto running = director->getRunningScene();

    if (!running) {
        director->runWithScene(scene);
        return;
    }

    if (auto transitionScene = StateTransitionEffects::createTransition(
            transition.transitionType, scene, transition.duration)) {
        director->replaceScene(transitionScene);
    } else {
        director->replaceScene(scene);
    }
}

void MainMenuState::enter(const StateTransitionInfo& transition) {
    auto scene = BeginScene::create();
    runSceneWithTransition(scene, transition);
}

bool MainMenuState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::CHARACTER_CREATION ||
           targetState == GameStateType::SETTINGS ||
           targetState == GameStateType::EXIT;
}

void CharacterCreationState::enter(const StateTransitionInfo& transition) {
    auto scene = CreateCharacter::create();
    runSceneWithTransition(scene, transition);
}

bool CharacterCreationState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::FARM ||
           targetState == GameStateType::MAIN_MENU;
}

void FarmState::enter(const StateTransitionInfo& transition) {
    auto scene = farm::create();
    runSceneWithTransition(scene, transition);
}

bool FarmState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::TOWN ||
           targetState == GameStateType::MYHOUSE ||
           targetState == GameStateType::BARN ||
           targetState == GameStateType::FOREST ||
           targetState == GameStateType::CAVE ||
           targetState == GameStateType::BEACH ||
           targetState == GameStateType::PAUSED ||
           targetState == GameStateType::MAIN_MENU;
}

void TownState::enter(const StateTransitionInfo& transition) {
    auto scene = Town::create();
    runSceneWithTransition(scene, transition);
}

bool TownState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::FARM ||
           targetState == GameStateType::SHOP ||
           targetState == GameStateType::BEACH ||
           targetState == GameStateType::FOREST ||
           targetState == GameStateType::PAUSED ||
           targetState == GameStateType::MAIN_MENU;
}

void ShopState::enter(const StateTransitionInfo& transition) {
    auto scene = supermarket::create();
    runSceneWithTransition(scene, transition);
}

bool ShopState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::TOWN ||
           targetState == GameStateType::INVENTORY ||
           targetState == GameStateType::PAUSED ||
           targetState == GameStateType::MAIN_MENU;
}

void MyHouseState::enter(const StateTransitionInfo& transition) {
    auto scene = Myhouse::create();
    runSceneWithTransition(scene, transition);
}

bool MyHouseState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::FARM ||
           targetState == GameStateType::TOWN ||
           targetState == GameStateType::PAUSED ||
           targetState == GameStateType::MAIN_MENU;
}

void BeachState::enter(const StateTransitionInfo& transition) {
    auto scene = Beach::create();
    runSceneWithTransition(scene, transition);
}

bool BeachState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::TOWN ||
           targetState == GameStateType::FARM ||
           targetState == GameStateType::PAUSED;
}

void ForestState::enter(const StateTransitionInfo& transition) {
    auto scene = Forest::create();
    runSceneWithTransition(scene, transition);
}

bool ForestState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::FARM ||
           targetState == GameStateType::TOWN ||
           targetState == GameStateType::CAVE ||
           targetState == GameStateType::PAUSED;
}

void CaveState::enter(const StateTransitionInfo& transition) {
    auto scene = Cave::create();
    runSceneWithTransition(scene, transition);
}

bool CaveState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::FARM ||
           targetState == GameStateType::FOREST ||
           targetState == GameStateType::PAUSED;
}

void BarnState::enter(const StateTransitionInfo& transition) {
    auto scene = Barn::create();
    runSceneWithTransition(scene, transition);
}

bool BarnState::canTransitionTo(GameStateType targetState) const {
    return targetState == GameStateType::FARM ||
           targetState == GameStateType::PAUSED;
}

void PausedState::enter(const StateTransitionInfo& transition) {
    _pausedFrom = transition.fromState;

    auto currentScene = Director::getInstance()->getRunningScene();
    if (!currentScene) {
        return;
    }

    _overlay = Layer::create();
    auto bg = LayerColor::create(Color4B(0, 0, 0, 160));
    _overlay->addChild(bg);

    auto label = Label::createWithTTF("PAUSED", "fonts/Marker Felt.ttf", 48);
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    _overlay->addChild(label);

    currentScene->addChild(_overlay, INT_MAX);
}

void PausedState::exit(const StateTransitionInfo&) {
    if (_overlay) {
        _overlay->removeFromParent();
        _overlay = nullptr;
    }
}

bool PausedState::handleInput(Event* event) {
    auto keyboardEvent = dynamic_cast<EventKeyboard*>(event);
    if (!keyboardEvent) {
        return false;
    }

    if (keyboardEvent->getKeyCode() == EventKeyboard::KeyCode::KEY_ESCAPE ||
        keyboardEvent->getKeyCode() == EventKeyboard::KeyCode::KEY_P) {
        GameStateManager::getInstance().popState();
        return true;
    }
    return false;
}

bool PausedState::canTransitionTo(GameStateType targetState) const {
    return targetState == _pausedFrom ||
           targetState == GameStateType::MAIN_MENU ||
           targetState == GameStateType::SETTINGS;
}

