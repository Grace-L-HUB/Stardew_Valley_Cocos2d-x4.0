#include "EconomicSystem.h"  
#include "EventManager.h"
#include "cocos2d.h"  // ??????? cocos2d.h ????? CCLOG  
#include <iostream>  

extern int GoldAmount;

// ???????????????????4000  
EconomicSystem::EconomicSystem ( Inventory* mybag , Inventory* goods)
    : goldAmount (GoldAmount) , _mybag ( mybag ) , _goods ( goods ) {
    // ?????????????????????  
}

// ????????  
EconomicSystem::~EconomicSystem () {
    // ????????  
    GoldAmount = goldAmount;
}

// ??????????  
void EconomicSystem::addGold ( int amount ) {
    if (amount > 0) {
        int oldAmount = goldAmount;
        goldAmount += amount;
        CCLOG ( "Added %d gold. Total: %d gold." , amount , goldAmount );
        EventManager::getInstance().publishGoldChanged(oldAmount, goldAmount, "economic_add");
    }
    else {
        CCLOG ( "Amount to add must be positive." );
    }
}

// ??????????  
void EconomicSystem::subtractGold ( int amount ) {
    if (amount > 0 && amount <= goldAmount) {
        int oldAmount = goldAmount;
        goldAmount -= amount;
        CCLOG ( "Subtracted %d gold. Total: %d gold." , amount , goldAmount );
        EventManager::getInstance().publishGoldChanged(oldAmount, goldAmount, "economic_subtract");
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

// ?????§ß???????????  
int EconomicSystem::getGoldAmount () const {
    return goldAmount;
}

// ??????  
void EconomicSystem::buyItem ( const string& itemName ) {
    Item item = _goods->GetItemByName ( itemName );

    if (goldAmount >= item.GetValue ()) {
        subtractGold ( item.GetValue () );
        _mybag->AddItem ( item );
        CCLOG ( "Purchased item: %s" , itemName.c_str () );
        EventManager::getInstance().publishItemBought(itemName, item.GetValue());
    }
    else {
        CCLOG ( "Not enough gold to buy %s." , itemName.c_str () );
    }
}

// ???????  
void EconomicSystem::sellItem ( const string& itemName , int count) {
    Item item = _mybag->GetItemByName ( itemName ); // ??????§Ý?????  

    int itemValue = item.GetValue (); // ??????????  
    addGold ( itemValue * count );
    _mybag->RemoveItem ( item , count );
    CCLOG ( "Sold item: %s for %d gold." , itemName.c_str () , itemValue * count );
    EventManager::getInstance().publishItemSold(itemName, itemValue * count);
}