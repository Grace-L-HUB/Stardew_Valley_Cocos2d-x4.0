#include "Timesystem.h"  
#include "ui/CocosGUI.h"  
#include "Item.h"  
#include "DailyRecordUI.h"
#include "EventManager.h"

USING_NS_CC;



bool Timesystem::init( std::string place ) {
    // ���ø����init()
    if (!Node::init()) {  // ����̳���Node������Node��init
        return false;
    }
    Place = place;
    // ���ü�ʱ����ǩ
    timer_label_day = Label::createWithTTF("Day: 0", "fonts/Marker Felt.ttf", 24);
    this->addChild(timer_label_day, 2);
    timer_label_day->setScale(1.7f);
    timer_label_day->setString("Day" + day);

    timer_label_hour = Label::createWithTTF("0:00", "fonts/Marker Felt.ttf", 24);
    this->addChild(timer_label_hour, 2);
    timer_label_hour->setScale(1.7f);
    timer_label_day->setString(remainingTime / 1800 + ":00");

    timer_label_season = Label::createWithTTF("Spring", "fonts/Marker Felt.ttf", 24);
    this->addChild(timer_label_season, 2);
    timer_label_season->setScale(1.5f);
    timer_label_season->setString(Season);

    timer_label_weather = Label::createWithTTF("Rainy", "fonts/Marker Felt.ttf", 24);
    this->addChild(timer_label_weather, 2);
    timer_label_weather->setScale(1.5f);
    timer_label_weather->setString(Weather);

    timer_label_festival = Label::createWithTTF("Fishing Festival", "fonts/Marker Felt.ttf", 24);
    this->addChild(timer_label_festival, 2);
    timer_label_festival->setScale(1.5f);
    timer_label_festival->setString(Festival);

    // ����ʱ�䱳��ͼƬ
    time_pic = Sprite::create("UIresource/TimePic.png");
    time_pic->setScale(1.7f);
    this->addChild(time_pic, 1);  // ����ͼƬ�㼶�ڱ�ǩ�·�

    // ���������߿�
    energy_frame = Sprite::create("UIresource/strength.png");
    energy_frame->setScale(3.7f);
    this->addChild(energy_frame, 1);
    energy_frame->setPosition(435, 500);

    energy_bar = Sprite::create("Beach/green_bar.png");
    energy_bar->setScaleY(strength / 100.0 * 16.5f);
    energy_bar->setScaleX(3.1f);
    energy_bar->setAnchorPoint(Vec2(0.5f, 0.0f));
    this->addChild(energy_bar, 3);
    energy_bar->setPosition(435, 405);

    // ���ø�Ԫ�ص�λ��
    timer_label_day->setPosition(585, 575);
    timer_label_hour->setPosition(690, 575);
    timer_label_season->setPosition(570, 500);
    timer_label_weather->setPosition(710, 500);
    timer_label_festival->setPosition(630, 415);
    time_pic->setPosition(630, 490);
   

    //�����ʾ
    currency_frame = Sprite::create ( "UIresource/supermarket/moneyFrame_new.png" );
    currency_frame->setScale ( 3.5f );
    this->addChild ( currency_frame , 1 );
    currency_frame->setPosition ( 630 , 330 );
    currency_num = nullptr;
    int goldAmount = GoldAmount;
    if (currency_num == nullptr) {
        currency_num = Label::createWithTTF ( std::to_string ( goldAmount ) , "fonts/Marker Felt.ttf" , 45 );
        currency_num->setTextColor ( Color4B::WHITE );
        currency_num->setPosition ( 630 , 320 );
        this->addChild ( currency_num , 4 );
    }

    //��־��ʾ
    daily_record = Sprite::create ( "UIresource/rizhi.png" );
    this->addChild ( daily_record , 1 );
    daily_record->setScale ( 1.5f );
    daily_record->setPosition ( 670 , 250 );

    auto listener = EventListenerMouse::create ();
    listener->onMouseMove = [this]( EventMouse* event ) {
        Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
        mousePos = this->convertToNodeSpace ( mousePos );
        if (daily_record->getBoundingBox ().containsPoint ( mousePos )) {
            daily_record->setScale ( 1.5f * 1.2f );
        }
        else
            daily_record->setScale ( 1.5f );
        };
    listener->onMouseDown = [this, place](EventMouse* event) {
        Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
        mousePos = this->convertToNodeSpace ( mousePos );
        if (daily_record->getBoundingBox ().containsPoint ( mousePos )) {
            DailyRecordUI* Dailyrecord = DailyRecordUI::create(place);
            // ��ȡ��ǰ���еĳ���
            Scene* currentScene = Director::getInstance ()->getRunningScene ();
            currentScene->addChild ( Dailyrecord , 20 );
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , daily_record );

    // ���¼���ʱ��
    int lastDay = day;
    int lastHour = remainingTime / 1800;
    std::string lastSeason = Season;
    std::string lastWeather = Weather;

    this->schedule([this, &lastDay, &lastHour, &lastSeason, &lastWeather](float dt) {
        // ��ȡ��ǰֵ
        int currentHour = remainingTime / 1800;
        
        // ��־���洢��ֵ���滻��
        timer_label_day->setString("Day: " + std::to_string(day));
        timer_label_hour->setString(std::to_string(currentHour) + ":00");
        timer_label_season->setString(Season);
        timer_label_weather->setString(Weather);
        currency_num->setString ( std::to_string ( GoldAmount ) );
        
        // ���发布时间变化事件
        if (currentHour != lastHour || day != lastDay || Season != lastSeason) {
            EventManager::getInstance().publishTimeChanged(day, currentHour, Season);
            lastHour = currentHour;
            lastDay = day;
            lastSeason = Season;
        }
        
        // 发布季节变化事件
        if (Season != lastSeason) {
            lastSeason = Season;
        }
        
        // 发布天气变化事件
        if (Weather != lastWeather) {
            EventManager::getInstance().publishWeatherChanged(lastWeather, Weather);
            lastWeather = Weather;
        }
        }, 0.01f, "updatetime");

    return true;
}

Timesystem* Timesystem::create( std::string place ) {
    Timesystem* ret = new Timesystem();
    if (ret && ret->init(place)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Timesystem::UpdateEnergy () {
    TimeUI->energy_bar->setScaleY ( strength / 100.0 * 16.5f );
    
    // 发布玩家能量变化事件
    EventManager::getInstance().publishPlayerEnergyChanged(strength);
}