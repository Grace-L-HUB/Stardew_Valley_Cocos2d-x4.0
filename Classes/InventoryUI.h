// InventoryUI.h  
#pragma once  
#include "cocos2d.h"  
#include "Inventory.h"  
#include "AppDelegate.h"
#include "EventObserver.h"
#include "intimacyUI.h"
#include "SkillTreeUI.h"

class InventoryUI : public cocos2d::Layer {
public:
    ~InventoryUI();
    virtual bool init ( Inventory* inventory , std::string sceneName );

    static InventoryUI* create ( Inventory* inventory , std::string sceneName );

    void backgroundcreate ();

    void Itemblock ( Inventory* inventory );

    void updateDisplay (); // ������ʾ����  

    void Buttons_switching ();

    void updateCoordinate ( float &x , float &y );
    void registerEventObservers();
    void refreshGoldLabel(int amount);
private:
    Inventory* _inventory; // ָ�� Inventory ʵ����ָ��  

    cocos2d::Label* _itemLabel;  // ��ʾ��Ʒ��Ϣ�ı�ǩ 
    cocos2d::Label* _goldLabel = nullptr;

    cocos2d::Vector<cocos2d::Sprite*> _itemSlots; // �洢��Ʒ�۵� Sprite  

    int _selectedSlot; // ��ǰѡ�еĲ�λ 

    bool isClick = false;  // ��־����ʾ�Ƿ񱻵��

    Sprite* currentItemSprite = nullptr; // ��ʶ��ǰѡ�����Ʒ

    std::string SceneName;

    std::shared_ptr<UIEventObserver> _eventObserver;
};
