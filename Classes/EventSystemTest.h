#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include <map>

USING_NS_CC;
using namespace cocos2d::ui;

class EventSystemTest : public Layer {
public:
    static EventSystemTest* create();
    virtual bool init() override;
    
    void subscribeToEvents();
    void unsubscribeFromEvents();
    void createTestButtons();
    
    // 测试方法
    void testItemEvent();
    void testPlayerEvent();
    void testTimeEvent();
    void updateLog(const std::string& message);
    
    virtual ~EventSystemTest();
    
private:
    std::map<std::string, int> eventListenerIds; // 存储监听器ID
    Label* logText; // 日志显示标签
};
