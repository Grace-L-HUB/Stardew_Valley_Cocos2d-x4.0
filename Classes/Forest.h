#ifndef __Forest_H__
#define __Forest_H__

#include "cocos2d.h"
#include "Player.h"
#include "tree.h"
#include "AppDelegate.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class Forest : public cocos2d::Scene
{
public:

    Forest();
    ~Forest();

    virtual bool init();

    static  Forest* create();

    // �жϽ�ɫ��λ��
    void checkPlayerPosition();

    // �����������
    int getRegionNumber(Vec2 pos);

    // ����Ч��
    void createRainEffect();

    // ���������������
    void updaterain(float deltaTime);

    // ����һ���б������ڱ������з�͸�����ص�����
    std::vector<cocos2d::Vec2> nonTransparentPixels;
  
    // �ָ���ֲ
    void AllInitialize_ore(); 

    // ǰ��ũ��
    Rect farm_area = Rect(1100, 2500, 150, 300);

    // ǰ��С��
    Rect town_area = Rect(2500, 1850, 400, 150);

    cocos2d::ParticleRain* emitter;

   
private:

    /* �˳���ť */
    Sprite* button;

    // �˳���ť
    cocos2d::MenuItemImage* closeItem;

    cocos2d::Menu* menu;

    bool isEnterKeyPressed = false;
    // �жϿ���L���Ƿ���
    bool isLKeyPressed = false;


};

#endif 
