#ifndef __shop_H__
#define __shop_H__

#include "cocos2d.h"
#include "Player.h"
#include "AppDelegate.h"
#include "mini_bag.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class shop : public cocos2d::Scene
{
public:

    shop();
    ~shop();

    virtual bool init();

    static shop* create();
    static cocos2d::Scene* createScene();

    // 检查玩家位置
    void checkPlayerPosition();

    // 商店区域
    cocos2d::Rect shop_area = cocos2d::Rect(0, 0, 0, 0);

private:
    
    Sprite* Box;
    bool isEnterKeyPressed = false;
};

#endif // __shop_H__