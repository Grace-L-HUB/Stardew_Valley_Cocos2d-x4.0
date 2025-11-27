#include "StateTransitionEffects.h"

using namespace cocos2d;

TransitionScene* StateTransitionEffects::createTransition(const std::string& type,
                                                          Scene* targetScene,
                                                          float duration) {
    if (!targetScene) {
        return nullptr;
    }

    const float dur = duration > 0.0f ? duration : 0.5f;

    if (type == "fade") {
        return TransitionFade::create(dur, targetScene);
    }
    if (type == "slide_left") {
        return TransitionSlideInL::create(dur, targetScene);
    }
    if (type == "slide_right") {
        return TransitionSlideInR::create(dur, targetScene);
    }
    if (type == "flip") {
        return TransitionFlipX::create(dur, targetScene);
    }
    if (type == "zoom") {
        return TransitionZoomFlipX::create(dur, targetScene);
    }
    return nullptr;
}

void StateTransitionEffects::applyCustomTransition(const std::string& type,
                                                   Scene* fromScene,
                                                   Scene* toScene,
                                                   float duration,
                                                   const std::function<void()>& onComplete) {
    if (type == "door_open" && fromScene && toScene) {
        auto sequence = Sequence::create(
            FadeOut::create(duration * 0.5f),
            CallFunc::create([toScene]() {
                Director::getInstance()->replaceScene(toScene);
            }),
            FadeIn::create(duration * 0.5f),
            CallFunc::create([onComplete]() {
                if (onComplete) {
                    onComplete();
                }
            }),
            nullptr);
        fromScene->runAction(sequence);
        return;
    }

    if (type == "teleport" && toScene) {
        Director::getInstance()->replaceScene(toScene);
        if (onComplete) {
            onComplete();
        }
        return;
    }

    if (onComplete) {
        onComplete();
    }
}

