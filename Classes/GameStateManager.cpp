#include "GameStateManager.h"

#include "cocos2d.h"

#include <algorithm>

std::unique_ptr<GameStateManager> GameStateManager::s_instance = nullptr;

GameStateManager& GameStateManager::getInstance() {
    if (!s_instance) {
        s_instance = std::unique_ptr<GameStateManager>(new GameStateManager());
    }
    return *s_instance;
}

void GameStateManager::addValidTransition(GameStateType from, GameStateType to) {
    _validTransitions[from].push_back(to);
}

void GameStateManager::setTransitionCallback(const std::function<void(GameStateType, GameStateType)>& callback) {
    _transitionCallback = callback;
}

bool GameStateManager::changeState(GameStateType newStateType,
                                   const std::string& transitionType,
                                   float duration,
                                   const std::map<std::string, std::string>& parameters,
                                   bool savePrevious) {
    if (!canTransitionTo(newStateType)) {
        return false;
    }

    auto nextState = createState(newStateType);
    if (!nextState) {
        return false;
    }

    StateTransitionInfo transition(getCurrentStateType(), newStateType, transitionType, duration, savePrevious);
    transition.parameters = parameters;

    return applyState(std::move(nextState), transition, true);
}

void GameStateManager::pushState(GameStateType newStateType,
                                 const std::string& transitionType,
                                 float duration,
                                 const std::map<std::string, std::string>& parameters) {
    auto nextState = createState(newStateType);
    if (!nextState) {
        return;
    }

    StateTransitionInfo transition(getCurrentStateType(), newStateType, transitionType, duration, false);
    transition.parameters = parameters;

    if (_currentState) {
        _currentState->pause();
        _pausedStates.push_back(std::move(_currentState));
    }

    applyState(std::move(nextState), transition, false);
}

void GameStateManager::popState(const std::string& transitionType, float duration) {
    if (_currentState) {
        StateTransitionInfo transition(_currentState->getType(),
                                       !_pausedStates.empty() ? _pausedStates.back()->getType() : GameStateType::STARTUP,
                                       transitionType,
                                       duration,
                                       false);
        _currentState->exit(transition);
    }

    if (_pausedStates.empty()) {
        _currentState.reset();
        return;
    }

    auto previous = std::move(_pausedStates.back());
    _pausedStates.pop_back();

    previous->resume();

    if (_transitionCallback) {
        _transitionCallback(GameStateType::PAUSED, previous->getType());
    }

    recordStateChange(previous->getType());
    _currentState = std::move(previous);
}

void GameStateManager::queueTransition(const StateTransitionInfo& transition) {
    _pendingTransitions.push(transition);
}

void GameStateManager::processQueuedTransitions() {
    while (!_pendingTransitions.empty()) {
        auto transition = _pendingTransitions.front();
        _pendingTransitions.pop();
        changeState(transition.toState, transition.transitionType, transition.duration, transition.parameters, transition.saveCurrentState);
    }
}

GameStateType GameStateManager::getCurrentStateType() const {
    return _currentState ? _currentState->getType() : GameStateType::STARTUP;
}

IGameState* GameStateManager::getCurrentState() const {
    return _currentState.get();
}

std::vector<GameStateType> GameStateManager::getStateHistory() const {
    return _history;
}

bool GameStateManager::canTransitionTo(GameStateType targetState) const {
    auto currentType = getCurrentStateType();
    return isValidTransition(currentType, targetState);
}

void GameStateManager::update(float deltaTime) {
    if (_currentState) {
        _currentState->update(deltaTime);
    }
}

void GameStateManager::render() {
    if (_currentState) {
        _currentState->render();
    }
}

bool GameStateManager::handleInput(cocos2d::Event* event) {
    if (_currentState) {
        return _currentState->handleInput(event);
    }
    return false;
}

void GameStateManager::saveCurrentState(const std::string& saveSlot) {
    if (!_currentState) {
        return;
    }
    std::map<std::string, std::string> data;
    _currentState->saveState(data);
    _savedSnapshots[saveSlot] = data;
}

bool GameStateManager::loadState(const std::string& saveSlot) {
    if (!_currentState) {
        return false;
    }
    auto it = _savedSnapshots.find(saveSlot);
    if (it == _savedSnapshots.end()) {
        return false;
    }
    _currentState->loadState(it->second);
    return true;
}

std::string GameStateManager::getDebugInfo() const {
    std::string info = "Current State: " + std::to_string(static_cast<int>(getCurrentStateType())) + "\nHistory: ";
    for (auto state : _history) {
        info += std::to_string(static_cast<int>(state)) + " ";
    }
    return info;
}

void GameStateManager::printStateHistory() const {
    cocos2d::log("%s", getDebugInfo().c_str());
}

std::unique_ptr<IGameState> GameStateManager::createState(GameStateType type) {
    auto it = _stateFactories.find(type);
    if (it == _stateFactories.end()) {
        cocos2d::log("GameStateManager: no factory registered for state %d", static_cast<int>(type));
        return nullptr;
    }
    return it->second();
}

void GameStateManager::recordStateChange(GameStateType newState) {
    _history.push_back(newState);
    if (_history.size() > _maxHistorySize) {
        _history.erase(_history.begin());
    }
}

bool GameStateManager::isValidTransition(GameStateType from, GameStateType to) const {
    auto it = _validTransitions.find(from);
    if (it == _validTransitions.end() || it->second.empty()) {
        return true;
    }
    return std::find(it->second.begin(), it->second.end(), to) != it->second.end();
}

bool GameStateManager::applyState(std::unique_ptr<IGameState> nextState,
                                  const StateTransitionInfo& transition,
                                  bool exitCurrent) {
    if (!nextState) {
        return false;
    }

    if (_currentState && exitCurrent) {
        if (transition.saveCurrentState) {
            saveCurrentState("auto");
        }
        _currentState->exit(transition);
    }

    auto newType = nextState->getType();
    nextState->enter(transition);

    if (_transitionCallback) {
        _transitionCallback(transition.fromState, newType);
    }

    recordStateChange(newType);
    _currentState = std::move(nextState);
    return true;
}

