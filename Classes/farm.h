#ifndef __farm_H__
#define __farm_H__

#include "cocos2d.h"
#include "Barn.h"
#include "Player.h"
#include "Crop.h"
#include "AppDelegate.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class EconomicSystem;


class farm : public cocos2d::Scene
{
public:

    farm ();
    ~farm ();

    virtual bool init ();

    static farm* create ();

    // �жϽ�ɫ��λ��
    void checkPlayerPosition ();

    // ����Ч��
    void createRainEffect();

    // ���������������
    void updaterain(float deltaTime);
    
    // 处理天气变化
    void handleWeatherChanged(const std::string& oldWeather, const std::string& newWeather);



    // �����������
    int getRegionNumber ( Vec2 pos );

    // ����һ���б������ڱ������з�͸�����ص�����
    std::vector<cocos2d::Vec2> nonTransparentPixels;

    // �뿪ũ��������
    Rect Out_Farm = Rect ( 792 , 1187 , 36 , 153 );

    // ����ֲ����(�������Ϊ 48 * 48 )
    Rect plant_area = Rect ( 474 , 467 , 864 , 510 );

    // ���Խ���ҵ�����
    Rect myhouse_area = Rect ( 70 , 920 , 75 , 30 );

    // ���Խ������������
    Rect barn_area = Rect ( 20 , 170 , 70 , 70 );

    // ���Խ���ɽ��������
    Rect cave_area = Rect ( 635 , 1185 , 90 , 40 );

    // ���Խ���ɭ�ֵ�����
    Rect forest_area = Rect ( 750 , -70 , 100 , 50 );

    cocos2d::ParticleRain* emitter;

    // �ָ���ֲ
    void AllInitialize_crop ();

private:

    /* �˳���ť */
    Sprite* button;


    // �˳���ť
    cocos2d::MenuItemImage* closeItem;

    cocos2d::Menu* menu;

    bool isEnterKeyPressed = false;
    // �ж���ֲP���Ƿ���
    bool isPKeyPressed = false;
    // �жϽ�ˮW���Ƿ���
    bool isWKeyPressed = false;
    // �ж��ո�G���Ƿ���
    bool isGKeyPressed = false;

    Sprite* Box;

};

#endif 