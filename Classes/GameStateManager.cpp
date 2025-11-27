#include "GameStateManager.h"
#include "GameStateFactory.h"
#include "cocos2d.h"
#include <cassert>
#include <iostream>
using namespace cocos2d;

// 初始化静态成员
GameStateManager* GameStateManager::_instance = nullptr;

GameStateManager::GameStateManager()
    : _defaultTransitionType("fade"),
      _defaultTransitionDuration(0.5f),
      _isTransitioning(false) {
    // 创建全局键盘事件监听器
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        // 忽略事件冒泡
        event->stopPropagation();
    };
    keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        // 将事件转发给当前状态处理
        if (!_stateStack.empty()) {
            auto currentState = _stateStack.top();
            currentState->handleInput(event);
        }
        // 忽略事件冒泡
        event->stopPropagation();
    };
    
    // 添加监听器到事件分发器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, Director::getInstance()->getRunningScene());
}

GameStateManager::~GameStateManager() {
    clearAllStates();
}

GameStateManager* GameStateManager::getInstance() {
    if (!_instance) {
        _instance = new GameStateManager();
    }
    return _instance;
}

void GameStateManager::destroyInstance() {
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

void GameStateManager::registerStateCreator(GameStateType type, const StateCreator& creator) {
    _stateCreators[type] = creator;
}

void GameStateManager::changeState(GameStateType type, const std::string& transitionData) {
    if (_isTransitioning) return;
    
    // 保存当前状态数据
    if (_currentState) {
        _currentState->exit();
    }
    
    // 创建新状态
    std::shared_ptr<IGameState> newState = createState(type);
    if (!newState) {
        std::cerr << "Failed to create state of type " << static_cast<int>(type) << std::endl;
        return;
    }
    
    // 清空状态栈并设置新状态
    clearAllStates();
    _currentState = newState;
    
    // 记录转换信息
    StateTransitionInfo transitionInfo;
    transitionInfo.fromState = GameStateType::MAIN_MENU; // 默认值
    if (!_stateStack.empty()) {
        transitionInfo.fromState = _stateStack.top()->getType();
    }
    transitionInfo.toState = type;
    transitionInfo.transitionType = _defaultTransitionType;
    transitionInfo.duration = _defaultTransitionDuration;
    transitionInfo.transitionData = transitionData;
    
    // 通知回调
    if (_transitionCallback) {
        _transitionCallback(transitionInfo);
    }
    
    // 设置当前状态并进入
    _stateStack.push(_currentState);
    _currentState->enter();
}

void GameStateManager::pushState(GameStateType type, const std::string& transitionData) {
    if (_isTransitioning) return;
    
    // 暂停当前状态
    if (_currentState) {
        _currentState->pause();
    }
    
    // 创建新状态
    std::shared_ptr<IGameState> newState = createState(type);
    if (!newState) {
        std::cerr << "Failed to create state of type " << static_cast<int>(type) << std::endl;
        return;
    }
    
    // 记录转换信息
    StateTransitionInfo transitionInfo;
    if (_currentState) {
        transitionInfo.fromState = _currentState->getType();
    }
    transitionInfo.toState = type;
    transitionInfo.transitionType = _defaultTransitionType;
    transitionInfo.duration = _defaultTransitionDuration;
    transitionInfo.transitionData = transitionData;
    
    // 通知回调
    if (_transitionCallback) {
        _transitionCallback(transitionInfo);
    }
    
    // 设置并进入新状态
    _currentState = newState;
    _currentState->enter();
    _stateStack.push(_currentState);
}

void GameStateManager::popState(const std::string& transitionData) {
    if (_isTransitioning || _stateStack.empty()) return;
    
    // 退出当前状态
    _currentState->exit();
    _stateStack.pop();
    
    // 恢复上一个状态
    if (!_stateStack.empty()) {
        _currentState = _stateStack.top();
        
        // 记录转换信息
        StateTransitionInfo transitionInfo;
        transitionInfo.fromState = _stateStack.top()->getType(); // 当前弹出的状态
        transitionInfo.toState = _currentState->getType();
        transitionInfo.transitionType = _defaultTransitionType;
        transitionInfo.duration = _defaultTransitionDuration;
        transitionInfo.transitionData = transitionData;
        
        // 通知回调
        if (_transitionCallback) {
            _transitionCallback(transitionInfo);
        }
        
        _currentState->resume();
    } else {
        _currentState = nullptr;
    }
}

std::shared_ptr<IGameState> GameStateManager::getCurrentState() const {
    return _currentState;
}

GameStateType GameStateManager::getCurrentStateType() const {
    if (_currentState) {
        return _currentState->getType();
    }
    return GameStateType::MAIN_MENU; // 默认返回主菜单类型
}

void GameStateManager::update(float dt) {
    if (_currentState && !_isTransitioning) {
        _currentState->update(dt);
    }
}

void GameStateManager::render() {
    if (_currentState && !_isTransitioning) {
        _currentState->render();
    }
}

bool GameStateManager::handleInput(cocos2d::Event* event) {
    if (_currentState && !_isTransitioning) {
        return _currentState->handleInput(event);
    }
    return false;
}

void GameStateManager::setTransitionType(const std::string& type, float duration) {
    _defaultTransitionType = type;
    _defaultTransitionDuration = duration;
}

bool GameStateManager::saveCurrentState(const std::string& saveName) {
    if (_currentState) {
        std::string stateData = _currentState->saveState();
        _savedStates[saveName] = stateData;
        return true;
    }
    return false;
}

bool GameStateManager::loadStateData(const std::string& saveName, GameStateType type) {
    auto it = _savedStates.find(saveName);
    if (it != _savedStates.end()) {
        std::shared_ptr<IGameState> state = createState(type);
        if (state) {
            state->loadState(it->second);
            changeState(type);
            return true;
        }
    }
    return false;
}

void GameStateManager::clearAllStates() {
    while (!_stateStack.empty()) {
        if (_stateStack.top()) {
            _stateStack.top()->exit();
        }
        _stateStack.pop();
    }
    _currentState = nullptr;
}

size_t GameStateManager::getStateStackSize() const {
    return _stateStack.size();
}

bool GameStateManager::isStateInStack(GameStateType type) const {
    // 遍历栈检查状态
    std::stack<std::shared_ptr<IGameState>> tempStack = _stateStack;
    while (!tempStack.empty()) {
        if (tempStack.top()->getType() == type) {
            return true;
        }
        tempStack.pop();
    }
    return false;
}

void GameStateManager::setStateTransitionCallback(std::function<void(const StateTransitionInfo&)> callback) {
    _transitionCallback = callback;
}

void GameStateManager::performStateTransition(const StateTransitionInfo& transitionInfo) {
    _isTransitioning = true;
    
    // 执行转场效果
    executeTransition(transitionInfo);
    
    // 完成后恢复
    _isTransitioning = false;
}

std::shared_ptr<IGameState> GameStateManager::createState(GameStateType type) {
    auto it = _stateCreators.find(type);
    if (it != _stateCreators.end()) {
        return it->second();
    }
    std::cerr << "No creator registered for state type " << static_cast<int>(type) << std::endl;
    return nullptr;
}

void GameStateManager::executeTransition(const StateTransitionInfo& info) {
    // 这里可以添加实际的转场效果实现
    // 目前只是简单记录
    std::cout << "Executing transition from " << static_cast<int>(info.fromState)
              << " to " << static_cast<int>(info.toState)
              << " with type: " << info.transitionType << std::endl;
}