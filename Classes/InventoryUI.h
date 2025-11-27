// InventoryUI.h  
#pragma once  
#include "cocos2d.h"  
#include "Inventory.h"  
#include "AppDelegate.h"
#include "intimacyUI.h"
#include "SkillTreeUI.h"
#include "EventManager.h"
#include "GameEvent.h"

class InventoryUI : public cocos2d::Layer {
public:
    virtual bool init ( Inventory* inventory , std::string sceneName );

    static InventoryUI* create ( Inventory* inventory , std::string sceneName );

    void backgroundcreate ();

    void Itemblock ( Inventory* inventory );

    void updateDisplay (); // ������ʾ����  

    void Buttons_switching ();

    void updateCoordinate ( float &x , float &y );
    
    // 生命周期方法
    virtual void onExit() override;
private:
    Inventory* _inventory; // ָ�� Inventory ʵ����ָ��  

    cocos2d::Label* _itemLabel;  // ��ʾ��Ʒ��Ϣ�ı�ǩ 

    cocos2d::Vector<cocos2d::Sprite*> _itemSlots; // �洢��Ʒ�۵� Sprite  

    int _selectedSlot; // ��ǰѡ�еĲ�λ 

    bool isClick = false;  // ��־����ʾ�Ƿ񱻵��

    Sprite* currentItemSprite = nullptr; // ��ʶ��ǰѡ�����Ʒ

    std::string SceneName;
    
    // 事件系统相关
    UIEventObserver* _eventObserver; // 事件观察者实例
};
