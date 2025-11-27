#ifndef STATE_TRANSITION_EFFECTS_H
#define STATE_TRANSITION_EFFECTS_H

#include "cocos2d.h"
#include "GameState.h"
#include <functional>
#include <string>

/**
 * 转场效果基类
 */
class TransitionEffect {
public:
    using CompletionCallback = std::function<void()>;

    virtual ~TransitionEffect() = default;

    /**
     * 执行转场效果
     */
    virtual void execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) = 0;

    /**
     * 设置转场持续时间
     */
    void setDuration(float duration) {
        _duration = duration;
    }

    /**
     * 获取转场持续时间
     */
    float getDuration() const {
        return _duration;
    }

    /**
     * 检查转场是否正在进行
     */
    bool isRunning() const {
        return _isRunning;
    }

    /**
     * 取消转场
     */
    virtual void cancel() {
        _isRunning = false;
    }

protected:
    float _duration = 0.5f;  // 默认持续时间
    bool _isRunning = false; // 转场是否正在运行
    CompletionCallback _completionCallback = nullptr;
};

/**
 * 淡入淡出转场效果
 */
class FadeTransition : public TransitionEffect {
public:
    FadeTransition(float duration = 0.5f);
    ~FadeTransition() override;

    void execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) override;
    void cancel() override;

private:
    cocos2d::LayerColor* _fadeLayer = nullptr;
    cocos2d::Sequence* _currentSequence = nullptr;
};

/**
 * 滑动转场效果
 */
class SlideTransition : public TransitionEffect {
public:
    enum class SlideDirection {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    SlideTransition(SlideDirection direction = SlideDirection::LEFT, float duration = 0.5f);
    ~SlideTransition() override;

    void execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) override;
    void cancel() override;

    void setDirection(SlideDirection direction) {
        _direction = direction;
    }

private:
    SlideDirection _direction;
    cocos2d::Sequence* _currentSequence = nullptr;
};

/**
 * 翻页转场效果
 */
class PageTransition : public TransitionEffect {
public:
    enum class PageDirection {
        LEFT,
        RIGHT
    };

    PageTransition(PageDirection direction = PageDirection::LEFT, float duration = 0.5f);
    ~PageTransition() override;

    void execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) override;
    void cancel() override;

    void setDirection(PageDirection direction) {
        _direction = direction;
    }

private:
    PageDirection _direction;
    cocos2d::Sequence* _currentSequence = nullptr;
};

/**
 * 门开关转场效果
 */
class DoorTransition : public TransitionEffect {
public:
    enum class DoorType {
        HORIZONTAL,
        VERTICAL
    };

    DoorTransition(DoorType type = DoorType::HORIZONTAL, float duration = 0.5f);
    ~DoorTransition() override;

    void execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) override;
    void cancel() override;

    void setDoorType(DoorType type) {
        _doorType = type;
    }

private:
    DoorType _doorType;
    cocos2d::LayerColor* _leftDoor = nullptr;
    cocos2d::LayerColor* _rightDoor = nullptr;
    cocos2d::Sequence* _currentSequence = nullptr;
};

/**
 * 缩放转场效果
 */
class ZoomTransition : public TransitionEffect {
public:
    ZoomTransition(float duration = 0.5f, float scaleFactor = 0.1f);
    ~ZoomTransition() override;

    void execute(cocos2d::Scene* fromScene, cocos2d::Scene* toScene, const CompletionCallback& callback) override;
    void cancel() override;

    void setScaleFactor(float factor) {
        _scaleFactor = factor;
    }

private:
    float _scaleFactor;
    cocos2d::Sequence* _currentSequence = nullptr;
};

/**
 * 转场效果工厂类
 */
class TransitionEffectFactory {
public:
    static std::unique_ptr<TransitionEffect> createEffect(const std::string& effectType, float duration = 0.5f);
    static std::unique_ptr<TransitionEffect> createEffectWithParams(const std::string& effectType, const std::string& params, float duration = 0.5f);
    static bool isEffectSupported(const std::string& effectType);
};

#endif // STATE_TRANSITION_EFFECTS_H