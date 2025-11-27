#ifndef __Town_H__
#define __Town_H__

#include "cocos2d.h"
#include "Player.h"
#include "AppDelegate.h"
#include "mini_bag.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class Town : public cocos2d::Scene
{
public:

    Town();
    ~Town();

    virtual bool init();

    static Town* create();
    static cocos2d::Scene* createScene();

    // �жϽ�ɫ��λ��
    void checkPlayerPosition();

    // ����Ч��
    void createRainEffect();

    // ���������������
    void updaterain(float deltaTime);
    
    // ����һ���б������ڱ������з�͸�����ص�����
    std::vector<cocos2d::Vec2> nonTransparentPixels;

    // �������ӵ�����
    cocos2d::Rect Region_supermarket = Rect(163, 518, 130, 60);
    cocos2d::Rect Region_forest = Rect(-940, -400, 200, 110);
    cocos2d::Rect Region_beach = Rect(490, -750, 50, 100);

    cocos2d::ParticleRain* emitter;

private:

    /* �˳���ť */
    Sprite* button;

    /* ���Ű�ť */
    Sprite* opendoor;

    // �˳���ť
    cocos2d::MenuItemImage* closeItem;

    cocos2d::Menu* menu;

    cocos2d::Sprite* In_gettask;

    bool isEnterKeyPressed = false;

    Sprite* Box;

};

#endif // __BACKGROUND1_H__
