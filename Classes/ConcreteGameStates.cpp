#include "ConcreteGameStates.h"
#include "GameStateManager.h"
#include "GameBeginUI.h"
#include "CreateCharacterUI.h"
#include "farm.h"
#include "town.h"
#include "shop.h"
#include "supermarket.h"
#include "Myhouse.h"
#include "Beach.h"
#include "Forest.h"
#include "Cave.h"
#include "Barn.h"
#include <iostream>

// ========== BaseGameState ==========

BaseGameState::BaseGameState(GameStateType type) : _stateType(type) {
}

BaseGameState::~BaseGameState() {
    cleanupScene();
}

// 类图中定义的核心方法实现
void BaseGameState::enter() {
    std::cout << "Entering state: " << static_cast<int>(_stateType) << std::endl;
    
    if (!_scene) {
        initializeScene();
    }
    
    _isPaused = false;
    
    if (_scene) {
        cocos2d::Director::getInstance()->replaceScene(_scene);
    }
}

void BaseGameState::exit() {
    std::cout << "Exiting state: " << static_cast<int>(_stateType) << std::endl;
    _isPaused = false;
}

void BaseGameState::pause() {
    std::cout << "Pausing state: " << static_cast<int>(_stateType) << std::endl;
    _isPaused = true;
}

void BaseGameState::resume() {
    std::cout << "Resuming state: " << static_cast<int>(_stateType) << std::endl;
    _isPaused = false;
}

void BaseGameState::update(float dt) {
    if (_isPaused || !_scene) return;
    
    // 状态更新逻辑由具体子类实现
    _scene->update(dt);
}

// 扩展方法实现
void BaseGameState::render() {
    if (!_scene) return;
    
    // 渲染由Cocos2d-x引擎处理
}

bool BaseGameState::handleInput(cocos2d::Event* event) {
    if (_isPaused) return false;
    return onHandleInput(event);
}

GameStateType BaseGameState::getType() const {
    return _stateType;
}

cocos2d::Scene* BaseGameState::getScene() const {
    return _scene;
}

std::string BaseGameState::saveState() const {
    // 基础状态保存，由具体子类扩展
    return "";
}

void BaseGameState::loadState(const std::string& stateData) {
    // 基础状态加载，由具体子类扩展
}

void BaseGameState::cleanupScene() {
    if (_scene) {
        _scene->removeAllChildrenWithCleanup(true);
        _scene = nullptr;
    }
}

// ========== MainMenuState ==========

MainMenuState::MainMenuState() : BaseGameState(GameStateType::MAIN_MENU) {
}

MainMenuState::~MainMenuState() {
}

void MainMenuState::initializeScene() {
    _scene = GameBeginUI::createScene();
    if (_scene) {
        _mainMenuUI = dynamic_cast<GameBeginUI*>(_scene->getChildByName("GameBeginUI"));
    }
}

bool MainMenuState::onHandleInput(cocos2d::Event* event) {
    // 主菜单输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_SPACE) {
            // 空格键开始新游戏
            GameStateManager::getInstance()->changeState(GameStateType::CHARACTER_CREATION);
            return true;
        }
    }
    return false;
}



// ========== FarmState ==========

FarmState::FarmState() : BaseGameState(GameStateType::FARM) {
}

FarmState::~FarmState() {
}

void FarmState::initializeScene() {
    _scene = farm::createScene();
    if (_scene) {
        _farmScene = dynamic_cast<farm*>(_scene->getChildByName("farm"));
    }
}

bool FarmState::onHandleInput(cocos2d::Event* event) {
    // 农场状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            // ESC键暂停游戏
            GameStateManager::getInstance()->pushState(GameStateType::PAUSED);
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_T) {
            // T键切换到小镇
            GameStateManager::getInstance()->changeState(GameStateType::TOWN, "from_farm");
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_H) {
            // H键回家
            GameStateManager::getInstance()->changeState(GameStateType::MYHOUSE, "from_farm");
            return true;
        }
    }
    return false;
}

// ========== TownState ==========

TownState::TownState() : BaseGameState(GameStateType::TOWN) {
}

TownState::~TownState() {
}

void TownState::initializeScene() {
    _scene = town::createScene();
    if (_scene) {
        _townScene = dynamic_cast<town*>(_scene->getChildByName("town"));
    }
}

bool TownState::onHandleInput(cocos2d::Event* event) {
    // 小镇状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            GameStateManager::getInstance()->pushState(GameStateType::PAUSED);
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_F) {
            // F键返回农场
            GameStateManager::getInstance()->changeState(GameStateType::FARM, "from_town");
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_S) {
            // S键进入商店
            GameStateManager::getInstance()->changeState(GameStateType::SHOP, "from_town");
            return true;
        }
    }
    return false;
}

// ========== ShopState ==========

ShopState::ShopState() : BaseGameState(GameStateType::SHOP) {
}

ShopState::~ShopState() {
}

void ShopState::initializeScene() {
    _scene = shop::createScene();
    if (_scene) {
        _shopScene = dynamic_cast<shop*>(_scene->getChildByName("shop"));
    }
}

bool ShopState::onHandleInput(cocos2d::Event* event) {
    // 商店状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            // ESC键返回小镇
            GameStateManager::getInstance()->changeState(GameStateType::TOWN, "from_shop");
            return true;
        }
    }
    return false;
}



// ========== PausedState ==========

PausedState::PausedState() : BaseGameState(GameStateType::PAUSED) {
}

PausedState::~PausedState() {
}

void PausedState::enter() {
    std::cout << "Entering paused state" << std::endl;
    _isPaused = false;
    
    if (!_pauseMenuLayer) {
        createPauseMenu();
    }
    
    // 不替换场景，而是添加暂停菜单层
    if (_pauseMenuLayer && !_pauseMenuLayer->getParent()) {
        auto director = cocos2d::Director::getInstance();
        auto runningScene = director->getRunningScene();
        if (runningScene) {
            runningScene->addChild(_pauseMenuLayer, 1000);
        }
    }
}

void PausedState::exit() {
    std::cout << "Exiting paused state" << std::endl;
    
    if (_pauseMenuLayer) {
        _pauseMenuLayer->removeFromParent();
    }
}

void PausedState::pause() {
    // 暂停状态不应该被再次暂停
    std::cout << "Warning: Attempting to pause paused state" << std::endl;
}

void PausedState::resume() {
    // 暂停状态不需要恢复
    std::cout << "Warning: Attempting to resume paused state" << std::endl;
}

void PausedState::initializeScene() {
    // 暂停状态不需要完整场景
    _scene = nullptr;
    createPauseMenu();
}

bool PausedState::onHandleInput(cocos2d::Event* event) {
    // 暂停菜单输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            // ESC键恢复游戏
            onResumeClicked();
            return true;
        }
    }
    return false;
}

void PausedState::createPauseMenu() {
    _pauseMenuLayer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 180));
    
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    // 暂停标题
    _pauseLabel = cocos2d::Label::createWithTTF("游戏暂停", "fonts/arial.ttf", 48);
    _pauseLabel->setPosition(visibleSize.width / 2, visibleSize.height * 0.7f);
    _pauseLabel->setColor(cocos2d::Color3B::WHITE);
    _pauseMenuLayer->addChild(_pauseLabel);
    
    // 恢复按钮
    _resumeButton = cocos2d::MenuItemFont::create("继续游戏", CC_CALLBACK_1(PausedState::onResumeClicked, this));
    _resumeButton->setPosition(visibleSize.width / 2, visibleSize.height * 0.5f);
    
    // 保存按钮
    _saveButton = cocos2d::MenuItemFont::create("保存游戏", CC_CALLBACK_1(PausedState::onSaveClicked, this));
    _saveButton->setPosition(visibleSize.width / 2, visibleSize.height * 0.4f);
    
    // 退出按钮
    _quitButton = cocos2d::MenuItemFont::create("退出游戏", CC_CALLBACK_1(PausedState::onQuitClicked, this));
    _quitButton->setPosition(visibleSize.width / 2, visibleSize.height * 0.3f);
    
    // 菜单
    auto menu = cocos2d::Menu::create(_resumeButton, _saveButton, _quitButton, nullptr);
    menu->setPosition(0, 0);
    _pauseMenuLayer->addChild(menu);
    
    // 添加触摸监听器防止事件穿透
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](cocos2d::Touch*, cocos2d::Event*) { return true; };
    _pauseMenuLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _pauseMenuLayer);
}

void PausedState::onResumeClicked(cocos2d::Ref*) {
    GameStateManager::getInstance()->popState();
}

void PausedState::onSaveClicked(cocos2d::Ref*) {
    // 保存当前游戏状态
    GameStateManager::getInstance()->saveCurrentState("auto_save");
    
    // 显示保存成功提示
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto saveMessage = cocos2d::Label::createWithTTF("保存成功", "fonts/arial.ttf", 36);
    saveMessage->setPosition(visibleSize.width / 2, visibleSize.height * 0.2f);
    saveMessage->setColor(cocos2d::Color3B::GREEN);
    _pauseMenuLayer->addChild(saveMessage);
    
    // 2秒后移除提示
    saveMessage->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(2.0f),
        cocos2d::RemoveSelf::create(),
        nullptr
    ));
}

void PausedState::onQuitClicked(cocos2d::Ref*) {
    // 回到主菜单
    GameStateManager::getInstance()->clearAllStates();
    GameStateManager::getInstance()->changeState(GameStateType::MAIN_MENU);
}

// ========== CharacterCreationState ==========

CharacterCreationState::CharacterCreationState() : BaseGameState(GameStateType::CHARACTER_CREATION) {
}

CharacterCreationState::~CharacterCreationState() {
}

void CharacterCreationState::initializeScene() {
    _scene = CreateCharacterUI::createScene();
    if (_scene) {
        _characterCreationUI = dynamic_cast<CreateCharacterUI*>(_scene->getChildByName("CreateCharacterUI"));
    }
}

bool CharacterCreationState::onHandleInput(cocos2d::Event* event) {
    // 角色创建状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            // ESC键返回主菜单
            GameStateManager::getInstance()->changeState(GameStateType::MAIN_MENU);
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ENTER) {
            // Enter键确认创建，进入农场
            GameStateManager::getInstance()->changeState(GameStateType::FARM);
            return true;
        }
    }
    return false;
}

// ========== SupermarketState ==========

SupermarketState::SupermarketState() : BaseGameState(GameStateType::SUPERMARKET) {
}

SupermarketState::~SupermarketState() {
}

void SupermarketState::initializeScene() {
    _scene = supermarket::createScene();
    if (_scene) {
        _supermarketScene = dynamic_cast<supermarket*>(_scene->getChildByName("supermarket"));
    }
}

bool SupermarketState::onHandleInput(cocos2d::Event* event) {
    // 超市状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            // ESC键暂停游戏
            GameStateManager::getInstance()->pushState(GameStateType::PAUSED);
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_T) {
            // T键返回小镇
            GameStateManager::getInstance()->changeState(GameStateType::TOWN, "from_supermarket");
            return true;
        }
    }
    return false;
}

// ========== MyhouseState ==========

MyhouseState::MyhouseState() : BaseGameState(GameStateType::MYHOUSE) {
}

MyhouseState::~MyhouseState() {
}

void MyhouseState::initializeScene() {
    _scene = Myhouse::createScene();
    if (_scene) {
        _myhouseScene = dynamic_cast<Myhouse*>(_scene->getChildByName("Myhouse"));
    }
}

bool MyhouseState::onHandleInput(cocos2d::Event* event) {
    // 家状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            // ESC键暂停游戏
            GameStateManager::getInstance()->pushState(GameStateType::PAUSED);
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_F) {
            // F键返回农场
            GameStateManager::getInstance()->changeState(GameStateType::FARM, "from_myhouse");
            return true;
        }
    }
    return false;
}

// ========== BeachState ==========

BeachState::BeachState() : BaseGameState(GameStateType::BEACH) {
}

BeachState::~BeachState() {
}

void BeachState::initializeScene() {
    _scene = Beach::createScene();
    if (_scene) {
        _beachScene = dynamic_cast<Beach*>(_scene->getChildByName("Beach"));
    }
}

bool BeachState::onHandleInput(cocos2d::Event* event) {
    // 沙滩状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            // ESC键暂停游戏
            GameStateManager::getInstance()->pushState(GameStateType::PAUSED);
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_T) {
            // T键返回小镇
            GameStateManager::getInstance()->changeState(GameStateType::TOWN, "from_beach");
            return true;
        }
    }
    return false;
}

// ========== ForestState ==========

ForestState::ForestState() : BaseGameState(GameStateType::FOREST) {
}

ForestState::~ForestState() {
}

void ForestState::initializeScene() {
    _scene = Forest::createScene();
    if (_scene) {
        _forestScene = dynamic_cast<Forest*>(_scene->getChildByName("Forest"));
    }
}

bool ForestState::onHandleInput(cocos2d::Event* event) {
    // 森林状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
            // ESC键暂停游戏
            GameStateManager::getInstance()->pushState(GameStateType::PAUSED);
            return true;
        } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_F) {
            // F键返回农场
            GameStateManager::getInstance()->changeState(GameStateType::FARM, "from_forest");
            return true;
        }
    }
    return false;
}

// ========== CaveState ==========

CaveState::CaveState() : BaseGameState(GameStateType::CAVE) {
}

CaveState::~CaveState() {
}

void CaveState::initializeScene() {
    _scene = Cave::createScene();
    if (_scene) {
        _caveScene = dynamic_cast<Cave*>(_scene->getChildByName("Cave"));
    }
}

bool CaveState::onHandleInput(cocos2d::Event* event) {
    // 洞穴状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_DOWN) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (_caveScene) {
            if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ENTER || 
                keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER) {
                _caveScene->isEnterKeyPressed = true;
                CCLOG("Enter key pressed. ");
                return true;
            } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_M) {
                _caveScene->isMKeyPressed = true;
                return true;
            } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
                // ESC键打开/关闭背包
                static int isOpen = 0;
                static InventoryUI* currentInventoryUI = nullptr;
                if (currentInventoryUI == nullptr || isOpen == 0) {
                    isOpen = 1;
                    CCLOG("Opening inventory.");
                    currentInventoryUI = InventoryUI::create(inventory, "Cave");
                    _caveScene->addChild(currentInventoryUI, 20);
                } else {
                    isOpen = 0;
                    CCLOG("Closing inventory.");
                    _caveScene->removeChild(currentInventoryUI, true);
                    currentInventoryUI = nullptr;
                }
                return true;
            }
        }
    } else if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (_caveScene) {
            if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ENTER || 
                keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER) {
                _caveScene->isEnterKeyPressed = false;
                return true;
            } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_M) {
                _caveScene->isMKeyPressed = false;
                return true;
            } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_F) {
                // F键返回农场
                GameStateManager::getInstance()->changeState(GameStateType::FARM, "from_cave");
                return true;
            }
        }
    }
    return false;
}

// ========== BarnState ==========

BarnState::BarnState() : BaseGameState(GameStateType::BARN) {
}

BarnState::~BarnState() {
}

void BarnState::initializeScene() {
    _scene = Barn::createScene();
    if (_scene) {
        _barnScene = dynamic_cast<Barn*>(_scene->getChildByName("Barn"));
    }
}

bool BarnState::onHandleInput(cocos2d::Event* event) {
    // 畜棚状态输入处理
    if (event->getType() == cocos2d::Event::Type::KEY_DOWN) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (_barnScene) {
            if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ENTER || 
                keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER) {
                _barnScene->isEnterKeyPressed = true;
                CCLOG("Enter key pressed. ");
                return true;
            } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
                // ESC键打开/关闭背包
                static int isOpen = 0;
                static InventoryUI* currentInventoryUI = nullptr;
                if (currentInventoryUI == nullptr || isOpen == 0) {
                    isOpen = 1;
                    CCLOG("Opening inventory.");
                    currentInventoryUI = InventoryUI::create(inventory, "Barn");
                    _barnScene->addChild(currentInventoryUI, 20);
                } else {
                    isOpen = 0;
                    CCLOG("Closing inventory.");
                    _barnScene->removeChild(currentInventoryUI, true);
                    currentInventoryUI = nullptr;
                }
                return true;
            }
        }
    } else if (event->getType() == cocos2d::Event::Type::KEY_UP) {
        auto keyEvent = static_cast<cocos2d::EventKeyboard*>(event);
        if (_barnScene) {
            if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_ENTER || 
                keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER) {
                _barnScene->isEnterKeyPressed = false;
                return true;
            } else if (keyEvent->getKeyCode() == cocos2d::EventKeyboard::KeyCode::KEY_F) {
                // F键返回农场
                GameStateManager::getInstance()->changeState(GameStateType::FARM, "from_barn");
                return true;
            }
        }
    }
    return false;
}

// ========== GameStateFactory ==========

std::shared_ptr<IGameState> GameStateFactory::createState(GameStateType type) {
    switch (type) {
        case GameStateType::MAIN_MENU:
            return std::make_shared<MainMenuState>();
        case GameStateType::CHARACTER_CREATION:
            return std::make_shared<CharacterCreationState>();
        case GameStateType::FARM:
            return std::make_shared<FarmState>();
        case GameStateType::TOWN:
            return std::make_shared<TownState>();
        case GameStateType::SHOP:
            return std::make_shared<ShopState>();
        case GameStateType::MYHOUSE:
            return std::make_shared<MyhouseState>();
        case GameStateType::BEACH:
            return std::make_shared<BeachState>();
        case GameStateType::FOREST:
            return std::make_shared<ForestState>();
        case GameStateType::CAVE:
            return std::make_shared<CaveState>();
        case GameStateType::BARN:
            return std::make_shared<BarnState>();
        case GameStateType::SUPERMARKET:
            return std::make_shared<SupermarketState>();
        case GameStateType::PAUSED:
            return std::make_shared<PausedState>();
        default:
            std::cerr << "Unknown state type: " << static_cast<int>(type) << std::endl;
            return nullptr;
    }
}

void GameStateFactory::registerAllStates() {
    auto stateManager = GameStateManager::getInstance();
    
    stateManager->registerStateCreator(GameStateType::MAIN_MENU, []() {
        return std::make_shared<MainMenuState>();
    });
    
    stateManager->registerStateCreator(GameStateType::CHARACTER_CREATION, []() {
        return std::make_shared<CharacterCreationState>();
    });
    
    stateManager->registerStateCreator(GameStateType::FARM, []() {
        return std::make_shared<FarmState>();
    });
    
    stateManager->registerStateCreator(GameStateType::TOWN, []() {
        return std::make_shared<TownState>();
    });
    
    stateManager->registerStateCreator(GameStateType::SHOP, []() {
        return std::make_shared<ShopState>();
    });
    
    stateManager->registerStateCreator(GameStateType::MYHOUSE, []() {
        return std::make_shared<MyhouseState>();
    });
    
    stateManager->registerStateCreator(GameStateType::BEACH, []() {
        return std::make_shared<BeachState>();
    });
    
    stateManager->registerStateCreator(GameStateType::FOREST, []() {
        return std::make_shared<ForestState>();
    });
    
    stateManager->registerStateCreator(GameStateType::CAVE, []() {
        return std::make_shared<CaveState>();
    });
    
    stateManager->registerStateCreator(GameStateType::BARN, []() {
        return std::make_shared<BarnState>();
    });
    
    stateManager->registerStateCreator(GameStateType::SUPERMARKET, []() {
        return std::make_shared<SupermarketState>();
    });
    
    stateManager->registerStateCreator(GameStateType::PAUSED, []() {
        return std::make_shared<PausedState>();
    });
}