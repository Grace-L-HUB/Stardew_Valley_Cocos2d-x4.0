#ifndef CONCRETE_GAME_STATES_H
#define CONCRETE_GAME_STATES_H

#include "GameState.h"
#include <memory>
#include <string>

// 前向声明 - 所有状态所需的类
class GameBeginUI;
class CreateCharacterUI;
class farm;
class town;
class shop;
class supermarket;
class Myhouse;
class Beach;
class Forest;
class Cave;
class Barn;

/**
 * 游戏状态基类，提供通用实现
 */
class BaseGameState : public IGameState {
public:
    BaseGameState(GameStateType type);
    virtual ~BaseGameState() override;

    // 类图中定义的核心方法
    virtual void enter() override;
    virtual void exit() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void update(float dt) override;
    
    // 扩展方法
    virtual void render() override;
    virtual bool handleInput(cocos2d::Event* event) override;
    virtual GameStateType getType() const override;
    virtual cocos2d::Scene* getScene() const override;
    virtual std::string saveState() const override;
    virtual void loadState(const std::string& stateData) override;

protected:
    GameStateType _stateType;
    cocos2d::Scene* _scene = nullptr;
    bool _isPaused = false;
    
    // 状态特定初始化
    virtual void initializeScene() = 0;
    virtual void cleanupScene();
    
    // 状态特定的输入处理
    virtual bool onHandleInput(cocos2d::Event* event) = 0;
};

/**
 * 主菜单状态 - 符合类图要求
 */
class MainMenuState : public BaseGameState {
public:
    MainMenuState();
    ~MainMenuState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    GameBeginUI* _mainMenuUI = nullptr;
};

/**
 * 农场状态 - 符合类图要求
 */
class FarmState : public BaseGameState {
public:
    FarmState();
    ~FarmState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    farm* _farmScene = nullptr;
};

/**
 * 小镇状态 - 符合类图要求
 */
class TownState : public BaseGameState {
public:
    TownState();
    ~TownState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    town* _townScene = nullptr;
};

/**
 * 商店状态 - 符合类图要求
 */
class ShopState : public BaseGameState {
public:
    ShopState();
    ~ShopState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    shop* _shopScene = nullptr;
};

/**
 * 暂停状态 - 符合类图要求
 */
class PausedState : public BaseGameState {
public:
    PausedState();
    ~PausedState() override;

    void enter() override;
    void exit() override;
    void pause() override;
    void resume() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    cocos2d::Layer* _pauseMenuLayer = nullptr;
    cocos2d::Label* _pauseLabel = nullptr;
    cocos2d::MenuItem* _resumeButton = nullptr;
    cocos2d::MenuItem* _saveButton = nullptr;
    cocos2d::MenuItem* _quitButton = nullptr;
    
    void createPauseMenu();
    void onResumeClicked();
    void onSaveClicked();
    void onQuitClicked();
};

/**
 * 角色创建状态 - 符合类图要求
 */
class CharacterCreationState : public BaseGameState {
public:
    CharacterCreationState();
    ~CharacterCreationState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    CreateCharacterUI* _characterCreationUI = nullptr;
};

/**
 * 超市状态 - 符合文档要求
 */
class SupermarketState : public BaseGameState {
public:
    SupermarketState();
    ~SupermarketState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    supermarket* _supermarketScene = nullptr;
};

/**
 * 家状态 - 符合文档要求
 */
class MyhouseState : public BaseGameState {
public:
    MyhouseState();
    ~MyhouseState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    Myhouse* _myhouseScene = nullptr;
};

/**
 * 沙滩状态 - 符合文档要求
 */
class BeachState : public BaseGameState {
public:
    BeachState();
    ~BeachState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    Beach* _beachScene = nullptr;
};

/**
 * 森林状态 - 符合文档要求
 */
class ForestState : public BaseGameState {
public:
    ForestState();
    ~ForestState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    Forest* _forestScene = nullptr;
};

/**
 * 洞穴状态 - 符合文档要求
 */
class CaveState : public BaseGameState {
public:
    CaveState();
    ~CaveState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    Cave* _caveScene = nullptr;
};

/**
 * 畜棚状态 - 符合文档要求
 */
class BarnState : public BaseGameState {
public:
    BarnState();
    ~BarnState() override;

protected:
    void initializeScene() override;
    bool onHandleInput(cocos2d::Event* event) override;
    
private:
    Barn* _barnScene = nullptr;
};

/**
 * 状态工厂类，用于创建所有游戏状态
 */
class GameStateFactory {
public:
    static std::shared_ptr<IGameState> createState(GameStateType type);
    static void registerAllStates();
};

#endif // CONCRETE_GAME_STATES_H