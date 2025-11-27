#include "shop.h"
#include "GameStateManager.h"

shop::shop()
{
}

shop::~shop()
{
}

bool shop::init()
{
    if (!cocos2d::Scene::init())
    {
        return false;
    }
    
    // 获取屏幕大小
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    
    // 创建商店背景
    auto background = cocos2d::Sprite::create("supermarket/shop_background.png");
    if (background)
    {
        background->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(background, 0);
    }
    
    // 添加返回按钮
    auto closeItem = cocos2d::MenuItemImage::create(
        "UIresource/closeButton.png",
        "UIresource/closeButton.png",
        CC_CALLBACK_1(shop::checkPlayerPosition, this));
    
    if (closeItem)
    {
        closeItem->setPosition(cocos2d::Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
                                            origin.y + visibleSize.height - closeItem->getContentSize().height / 2));
    }
    
    auto menu = cocos2d::Menu::create(closeItem, NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 1);
    
    // 初始化物品箱
    Box = cocos2d::Sprite::create("UIresource/box.png");
    if (Box)
    {
        Box->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        Box->setVisible(false);
        this->addChild(Box, 2);
    }
    
    return true;
}

shop* shop::create()
{
    shop* pRet = new(std::nothrow) shop();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

cocos2d::Scene* shop::createScene()
{
    auto scene = cocos2d::Scene::create();
    auto layer = shop::create();
    if (layer)
    {
        layer->setName("shop");
        scene->addChild(layer);
    }
    return scene;
}

void shop::checkPlayerPosition()
{
    // 这里可以添加更多逻辑，但主要通过GameStateManager处理状态切换
    GameStateManager::getInstance()->changeState(GameStateType::TOWN);
}