#pragma once

#include "cocos2d.h"
#include <functional>
#include <string>

class StateTransitionEffects {
public:
    static cocos2d::TransitionScene* createTransition(const std::string& type,
                                                      cocos2d::Scene* targetScene,
                                                      float duration);

    static void applyCustomTransition(const std::string& type,
                                      cocos2d::Scene* fromScene,
                                      cocos2d::Scene* toScene,
                                      float duration,
                                      const std::function<void()>& onComplete);
};

