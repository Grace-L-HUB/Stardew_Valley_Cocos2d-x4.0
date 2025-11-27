#include "StateTransitionEffects.h"
#include <iostream>
#include <memory>

// ========== FadeTransition ==========

FadeTransition::FadeTransition(float duration) {
    _duration = duration;
}

FadeTransition::~FadeTransition() {
    cancel();
}

void FadeTransition::execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) {
    if (!fromScene || !toScene) {
        std::cerr << "FadeTransition: Invalid scenes provided" << std::endl;
        if (callback) callback();
        return;
    }

    _isRunning = true;
    _completionCallback = callback;

    // 创建黑色遮罩层
    _fadeLayer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 0));
    fromScene->addChild(_fadeLayer, 1000);

    // 淡出当前场景
    auto fadeIn = cocos2d::FadeTo::create(_duration / 2, 255);
    
    // 替换场景
    auto replaceScene = cocos2d::CallFunc::create([fromScene, toScene]() {
        cocos2d::Director::getInstance()->replaceScene(toScene);
    });
    
    // 淡入新场景
    auto fadeOut = cocos2d::FadeTo::create(_duration / 2, 0);
    
    // 清理并完成
    auto cleanup = cocos2d::CallFunc::create([this]() {
        _isRunning = false;
        if (_fadeLayer) {
            _fadeLayer->removeFromParent();
            _fadeLayer = nullptr;
        }
        if (_completionCallback) {
            _completionCallback();
        }
    });

    _currentSequence = cocos2d::Sequence::create(fadeIn, replaceScene, fadeOut, cleanup, nullptr);
    _fadeLayer->runAction(_currentSequence);
}

void FadeTransition::cancel() {
    if (_currentSequence) {
        _currentSequence->stop();
        _currentSequence = nullptr;
    }
    if (_fadeLayer) {
        _fadeLayer->removeFromParent();
        _fadeLayer = nullptr;
    }
    _isRunning = false;
}

// ========== SlideTransition ==========

SlideTransition::SlideTransition(SlideDirection direction, float duration) {
    _direction = direction;
    _duration = duration;
}

SlideTransition::~SlideTransition() {
    cancel();
}

void SlideTransition::execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) {
    if (!fromScene || !toScene) {
        std::cerr << "SlideTransition: Invalid scenes provided" << std::endl;
        if (callback) callback();
        return;
    }

    _isRunning = true;
    _completionCallback = callback;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 startPos;
    cocos2d::Vec2 moveFromPos;
    cocos2d::Vec2 moveToPos;

    // 根据方向设置位置
    switch (_direction) {
        case SlideDirection::LEFT:
            startPos = cocos2d::Vec2(visibleSize.width, 0);
            moveFromPos = cocos2d::Vec2(0, 0);
            moveToPos = cocos2d::Vec2(-visibleSize.width, 0);
            break;
        case SlideDirection::RIGHT:
            startPos = cocos2d::Vec2(-visibleSize.width, 0);
            moveFromPos = cocos2d::Vec2(0, 0);
            moveToPos = cocos2d::Vec2(visibleSize.width, 0);
            break;
        case SlideDirection::UP:
            startPos = cocos2d::Vec2(0, -visibleSize.height);
            moveFromPos = cocos2d::Vec2(0, 0);
            moveToPos = cocos2d::Vec2(0, visibleSize.height);
            break;
        case SlideDirection::DOWN:
            startPos = cocos2d::Vec2(0, visibleSize.height);
            moveFromPos = cocos2d::Vec2(0, 0);
            moveToPos = cocos2d::Vec2(0, -visibleSize.height);
            break;
    }

    // 准备新场景位置
    toScene->setPosition(startPos);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(toScene, 100);

    // 同时移动两个场景
    auto moveFrom = cocos2d::MoveTo::create(_duration, moveToPos);
    auto moveTo = cocos2d::MoveTo::create(_duration, cocos2d::Vec2(0, 0));
    
    // 清理并完成
    auto cleanup = cocos2d::CallFunc::create([fromScene, toScene, this]() {
        _isRunning = false;
        fromScene->removeFromParent();
        cocos2d::Director::getInstance()->replaceScene(toScene);
        if (_completionCallback) {
            _completionCallback();
        }
    });

    _currentSequence = cocos2d::Sequence::create(cocos2d::Spawn::create(moveFrom, moveTo, nullptr), cleanup, nullptr);
    fromScene->runAction(moveFrom);
    toScene->runAction(_currentSequence);
}

void SlideTransition::cancel() {
    if (_currentSequence) {
        _currentSequence->stop();
        _currentSequence = nullptr;
    }
    _isRunning = false;
}

// ========== PageTransition ==========

PageTransition::PageTransition(PageDirection direction, float duration) {
    _direction = direction;
    _duration = duration;
}

PageTransition::~PageTransition() {
    cancel();
}

void PageTransition::execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) {
    if (!fromScene || !toScene) {
        std::cerr << "PageTransition: Invalid scenes provided" << std::endl;
        if (callback) callback();
        return;
    }

    _isRunning = true;
    _completionCallback = callback;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    float angle = _direction == PageDirection::LEFT ? -90.0f : 90.0f;

    // 设置3D变换
    auto fromLayer = cocos2d::Layer::create();
    fromLayer->addChild(fromScene);
    
    auto toLayer = cocos2d::Layer::create();
    toLayer->addChild(toScene);
    toLayer->setRotationY(angle);
    toLayer->setPosition3D(cocos2d::Vec3(visibleSize.width / 2, 0, -visibleSize.width / 2));

    auto container = cocos2d::Layer::create();
    container->addChild(fromLayer);
    container->addChild(toLayer);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(container, 100);

    // 翻页动画
    auto fromAction = cocos2d::RotateBy::create(_duration, 0, -angle);
    auto toAction = cocos2d::RotateBy::create(_duration, 0, -angle);
    
    // 清理并完成
    auto cleanup = cocos2d::CallFunc::create([container, toScene, this]() {
        _isRunning = false;
        container->removeFromParent();
        cocos2d::Director::getInstance()->replaceScene(toScene);
        if (_completionCallback) {
            _completionCallback();
        }
    });

    _currentSequence = cocos2d::Sequence::create(cocos2d::Spawn::create(fromAction, toAction, nullptr), cleanup, nullptr);
    fromLayer->runAction(fromAction);
    toLayer->runAction(toAction);
    container->runAction(cleanup);
}

void PageTransition::cancel() {
    if (_currentSequence) {
        _currentSequence->stop();
        _currentSequence = nullptr;
    }
    _isRunning = false;
}

// ========== DoorTransition ==========

DoorTransition::DoorTransition(DoorType type, float duration) {
    _doorType = type;
    _duration = duration;
}

DoorTransition::~DoorTransition() {
    cancel();
}

void DoorTransition::execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) {
    if (!fromScene || !toScene) {
        std::cerr << "DoorTransition: Invalid scenes provided" << std::endl;
        if (callback) callback();
        return;
    }

    _isRunning = true;
    _completionCallback = callback;

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    // 创建门
    if (_doorType == DoorType::HORIZONTAL) {
        _leftDoor = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 255), visibleSize.width / 2, visibleSize.height);
        _rightDoor = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 255), visibleSize.width / 2, visibleSize.height);
        _rightDoor->setPosition(visibleSize.width / 2, 0);
    } else {
        _leftDoor = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 255), visibleSize.width, visibleSize.height / 2);
        _rightDoor = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 255), visibleSize.width, visibleSize.height / 2);
        _rightDoor->setPosition(0, visibleSize.height / 2);
    }

    fromScene->addChild(_leftDoor, 1000);
    fromScene->addChild(_rightDoor, 1000);

    // 开门动画
    auto moveLeft = cocos2d::MoveBy::create(_duration / 2, 
        _doorType == DoorType::HORIZONTAL ? cocos2d::Vec2(-visibleSize.width / 2, 0) : cocos2d::Vec2(0, -visibleSize.height / 2));
    auto moveRight = cocos2d::MoveBy::create(_duration / 2, 
        _doorType == DoorType::HORIZONTAL ? cocos2d::Vec2(visibleSize.width / 2, 0) : cocos2d::Vec2(0, visibleSize.height / 2));
    
    // 替换场景
    auto replaceScene = cocos2d::CallFunc::create([fromScene, toScene]() {
        cocos2d::Director::getInstance()->replaceScene(toScene);
    });
    
    // 关门动画
    auto moveLeftBack = cocos2d::MoveBy::create(_duration / 2, 
        _doorType == DoorType::HORIZONTAL ? cocos2d::Vec2(visibleSize.width / 2, 0) : cocos2d::Vec2(0, visibleSize.height / 2));
    auto moveRightBack = cocos2d::MoveBy::create(_duration / 2, 
        _doorType == DoorType::HORIZONTAL ? cocos2d::Vec2(-visibleSize.width / 2, 0) : cocos2d::Vec2(0, -visibleSize.height / 2));
    
    // 清理并完成
    auto cleanup = cocos2d::CallFunc::create([this]() {
        _isRunning = false;
        if (_leftDoor) {
            _leftDoor->removeFromParent();
            _leftDoor = nullptr;
        }
        if (_rightDoor) {
            _rightDoor->removeFromParent();
            _rightDoor = nullptr;
        }
        if (_completionCallback) {
            _completionCallback();
        }
    });

    _currentSequence = cocos2d::Sequence::create(
        cocos2d::Spawn::create(moveLeft, moveRight, nullptr),
        replaceScene,
        cocos2d::Spawn::create(moveLeftBack, moveRightBack, nullptr),
        cleanup,
        nullptr);
    _leftDoor->runAction(_currentSequence);
    _rightDoor->runAction(moveRight);
}

void DoorTransition::cancel() {
    if (_currentSequence) {
        _currentSequence->stop();
        _currentSequence = nullptr;
    }
    if (_leftDoor) {
        _leftDoor->removeFromParent();
        _leftDoor = nullptr;
    }
    if (_rightDoor) {
        _rightDoor->removeFromParent();
        _rightDoor = nullptr;
    }
    _isRunning = false;
}

// ========== ZoomTransition ==========

ZoomTransition::ZoomTransition(float duration, float scaleFactor) {
    _duration = duration;
    _scaleFactor = scaleFactor;
}

ZoomTransition::~ZoomTransition() {
    cancel();
}

void ZoomTransition::execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) {
    if (!fromScene || !toScene) {
        std::cerr << "ZoomTransition: Invalid scenes provided" << std::endl;
        if (callback) callback();
        return;
    }

    _isRunning = true;
    _completionCallback = callback;

    // 准备缩放动画
    auto scaleDown = cocos2d::ScaleTo::create(_duration / 2, _scaleFactor);
    auto fadeOut = cocos2d::FadeTo::create(_duration / 2, 0);
    
    // 替换场景
    auto replaceScene = cocos2d::CallFunc::create([fromScene, toScene]() {
        toScene->setScale(_scaleFactor);
        toScene->setOpacity(0);
        cocos2d::Director::getInstance()->replaceScene(toScene);
    });
    
    // 放大并淡入
    auto scaleUp = cocos2d::ScaleTo::create(_duration / 2, 1.0f);
    auto fadeIn = cocos2d::FadeTo::create(_duration / 2, 255);
    
    // 清理并完成
    auto cleanup = cocos2d::CallFunc::create([toScene, this]() {
        _isRunning = false;
        if (_completionCallback) {
            _completionCallback();
        }
    });

    _currentSequence = cocos2d::Sequence::create(
        cocos2d::Spawn::create(scaleDown, fadeOut, nullptr),
        replaceScene,
        cocos2d::Spawn::create(scaleUp, fadeIn, nullptr),
        cleanup,
        nullptr);
    fromScene->runAction(_currentSequence);
}

void ZoomTransition::cancel() {
    if (_currentSequence) {
        _currentSequence->stop();
        _currentSequence = nullptr;
    }
    _isRunning = false;
}

// ========== TransitionEffectFactory ==========

std::unique_ptr<TransitionEffect> TransitionEffectFactory::createEffect(const std::string& effectType, float duration) {
    if (effectType == "fade") {
        return std::make_unique<FadeTransition>(duration);
    } else if (effectType == "slide_left") {
        return std::make_unique<SlideTransition>(SlideTransition::SlideDirection::LEFT, duration);
    } else if (effectType == "slide_right") {
        return std::make_unique<SlideTransition>(SlideTransition::SlideDirection::RIGHT, duration);
    } else if (effectType == "slide_up") {
        return std::make_unique<SlideTransition>(SlideTransition::SlideDirection::UP, duration);
    } else if (effectType == "slide_down") {
        return std::make_unique<SlideTransition>(SlideTransition::SlideDirection::DOWN, duration);
    } else if (effectType == "page_left") {
        return std::make_unique<PageTransition>(PageTransition::PageDirection::LEFT, duration);
    } else if (effectType == "page_right") {
        return std::make_unique<PageTransition>(PageTransition::PageDirection::RIGHT, duration);
    } else if (effectType == "door_horizontal") {
        return std::make_unique<DoorTransition>(DoorTransition::DoorType::HORIZONTAL, duration);
    } else if (effectType == "door_vertical") {
        return std::make_unique<DoorTransition>(DoorTransition::DoorType::VERTICAL, duration);
    } else if (effectType == "zoom") {
        return std::make_unique<ZoomTransition>(duration);
    }
    
    // 默认返回淡入淡出效果
    std::cerr << "Unknown transition effect type: " << effectType << "\nUsing default fade effect" << std::endl;
    return std::make_unique<FadeTransition>(duration);
}

std::unique_ptr<TransitionEffect> TransitionEffectFactory::createEffectWithParams(const std::string& effectType, const std::string& params, float duration) {
    // 基础实现，后续可以根据params解析更多参数
    return createEffect(effectType, duration);
}

bool TransitionEffectFactory::isEffectSupported(const std::string& effectType) {
    static const std::vector<std::string> supportedEffects = {
        "fade", "slide_left", "slide_right", "slide_up", "slide_down",
        "page_left", "page_right", "door_horizontal", "door_vertical", "zoom"
    };
    
    return std::find(supportedEffects.begin(), supportedEffects.end(), effectType) != supportedEffects.end();
}