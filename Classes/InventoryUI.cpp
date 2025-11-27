// InventoryUI.cpp  
#include "InventoryUI.h"  
#include "ui/CocosGUI.h"  
#include "Item.h"  
#include "AppDelegate.h"
#include "quitUI.h"
#include "EventManager.h"

USING_NS_CC;

InventoryUI::~InventoryUI() {
    if (_eventObserver) {
        EventManager::getInstance().removeObserver(_eventObserver);
        _eventObserver.reset();
    }
}


static void problemLoading ( const char* filename )
{
    printf ( "Error while loading: %s\n" , filename );
    printf ( "Depending on how you compiled you might have to add 'Resources/' in front of filenames in CreateCharacterScene.cpp\n" );
}

void InventoryUI::updateCoordinate ( float &x , float &y ) {
    Vec2 position = player1->getPosition ();
    float  Leftboundary = -10000.0f , rightboundary = 10000.0f , upperboundary = 10000.0f , lowerboundary = 10000.0f;
    if (SceneName == "Town") {
        Leftboundary = -170.0f;
        rightboundary = 1773.0f;
        upperboundary = 1498.0f;
        lowerboundary = -222.0f;
    }
    else if (SceneName == "Cave") {
        Leftboundary = 786.0f;
        rightboundary = 817.0f;
        upperboundary = 808.0f;
        lowerboundary = 460.0f;
    }
    else if (SceneName == "Beach") {
        Leftboundary = -315.0f;
        rightboundary = 20000.0f;
        upperboundary = 920.0f;
        lowerboundary = 360.0f;
    }
    else if (SceneName == "Forest") {
        Leftboundary = -600.0f;
        rightboundary = 2197.0f;
        upperboundary = 2200.0f;
        lowerboundary = -850.0f;
    }
    else if (SceneName == "farm") {
        Leftboundary = 637.0f;
        rightboundary = 960.0f;
        upperboundary = 777.0f;
        lowerboundary = 500.0f;
    }
    else if (SceneName == "Barn") {
        Leftboundary = 805.0f;
        rightboundary = 805.0f;
        upperboundary = 569.0f;
        lowerboundary = 569.0f;
    }
    else if (SceneName == "Myhouse") {
        Leftboundary = 800.0f;
        rightboundary = 800.0f;
        upperboundary = 580.0f;
        lowerboundary = 580.0f;
    }
    if (x <= Leftboundary) {
        x = Leftboundary;
    }
    else if (x >= rightboundary) {
        x = rightboundary;
    }
    else {
        x = position.x;
    }

    if (y >= upperboundary) {
        y = upperboundary;
    }
    else if (y <= lowerboundary) {
        y = lowerboundary;
    }
    else {
        y = position.y;
    }
    CCLOG ( "%f  %f" , x , y );
}

void InventoryUI::backgroundcreate(){
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );   
    // ����һ����͸���ĺ�ɫ����
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    auto darkLayer = cocos2d::LayerColor::create ( cocos2d::Color4B ( 0 , 0 , 0 , 120 ) , 5 * visibleSize.width , 5 * visibleSize.height );  // ��ɫ��͸����Ϊ120
    darkLayer->setPosition ( Vec2 ( currentx , currenty ) - visibleSize );// �������ֲ��λ��
    this->addChild ( darkLayer , 0 );
    auto bag = Sprite::create ( "UIresource/beibao/newbag2.png" );
    bag->setTag ( 101 );
    if (bag == nullptr)
    {
        problemLoading ( "'newbag2.png'" );
    }
    else
    {
        // ��ȡԭʼͼƬ�Ŀ���
        float originalWidth = bag->getContentSize ().width;
        float originalHeight = bag->getContentSize ().height;
        // ������Ļ���Ⱥ�ͼƬԭʼ���߼������
        float scaleX = visibleSize.width / originalWidth;
        float scaleY = visibleSize.height / originalHeight;
        // ѡ����С�����ű������Ա�֤ͼƬ��ȫ��ʾ����Ļ���Ҳ�����
        float scale = std::min ( scaleX , scaleY );
        bag->setScale ( scale / 1.5 );
        bag->setPosition ( Vec2 ( currentx , currenty ) );
        this->addChild ( bag , 0 );
    }

    //��������
    auto CharacterDisplay = Sprite::create ( "character1/player_down3.png" );
    if (CharacterDisplay == nullptr)
    {
        problemLoading ( "'character1/player_down3.png'" );
    }
    else
    {
        float originalWidth = CharacterDisplay->getContentSize ().width;
        float originalHeight = CharacterDisplay->getContentSize ().height;
        float scaleX = visibleSize.width / originalWidth;
        float scaleY = visibleSize.height / originalHeight;
        float scale = std::min ( scaleX , scaleY );
        CharacterDisplay->setScale ( scale / 7.5 );
        CharacterDisplay->setPosition ( Vec2 ( currentx - visibleSize.width * 0.2 , currenty - visibleSize.height * 0.12 ) );
        this->addChild ( CharacterDisplay , 2 );
    }
    auto NameDisplay = Label::createWithTTF ( protagonistName , "fonts/Marker Felt.ttf" , 45 );
    NameDisplay->setTextColor ( Color4B::BLACK );
    NameDisplay->setPosition ( Vec2 ( currentx + visibleSize.width * 0.1 , currenty - visibleSize.height * 0.07 ) );
    this->addChild ( NameDisplay , 4 );

    auto FarmNameDisplay = Label::createWithTTF ( FarmName , "fonts/Marker Felt.ttf" , 45 );
    FarmNameDisplay->setTextColor ( Color4B::BLACK );
    FarmNameDisplay->setPosition ( Vec2 ( currentx + visibleSize.width * 0.1 , currenty - visibleSize.height * 0.15 ) );
    this->addChild ( FarmNameDisplay , 4 );

    if (!_goldLabel) {
        _goldLabel = Label::createWithTTF(std::to_string(GoldAmount) + "g", "fonts/Marker Felt.ttf", 38);
        _goldLabel->setTextColor(Color4B::BLACK);
        _goldLabel->setPosition(Vec2(currentx + visibleSize.width * 0.1f, currenty - visibleSize.height * 0.23f));
        this->addChild(_goldLabel, 4);
    } else {
        _goldLabel->setString(std::to_string(GoldAmount) + "g");
    }
}

void InventoryUI::Itemblock ( Inventory* inventory ) {
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    Vec2 origin = Director::getInstance ()->getVisibleOrigin ();
    _inventory = inventory;
    _selectedSlot = 1; // Ĭ��ѡ�е�һ����λ  


    // ��ʼ����Ʒ�� Sprite 
    for (int m = 0; m < 3; m++)
    {
        for (int i = 0; i < kRowSize; ++i) {
            auto slot = Sprite::create ( "UIresource/beibao/itemblock.png" );
            auto bag = getChildByTag ( 101 );
            // ��ȡԭʼͼƬ�Ŀ���
            float originalWidth = slot->getContentSize ().width;
            float originalHeight = slot->getContentSize ().height;
            // ������Ļ���Ⱥ�ͼƬԭʼ���߼������
            float scaleX = visibleSize.width / originalWidth;
            float scaleY = visibleSize.height / originalHeight;
            // ѡ����С�����ű������Ա�֤ͼƬ��ȫ��ʾ����Ļ���Ҳ�����
            float scale = std::min ( scaleX , scaleY );
            slot->setScale ( scale / 16.5 );
            float bagWidth = bag->getContentSize ().width;
            float bagHeight = bag->getContentSize ().height;
            slot->setPosition ( currentx - bagWidth * 0.545 + (originalWidth * scale / 16.5 + 5) * i , currenty + bagHeight * 1.73 / 3.643 - m * (originalHeight * scale / 16.5 + 10) ); // �����λλ��  
            slot->setTag ( i + 1 ); // ���ò�λ�ı�ǩ  
            this->addChild ( slot , 2 );

            _itemSlots.pushBack ( slot );

        }
    }
}

bool InventoryUI::init ( Inventory* inventory , std::string sceneName ) {
    if (!Layer::init ()) {
        return false;
    }

    SceneName = sceneName;

    backgroundcreate ();

    Buttons_switching ();

    Itemblock ( inventory );

    auto visibleSize = Director::getInstance ()->getVisibleSize ();

    updateDisplay (); // ������ʾ����  

    registerEventObservers();

    return true;
}

void InventoryUI::Buttons_switching () {
    Vec2 position = player1->getPosition ();
    float currentx = position.x , currenty = position.y;
    updateCoordinate ( currentx , currenty );
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    //ͼ����ʾ
    auto bagkey = Sprite::create ( "UIresource/beibao/bagkey.png" );
    auto Skillkey = Sprite::create ( "UIresource/beibao/Skillkey.png" );
    auto intimacykey = Sprite::create ( "UIresource/beibao/intimacykey.png" );
    auto quitkey = Sprite::create ( "UIresource/beibao/quit.png" );
    if (bagkey == nullptr)
    {
        problemLoading ( "'bagkey.png'" );
    }
    else
    {
        // ��ȡԭʼͼƬ�Ŀ���
        float originalWidth = bagkey->getContentSize ().width;
        float originalHeight = bagkey->getContentSize ().height;
        // ������Ļ���Ⱥ�ͼƬԭʼ���߼������
        float scaleX = visibleSize.width / originalWidth;
        float scaleY = visibleSize.height / originalHeight;
        // ѡ����С�����ű������Ա�֤ͼƬ��ȫ��ʾ����Ļ���Ҳ�����
        float scale = std::min ( scaleX , scaleY );
        bagkey->setScale ( scale / 16.5 );
        bagkey->setPosition ( Vec2 ( currentx - visibleSize.width * 0.25 , currenty + visibleSize.height * 0.305 ) );//0.305��ѡ��ʱλ��
        Skillkey->setScale ( scale / 16.5 );
        Skillkey->setPosition ( Vec2 ( currentx - visibleSize.width * 0.19 , currenty + visibleSize.height * 0.315 ) );//0.315��δѡ��ʱλ��
        intimacykey->setScale ( scale / 16.5 );
        intimacykey->setPosition ( Vec2 ( currentx - visibleSize.width * 0.13 , currenty + visibleSize.height * 0.315 ) );
        quitkey->setScale ( scale / 16.5 );
        quitkey->setPosition ( Vec2 ( currentx - visibleSize.width * 0.07 , currenty + visibleSize.height * 0.315 ) );
        this->addChild ( bagkey , 2 );
        this->addChild ( Skillkey , 2 );
        this->addChild ( intimacykey , 2 );
        this->addChild ( quitkey , 2 );
    }

    //�����Լ��л�Layer
    auto listener = EventListenerMouse::create ();
    listener->onMouseDown = [this, bagkey, Skillkey,intimacykey , quitkey]( EventMouse* event ) {
        Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
        mousePos = this->convertToNodeSpace ( mousePos );
        //CCLOG ( "X:%f,Y:%f" , event->getCursorX () , event->getCursorY () );
        if (bagkey->getBoundingBox ().containsPoint ( mousePos )) {
        }
        else if (Skillkey->getBoundingBox ().containsPoint ( mousePos )) {
            std::string nowScene = SceneName;
            this->removeFromParent ();
            Director::getInstance ()->getRunningScene ()->addChild ( SkillTreeUI::create ( nowScene ) , 20 );
        }
        else if (intimacykey->getBoundingBox ().containsPoint ( mousePos )) {
            // �Ƴ���ǰ��Layer
            std::string nowScene = SceneName;
            this->removeFromParent ();
            Director::getInstance ()->getRunningScene ()->addChild ( intimacyUI::create ( nowScene ) , 20 );
        }
        else if (quitkey->getBoundingBox ().containsPoint ( mousePos )) {
            std::string nowScene = SceneName;
            this->removeFromParent ();
            Director::getInstance ()->getRunningScene ()->addChild ( quitUI::create ( nowScene ) , 20 );
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , this );
}

InventoryUI* InventoryUI::create ( Inventory* inventory , std::string sceneName ) {
    InventoryUI* ret = new InventoryUI ();
    if (ret && ret->init ( inventory , sceneName )) {
        ret->autorelease ();
        return ret;
    }
    CC_SAFE_DELETE ( ret );
    return nullptr;
}

void InventoryUI::updateDisplay () {
    if (!_inventory) {
        CCLOG ( "Warning: _inventory is nullptr" );
        return; // �˳�����  
    }

    for (int m = 0; m < 3; m++) {
        // ��ȡ��ǰѡ�����Ʒ�Ĳ�λ  
        for (int i = 0; i < kRowSize; ++i) {
            int serial_number = i + m * 12;
            auto slot = _itemSlots.at ( serial_number );
            slot->setVisible ( true ); // ȷ����ʾ���в�λ  

            // ��ȡ��λ��Ʒ  
            auto item = _inventory->GetItemAt ( serial_number + 1 ); // ��ȡ�ض���λ����Ʒ��ע���λ��1��ʼ 

            // ��ȡ��Ʒ����   
            int itemCount = _inventory->GetItemCountAt ( serial_number + 1 ); // ��ȡ�ò�λ����Ʒ����  

            if (item) {
                CCLOG ( "Item in slot %d: %s" , serial_number + 1 , item->GetName ().c_str () );
            }
            else {
                CCLOG ( "No item in slot %d" , serial_number + 1 );
            }

            // �����Ҫ��ȡ�ض���λ����Ʒ��ʹ�� GetItemAt(int position) �����º���  

            // ���²�λ�Ӿ�����  
            if (item) {
                // ���֮ǰ���ӽڵ�  
                slot->removeAllChildren ();

                // ͼƬ·��  
                auto itemSprite = Sprite::create ( item->initial_pic );
                if (itemSprite) {
                    itemSprite->setPosition ( slot->getContentSize () / 2 );
                    itemSprite->setScale ( 0.7f );
                    slot->addChild ( itemSprite , 3 );
                    CCLOG ( "Loading item sprite: %s" , item->initial_pic.c_str () );
                }
                else {
                    CCLOG ( "Error loading item sprite: %s" , item->initial_pic.c_str () );
                }

                // ���� item �������������������ǩ�������Ҫ����  
                // ���������ﴴ��һ�� Label ��ʾ����  
                auto countLabel = static_cast<Label*>(slot->getChildByTag ( 200 + serial_number )); // ʹ�ò�λ�ı�ǩ����������ǩ��ΨһID  
                if (!countLabel) {
                    // �����ǩ�����ڣ������µı�ǩ  
                    countLabel = Label::createWithSystemFont ( std::to_string ( itemCount ) , "fonts/Comic Sans MS.ttf" , 20 );
                    countLabel->setTextColor ( Color4B ( 255 , 153 , 0 , 255 ) );
                    countLabel->setPosition ( slot->getContentSize ().width * 0.8 , slot->getContentSize ().height * 0.2 ); // ����λ���ڲ�λ���·�  
                    countLabel->setTag ( 200 + serial_number ); // ���ñ�ǩ  
                    slot->addChild ( countLabel , 4 ); // ���ӵ��㼶��  
                }
                else {
                    // �����ǩ���ڣ���������  
                    countLabel->setString ( std::to_string ( itemCount ) );
                }


                // ���Ӵ����¼�  
                auto listener = EventListenerMouse::create ();

                // ����ƶ��¼�
                listener->onMouseMove = [this , slot , itemSprite , countLabel]( EventMouse* event ) {
                    Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
                    mousePos = this->convertToNodeSpace ( mousePos );
                    if (slot && slot->getBoundingBox ().containsPoint ( mousePos )) {
                        itemSprite->setScale ( 1.1f );
                        countLabel->setScale ( 1.5f );
                    }
                    else if (slot && itemSprite != currentItemSprite) {
                        itemSprite->setScale ( 0.7f ); // �ָ�ԭ��С
                        countLabel->setScale ( 1.0f );
                    }
                    };

                // ������갴���¼�  
                listener->onMouseDown = [this , slot , itemSprite, serial_number]( EventMouse* event ) {
                    Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
                    mousePos = this->convertToNodeSpace ( mousePos );

                    // �������Ƿ����� slot  
                    if (slot->getBoundingBox ().containsPoint ( mousePos )) {
                        if (!isClick) {
                            currentItemSprite = itemSprite; // ��¼��ǰѡ�����Ʒ
                            _selectedSlot = serial_number + 1;
                            CCLOG ( "_selectedSlot:%d" , _selectedSlot );
                        }
                        else {
                            currentItemSprite = nullptr;
                        }
                        isClick = !isClick;
                    }
                    };

                _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , itemSprite );
            }
            else {
                slot->removeAllChildren (); // ��ղ�λ  

                // ���������ǩ  
                auto countLabel = static_cast<Label*>(slot->getChildByTag ( 200 + i ));
                if (countLabel) {
                    countLabel->removeFromParent (); // �Ƴ�������ǩ  
                }
            }
        }
    }

    // ������Ʒ��Ϣ��ǩ�����ڵ��ԣ�  
    if (_itemLabel) { // ��� _itemLabel �Ƿ�Ϊ nullptr  
        if (auto selectedItem = _inventory->GetSelectedItem ()) {
            _itemLabel->setString ( "Selected: " + selectedItem->GetName () );
        }
        else {
            _itemLabel->setString ( "No item selected." );
        }
    }
    else {
        CCLOG ( "Warning: _itemLabel is nullptr" );
    }
}

void InventoryUI::registerEventObservers() {
    _eventObserver = std::make_shared<UIEventObserver>(this);
    _eventObserver
        ->on(GameEventType::GOLD_AMOUNT_CHANGED, [this](const GameEvent& event) {
            if (auto data = event.getData<GoldChangedEventData>()) {
                refreshGoldLabel(data->newAmount);
            }
        })
        .on(GameEventType::ITEM_ADDED, [this](const GameEvent&) {
            this->updateDisplay();
        })
        .on(GameEventType::ITEM_REMOVED, [this](const GameEvent&) {
            this->updateDisplay();
        });
    EventManager::getInstance().addObserver(_eventObserver);
}

void InventoryUI::refreshGoldLabel(int amount) {
    if (_goldLabel) {
        _goldLabel->setString(std::to_string(amount) + "g");
    }
}
