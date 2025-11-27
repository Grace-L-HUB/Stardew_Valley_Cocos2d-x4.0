#pragma once
// StoreUI.h    
#include "cocos2d.h"  
#include "Inventory.h"  
#include "AppDelegate.h"
#include "EconomicSystem.h"
#include "EventObserver.h"

USING_NS_CC;

class StoreUI : public cocos2d::Layer {
public:
    ~StoreUI();
    virtual bool init ( Inventory* mybag , Inventory* goods );
    static StoreUI* create ( Inventory* mybag , Inventory* goods );
    //???????
    void backgroundcreate ();
    //?????
    void ProductDisplay ( Inventory* mybag , Inventory* goods );

    void Itemblock ( Inventory* mybag , Inventory* goods );

    void updateDisplay (); // ???????????  

    void moneyDisplay ();

    void SliderDisplay ();

    void updateCoordinate ( float& x , float& y );

private:

    Inventory* _mybag; // ??????????????????  

    Inventory* _goods; // ??????????????  

    cocos2d::Label* _itemLabel;  // ????????????? 

    cocos2d::Vector<cocos2d::Sprite*> _itemSlots; // ???????? Sprite  

    int _selectedSlot; // ?????§Ö??¦Ë  

    void onItemSlotClicked ( cocos2d::Ref* sender ); // ??????????????? 

    bool isClick = false;

    shared_ptr<Item> chosen_Item = nullptr;

    std::shared_ptr<EconomicSystem> economicSystem;

    std::shared_ptr<UIEventObserver> _eventObserver;
    cocos2d::Label* _goldLabel = nullptr;
};
