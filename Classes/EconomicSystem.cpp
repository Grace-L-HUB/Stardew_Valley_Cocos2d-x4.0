#include "EconomicSystem.h"  
#include "cocos2d.h"  // ȷ������ cocos2d.h ��ʹ�� CCLOG  
#include <iostream>  
#include "EventManager.h"  
#include "GameEvent.h"  

extern int GoldAmount;

// ���캯����ʼ���������Ϊ4000  
EconomicSystem::EconomicSystem ( Inventory* mybag , Inventory* goods)
    : goldAmount (GoldAmount) , _mybag ( mybag ) , _goods ( goods ) {
    // ��ʼ���������������ִ��  
}

// ��������  
EconomicSystem::~EconomicSystem () {
    // ��������  
    GoldAmount = goldAmount;
}

// ���ӽ�ҵĺ���  
void EconomicSystem::addGold ( int amount ) {
    if (amount > 0) {
        int oldAmount = goldAmount;
        goldAmount += amount;
        CCLOG ( "Added %d gold. Total: %d gold." , amount , goldAmount );
        
        // 发送金币变化事件
        auto eventData = std::make_shared<GoldChangedEventData>(oldAmount, goldAmount);
        QUEUE_EVENT(GameEventType::GOLD_AMOUNT_CHANGED, eventData);
    }
    else {
        CCLOG ( "Amount to add must be positive." );
    }
}

// ���ٽ�ҵĺ���  
void EconomicSystem::subtractGold ( int amount ) {
    if (amount > 0 && amount <= goldAmount) {
        int oldAmount = goldAmount;
        goldAmount -= amount;
        CCLOG ( "Subtracted %d gold. Total: %d gold." , amount , goldAmount );
        
        // 发送金币变化事件
        auto eventData = std::make_shared<GoldChangedEventData>(oldAmount, goldAmount);
        QUEUE_EVENT(GameEventType::GOLD_AMOUNT_CHANGED, eventData);
    }
    else {
        if (amount > goldAmount) {
            CCLOG ( "Not enough gold to subtract." );
        }
        else {
            CCLOG ( "Amount to subtract must be positive." );
        }
    }
}

// ��ȡӵ�н�������ĺ���  
int EconomicSystem::getGoldAmount () const {
    return goldAmount;
}

// ������  
void EconomicSystem::buyItem ( const string& itemName ) {
    Item item = _goods->GetItemByName ( itemName );

    if (goldAmount >= item.GetValue ()) {
        subtractGold ( item.GetValue () );
        _mybag->AddItem ( item );
        CCLOG ( "Purchased item: %s" , itemName.c_str () );
        
        // 发送物品购买事件
        auto eventData = std::make_shared<ItemEventData>(itemName, 1);
        QUEUE_EVENT(GameEventType::ITEM_BOUGHT, eventData);
    }
    else {
        CCLOG ( "Not enough gold to buy %s." , itemName.c_str () );
        
        // 发送购买失败事件
        auto eventData = std::make_shared<ItemEventData>(itemName, 0);
        QUEUE_EVENT(GameEventType::BUY_FAILED, eventData);
    }
}

// ���ۺ���  
void EconomicSystem::sellItem ( const string& itemName , int count) {
    Item item = _mybag->GetItemByName ( itemName ); // �ӱ����л�ȡ��Ʒ  

    int itemValue = item.GetValue (); // ��ȡ�����۸�  
    addGold ( itemValue * count );
    _mybag->RemoveItem ( item , count );
    CCLOG ( "Sold item: %s for %d gold." , itemName.c_str () , itemValue * count );
    
    // 发送物品出售事件
    auto eventData = std::make_shared<ItemTransactionEventData>(itemName, itemValue, TransactionType::SELL, count);
    QUEUE_EVENT(GameEventType::ITEM_SOLD, eventData);
}