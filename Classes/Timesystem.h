#pragma once
#include "cocos2d.h"  
#include "AppDelegate.h"
#include "GameEvent.h"
#include "EventManager.h"
#include <NPC.h>

class Timesystem : public cocos2d::Node {  // 继承自Node
public:
    bool init ( std::string place );

    static Timesystem* create ( std::string place );

    void UpdateEnergy();

    // ������Ա�����ͷ���
    cocos2d::Label* timer_label_day;
    cocos2d::Label* timer_label_hour;
    cocos2d::Label* timer_label_season;
    cocos2d::Label* timer_label_weather;
    cocos2d::Label* timer_label_festival;
    cocos2d::Sprite* time_pic;
    cocos2d::Sprite* energy_frame;
    cocos2d::Sprite* energy_bar;

    cocos2d::Sprite* currency_frame;
    cocos2d::Label* currency_num;

    cocos2d::Sprite* daily_record;

    // 场景的名称
    std::string Place;
    
    // 时间相关属性
    int day = 0;
    int remainingTime = 0;
    std::string Season = "Spring";
    std::string Weather = "Sunny";
    std::string Festival = "None";
    int strength = 100;
    int GoldAmount = 0;
    
    // 静态单例指针
    static Timesystem* TimeUI;
    
    // 事件通知方法
    void notifyTimeChanged(int oldHour, int newHour);
    void notifyDayChanged(int newDay);
    void notifySeasonChanged(const std::string& oldSeason, const std::string& newSeason);
    void notifyWeatherChanged(const std::string& oldWeather, const std::string& newWeather);
    void notifyEnergyChanged(int oldEnergy, int newEnergy);
    void notifyGoldChanged(int oldGold, int newGold);
};