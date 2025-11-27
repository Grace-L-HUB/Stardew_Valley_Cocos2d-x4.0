#pragma once
// StoreUI.h    
#include "cocos2d.h"  
#include "Inventory.h"  
#include "AppDelegate.h"
#include "EconomicSystem.h"
#include "EventObserver.h"
#include "GameEvent.h"

USING_NS_CC;

class StoreUI : public cocos2d::Layer, public EventObserver {
public:
    virtual bool init ( Inventory* mybag , Inventory* goods );
    static StoreUI* create ( Inventory* mybag , Inventory* goods );
    //����ͼչʾ
    void backgroundcreate ();
    //��Ʒչʾ
    void ProductDisplay ( Inventory* mybag , Inventory* goods );

    void Itemblock ( Inventory* mybag , Inventory* goods );

    void updateDisplay (); // ������ʾ����  

    void moneyDisplay ();

    void SliderDisplay ();

    void updateCoordinate ( float& x , float& y );

private:

    Inventory* _mybag; // ָ���Լ�����ʵ����ָ��  

    Inventory* _goods; // ָ����Ʒʵ����ָ��  

    cocos2d::Label* _itemLabel;  // ��ʾ��Ʒ��Ϣ�ı�ǩ 

    cocos2d::Vector<cocos2d::Sprite*> _itemSlots; // �洢��Ʒ�۵� Sprite  

    int _selectedSlot; // ��ǰѡ�еĲ�λ  

    void onItemSlotClicked ( cocos2d::Ref* sender ); // ��Ʒ�۵ĵ���¼����� 

    bool isClick = false;

    shared_ptr<Item> chosen_Item = nullptr;

    std::shared_ptr<EconomicSystem> economicSystem;
    
public:
    // 事件处理方法
    virtual void onEvent(EventType eventType, const EventData& data) override;
    
    // 生命周期方法
    virtual void onEnter() override;
    virtual void onExit() override;
};
