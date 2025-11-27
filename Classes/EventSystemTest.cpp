#include "EventSystemTest.h"
#include "EventManager.h"
#include "GameEvent.h"
#include "cocos2d.h"

USING_NS_CC;

EventSystemTest* EventSystemTest::create() {
    EventSystemTest* ret = new EventSystemTest();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool EventSystemTest::init() {
    if (!Layer::init()) {
        return false;
    }
    
    // 测试UI
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto testLabel = Label::createWithTTF("Event System Test", "fonts/Marker Felt.ttf", 36);
    testLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 50));
    this->addChild(testLabel, 1);
    
    // 订阅事件
    subscribeToEvents();
    
    // 创建测试按钮
    createTestButtons();
    
    log("EventSystemTest initialized and subscribed to events");
    
    return true;
}

void EventSystemTest::subscribeToEvents() {
    // 订阅物品事件
    eventListenerIds["ITEM_ADDED"] = EventManager::getInstance().addListener(
        GameEventType::ITEM_ADDED,
        [this](const std::shared_ptr<GameEvent>& event) {
            auto data = event->getData<ItemAddedEventData>();
            if (data) {
                log("Test received ITEM_ADDED event: %s, Count: %d", 
                    data->item.GetName().c_str(), data->count);
                updateLog("Item added: " + data->item.GetName());
            }
        }
    );
    
    // 订阅玩家位置事件
    eventListenerIds["PLAYER_POSITION_CHANGED"] = EventManager::getInstance().addListener(
        GameEventType::PLAYER_POSITION_CHANGED,
        [this](const std::shared_ptr<GameEvent>& event) {
            auto data = event->getData<PlayerPositionChangedEventData>();
            if (data) {
                log("Test received PLAYER_POSITION_CHANGED event: (%.2f, %.2f)", 
                    data->position.x, data->position.y);
                updateLog("Player position: (" + 
                         std::to_string(data->position.x) + ", " + 
                         std::to_string(data->position.y) + ")");
            }
        }
    );
    
    // 订阅时间变化事件
    eventListenerIds["TIME_CHANGED"] = EventManager::getInstance().addListener(
        GameEventType::TIME_CHANGED,
        [this](const std::shared_ptr<GameEvent>& event) {
            auto data = event->getData<TimeChangedEventData>();
            if (data) {
                log("Test received TIME_CHANGED event: %d:00 -> %d:00", 
                    data->oldHour, data->newHour);
                updateLog("Time changed: " + 
                         std::to_string(data->oldHour) + " -> " + 
                         std::to_string(data->newHour));
            }
        }
    );
    
    // 订阅作物事件
    eventListenerIds["CROP_PHASE_CHANGED"] = EventManager::getInstance().addListener(
        GameEventType::CROP_PHASE_CHANGED,
        [this](const std::shared_ptr<GameEvent>& event) {
            auto data = event->getData<CropPhaseChangedEventData>();
            if (data) {
                log("Test received CROP_PHASE_CHANGED event: %s (%d -> %d)", 
                    data->crop.GetName().c_str(), data->oldPhase, data->newPhase);
                updateLog("Crop phase changed: " + data->crop.GetName());
            }
        }
    );
}

void EventSystemTest::unsubscribeFromEvents() {
    for (const auto& pair : eventListenerIds) {
        EventManager::getInstance().removeListener(pair.second);
    }
    eventListenerIds.clear();
    log("EventSystemTest unsubscribed from all events");
}

void EventSystemTest::createTestButtons() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 测试发送物品事件按钮
    auto testItemBtn = ui::Button::create("UIresource/button_normal.png", 
                                          "UIresource/button_pressed.png");
    testItemBtn->setTitleText("Test Item Event");
    testItemBtn->setTitleFontSize(24);
    testItemBtn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 150));
    testItemBtn->addClickEventListener([this](Ref* pSender) {
        testItemEvent();
    });
    this->addChild(testItemBtn, 1);
    
    // 测试发送玩家位置事件按钮
    auto testPlayerBtn = ui::Button::create("UIresource/button_normal.png", 
                                           "UIresource/button_pressed.png");
    testPlayerBtn->setTitleText("Test Player Event");
    testPlayerBtn->setTitleFontSize(24);
    testPlayerBtn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 200));
    testPlayerBtn->addClickEventListener([this](Ref* pSender) {
        testPlayerEvent();
    });
    this->addChild(testPlayerBtn, 1);
    
    // 测试发送时间事件按钮
    auto testTimeBtn = ui::Button::create("UIresource/button_normal.png", 
                                         "UIresource/button_pressed.png");
    testTimeBtn->setTitleText("Test Time Event");
    testTimeBtn->setTitleFontSize(24);
    testTimeBtn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 250));
    testTimeBtn->addClickEventListener([this](Ref* pSender) {
        testTimeEvent();
    });
    this->addChild(testTimeBtn, 1);
    
    // 日志显示区域
    logText = Label::createWithTTF("Event logs will appear here", "fonts/Marker Felt.ttf", 24);
    logText->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    logText->setDimensions(visibleSize.width - 100, 400);
    logText->setHorizontalAlignment(TextHAlignment::LEFT);
    logText->setVerticalAlignment(TextVAlignment::TOP);
    logText->setOverflow(Label::Overflow::SHRINK);
    this->addChild(logText, 1);
}

void EventSystemTest::testItemEvent() {
    // 创建一个临时的Item对象用于测试
    class TestItem : public Item {
    public:
        TestItem() : Item("TestItem", "test.png", 10) {}
    };
    
    TestItem testItem;
    auto eventData = std::make_shared<ItemAddedEventData>(testItem, 1);
    auto event = std::make_shared<GameEvent>(GameEventType::ITEM_ADDED, eventData, "EventSystemTest");
    EventManager::getInstance().dispatchEvent(event);
    
    log("Test: Dispatched ITEM_ADDED event");
}

void EventSystemTest::testPlayerEvent() {
    Vec2 testPosition(100, 200);
    auto eventData = std::make_shared<PlayerPositionChangedEventData>(testPosition);
    auto event = std::make_shared<GameEvent>(GameEventType::PLAYER_POSITION_CHANGED, eventData, "EventSystemTest");
    EventManager::getInstance().dispatchEvent(event);
    
    log("Test: Dispatched PLAYER_POSITION_CHANGED event");
}

void EventSystemTest::testTimeEvent() {
    auto eventData = std::make_shared<TimeChangedEventData>(9, 10, 1, 1, "Spring");
    auto event = std::make_shared<GameEvent>(GameEventType::TIME_CHANGED, eventData, "EventSystemTest");
    EventManager::getInstance().dispatchEvent(event);
    
    log("Test: Dispatched TIME_CHANGED event");
}

void EventSystemTest::updateLog(const std::string& message) {
    // 限制日志行数，只显示最近的几条
    static std::vector<std::string> logs;
    logs.push_back(message);
    if (logs.size() > 10) {
        logs.erase(logs.begin());
    }
    
    std::string logTextStr;
    for (const auto& log : logs) {
        logTextStr += log + "\n";
    }
    
    logText->setString(logTextStr);
}

EventSystemTest::~EventSystemTest() {
    unsubscribeFromEvents();
}
