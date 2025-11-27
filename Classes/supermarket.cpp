#include "AppDelegate.h"
#include "supermarket.h"
#include "Town.h"
#include "Player.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "StoreUI.h"
#include "GameStateManager.h"

USING_NS_CC;

extern int remainingTime;
extern Player* player1;
extern Town* town;
extern supermarket* seedshop;
extern Inventory* inventory;

supermarket::supermarket() {}

supermarket::~supermarket() {}

bool supermarket::init()
{
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    button = cocos2d::Sprite::create("CloseNormal.png");
    this->addChild(button, 11);

    StoreItem = new Inventory ();

    // ����  
    StoreItem->AddItem ( AnimalChicken );

    StoreItem->AddItem ( AnimalSheep );

    StoreItem->AddItem ( AnimalCow );

    StoreItem->AddItem ( Blueberry_Seeds );

    StoreItem->AddItem ( Corn_Seeds );

    StoreItem->AddItem ( Potato_Seeds );

    StoreItem->AddItem ( Pumpkin_Seeds );

    StoreItem->AddItem ( Wheat_Seeds );

    StoreItem->AddItem ( Apple_Sapling );

    StoreItem->AddItem ( Apricot_Sapling );


    StoreItem->AddItem ( Banana_Sapling );

    StoreItem->AddItem ( Cherry_Sapling );

    StoreItem->AddItem ( Mango_Sapling );

    StoreItem->AddItem ( Orange_Sapling );

    StoreItem->AddItem ( Peach_Sapling );

    StoreItem->AddItem ( Pomegranate_Sapling );

    /*
    // ����������Ʒ�б�  
    StoreItem->AddItem ( Bean_Starter );

    StoreItem->AddItem ( Carrot_Seeds );

    StoreItem->AddItem ( Cauliflower_Seeds );

    StoreItem->AddItem ( Coffee_Bean );

    StoreItem->AddItem ( Garlic_Seeds );

    StoreItem->AddItem ( Jazz_Seeds );

    StoreItem->AddItem ( Kale_Seeds );

    StoreItem->AddItem ( Parsnip_Seeds );

    StoreItem->AddItem ( Potato_Seeds );

    StoreItem->AddItem ( Rhubarb_Seeds );

    StoreItem->AddItem ( Rice_Shoot );

    StoreItem->AddItem ( Strawberry_Seeds );

    StoreItem->AddItem ( Tulip_Bulb );

    // �ļ�������Ʒ�б�  
    StoreItem->AddItem ( Amaranth_Seeds );

    StoreItem->AddItem ( Artichoke_Seeds );

    StoreItem->AddItem ( Beet_Seeds );

    StoreItem->AddItem ( Blueberry_Seeds );

    StoreItem->AddItem ( Bok_Choy_Seeds );

    StoreItem->AddItem ( Broccoli_Seeds );

    StoreItem->AddItem ( Corn_Seeds );

    StoreItem->AddItem ( Cranberry_Seeds );

    StoreItem->AddItem ( Eggplant_Seeds );

    StoreItem->AddItem ( Fairy_Seeds );

    StoreItem->AddItem ( Grape_Starter );

    StoreItem->AddItem ( Hops_Starter );

    StoreItem->AddItem ( Melon_Seeds );

    StoreItem->AddItem ( Pepper_Seeds );

    StoreItem->AddItem ( Poppy_Seeds );

    StoreItem->AddItem ( Pumpkin_Seeds );

    StoreItem->AddItem ( Radish_Seeds );

    StoreItem->AddItem ( Red_Cabbage_Seeds );

    StoreItem->AddItem ( Spangle_Seeds );

    StoreItem->AddItem ( Starfruit_Seeds );

    StoreItem->AddItem ( Summer_Squash_Seeds );

    StoreItem->AddItem ( Sunflower_Seeds );

    StoreItem->AddItem ( Tomato_Seeds );

    StoreItem->AddItem ( Wheat_Seeds );

    StoreItem->AddItem ( Yam_Seeds );

    // ����������Ʒ�б�  
    StoreItem->AddItem ( Powdermelon_Seeds );
    */

    // �����б�  
    StoreItem->AddItem ( Backpack_36 );

    StoreItem->AddItem ( Backpack );

    StoreItem->AddItem ( Advanced_Iridium_Rod );

    StoreItem->AddItem ( Axe );

    StoreItem->AddItem ( Bamboo_Pole );

    StoreItem->AddItem ( Copper_Axe );

    StoreItem->AddItem ( Copper_Hoe );

    StoreItem->AddItem ( Copper_Pan );

    StoreItem->AddItem ( Copper_Pickaxe );

    StoreItem->AddItem ( Copper_Watering );

    StoreItem->AddItem ( Fiberglass_Rod );

    StoreItem->AddItem ( Gold_Axe );

    StoreItem->AddItem ( Gold_Hoe );

    StoreItem->AddItem ( Gold_Pan );

    StoreItem->AddItem ( Gold_Pickaxe );

    StoreItem->AddItem ( Gold_Watering_Can );

    StoreItem->AddItem ( Golden_Scythe );

    StoreItem->AddItem ( Hoe );

    StoreItem->AddItem ( Pickaxe );

    StoreItem->AddItem ( Milk_Pail );

    StoreItem->AddItem ( Scythe );

    StoreItem->AddItem ( Steel_Axe );

    StoreItem->AddItem ( Steel_Hoe );

    StoreItem->AddItem ( Steel_Pan );

    StoreItem->AddItem ( Steel_Pickaxe );

    StoreItem->AddItem ( Steel_Watering );

    StoreItem->AddItem ( Trash_Can_Copper );

    StoreItem->AddItem ( Watering_Can );

    StoreItem->AddItem ( Trash_Can_Steel );

    // �����б�  
    StoreItem->AddItem ( Apple_Sapling );

    StoreItem->AddItem ( Apricot_Sapling );

    StoreItem->AddItem ( Banana_Sapling );

    StoreItem->AddItem ( Cherry_Sapling );

    StoreItem->AddItem ( Mango_Sapling );

    StoreItem->AddItem ( Orange_Sapling );

    StoreItem->AddItem ( Peach_Sapling );

    StoreItem->AddItem ( Pomegranate_Sapling );

    // ���ü�ʱ����ǩ
    TimeUI = Timesystem::create ( "supermarket" );
    this->addChild ( TimeUI , 13 );

    // ��ʼ�����ż�
    opendoor = Sprite::create("opendoor.png");
    this->addChild(opendoor, 11);
    opendoor->setVisible(false);

    // ���ñ���ͼƬ
    auto background_real = Sprite::create("supermarket/supermarket.png");
    background_real->setPosition(Vec2(visibleSize.width / 2 + 1700, visibleSize.height / 2 + 370));
    this->addChild(background_real, 1);
    background_real->setScale(6.7f);

    auto background_up = Sprite::create("supermarket/supermarket_up.png");
    background_up->setPosition(Vec2(visibleSize.width / 2 + 1700, visibleSize.height / 2 + 370));
    this->addChild(background_up, 7);
    background_up->setScale(6.7f);

    auto background = Sprite::create("supermarket/supermarket_path.png");
    this->addChild(background, 5);
    background->setPosition(Vec2(visibleSize.width / 2 + 1700, visibleSize.height / 2 + 370));
    background->setScale(6.7f);


    Vec2 spritePosition = background->getPosition();   // ��ȡ�����λ�ã����ĵ㣩
    Size spriteSize = background->getContentSize();    // ��ȡ����ĳߴ磨���Ⱥ͸߶ȣ�
  

    // �������½ǵ�����
    Vec2 leftBottomPosition = Vec2(
        spritePosition.x - background->getScaleX() * spriteSize.width / 2,   // ���ĵ� x �����ȥ���ȵ�һ��
        spritePosition.y - background->getScaleY() * spriteSize.height / 2   // ���ĵ� y �����ȥ�߶ȵ�һ��
    );
   

    Image img;
    if (img.initWithImageFile("supermarket/supermarket_path.png"))
    {
        int width = img.getWidth();
        int height = img.getHeight();

        // ��ȡ��������
        unsigned char* data = img.getData();

        // �����������أ�����Ƿ������ݣ�͸���ȴ���0��
        for (int y = 0; y < height; y = y + 4)
        {
            for (int x = 0; x < width; x = x + 4)
            {
                // ��ȡ��ǰ���ص� RGBA ֵ
                int index = (y * width + x) * 4;  // ÿ������ռ�� 4 ���ֽ� (RGBA)
                unsigned char a = data[index + 3];  // ͸����

                // ���͸���� (alpha) ���� 0����ʾ������������
                if (a > 0)
                {
                    float screenX = leftBottomPosition.x + x * background->getScaleX();
                    float screenY = leftBottomPosition.y + (height - y - 1) * background->getScaleY();  // ע�� Y �ᷴ��
                    nonTransparentPixels.push_back(Vec2(screenX, screenY));  // ��¼��Ļ����
                }
            }
        }
    }


    // ��ʼ����ɫ���������ӵ�����
    this->addChild(player1, 5);
    player1->schedule([=](float dt) {
        player1->player1_move();
        }, 0.05f, "player1_move");

    player1->schedule([=](float dt) {
        player1->player_change();
        }, 0.3f, "player_change");
    player1->setPosition(Vec2(visibleSize.width / 2 + 43, visibleSize.height / 2 - 101));  // ������ҳ�ʼλ��
    player1->setScale(3.1f);
    player1->setAnchorPoint(Vec2(0.5f, 0.2f));
    player1->speed = 5.3f;

    // ���㱳����������ź�Χ
    float scaledWidth = background->getContentSize().width * background->getScaleX();
    float scaledHeight = background->getContentSize().height * background->getScaleY();

    // ���� Follow �ı߽� Rect
    auto followRect = cocos2d::Rect(leftBottomPosition.x, leftBottomPosition.y, scaledWidth, scaledHeight);

    // ���� Follow ��������������ڱ�����Χ���ƶ�
    auto followAction = Follow::create(player1, followRect);
    this->runAction(followAction);


    // ���ڸ������״̬
    this->schedule([this](float dt) {
        this->checkPlayerPosition();  // �������Ƿ�ӽ�������
        }, 0.01f, "check_position_key");

    auto listener = EventListenerMouse::create();

    listener->onMouseDown = [this](Event* event) {
       
        // ��ȡ�������λ��
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 clickPos(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        clickPos = this->convertToNodeSpace(clickPos);
        // ����������λ��
        CCLOG("Mouse Position: (%f, %f)", clickPos.x, clickPos.y);

        // �жϵ��λ���Ƿ��ھ��鷶Χ��
        if (button != nullptr && button->getBoundingBox().containsPoint(clickPos)) {
            CCLOG("Button clicked!");
            Director::getInstance()->end();
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, button);

    // ���ü��̼�����
    auto listenerWithPlayer = EventListenerKeyboard::create();
    listenerWithPlayer->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event)
        {
            // ��¼ Enter ��������
            if (keyCode == EventKeyboard::KeyCode::KEY_ENTER || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                isEnterKeyPressed = true;
                CCLOG("Enter key pressed.");
            }
            // ������������  
            if (keyCode == EventKeyboard::KeyCode::KEY_P) {
                static StoreUI* currentStoreUI = nullptr;  // ���浱ǰ��ʾ�� StoreUI  
                // �����ǰû�д� StoreUI�������  
                if (currentStoreUI == nullptr) {
                    CCLOG ( "Opening inventory." );
                    currentStoreUI = StoreUI::create ( inventory , StoreItem );
                    this->addChild ( currentStoreUI , 20 );
                } 
                else {
                    CCLOG ( "Closing inventory." );
                    this->removeChild ( currentStoreUI , true );
                    currentStoreUI = nullptr;  // ����ָ��  
                }
            }
        };

    listenerWithPlayer->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event)
        {
            // �ͷ� Enter ��ʱ������Ϊ false
            if (keyCode == EventKeyboard::KeyCode::KEY_ENTER || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                isEnterKeyPressed = false;
                CCLOG("Enter key released.");
            }
        };

    // �����������ӵ��¼��ַ�����
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerWithPlayer, this);


    return true;
}


supermarket* supermarket::create()
{
    supermarket* scene = new supermarket();
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

// �������Ƿ�ӽ�������������
void supermarket::checkPlayerPosition()
{
    // ��ȡ��ҵ�λ��
    Vec2 playerPos = player1->getPosition();

    // ���¼�ʱ����ʾ
    remainingTime++;
    if (remainingTime == 43200) {

        day++;

        IsNextDay = true;

        if (day == 8) {
            if (Season == "Spring") {
                Season = "Summer";
            }
            else if (Season == "Summer") {
                Season = "Autumn";
            }
            else {
                Season = "Winter";
            }
            day = 1;
        }

        if (day % 3 == 1) {
            Weather = "Rainy";
        }
        else {
            Weather = "Sunny";
        }

        if ((Season == "Spring") && (day == 1)) {
            Festival = "Fishing Day";
        }
        else {
            Festival = "Noraml Day";
        }


        for (auto it = Crop_information.begin(); it != Crop_information.end();) {

            auto crop = *it;  // �����õ������Է��� Crop ����

            if (day == 1) {
                crop->watered = true;
            }
            if ((day - 1) % 3 == 1) {
                crop->watered = true;
            }

            // �ж�ǰһ���Ƿ�ˮ
            if ((crop->watered == false) && (crop->GetPhase() != Phase::MATURE)) {
                // �ж��Ƿ��Ѿ������ή״̬
                if (crop->GetPhase() != Phase::SAPLESS) {
                    crop->ChangePhase(Phase::SAPLESS);
                    crop->ChangMatureNeeded(2); // �ӳ������ջ�
                    it++;
                }
                else {
                    // ɾ��Ԫ�ز����µ�����
                    it = Crop_information.erase(it);
                }

            }
            else {
                // ����״̬
                crop->UpdateGrowth();
                it++;
            }

        }

        for (auto& pair : F_lastplace) {
            if (pair.second) {
                pair.second = false;
            }
            if (pair.first.first == "myhouse") {  // ��� bool ֵ�Ƿ�Ϊ true
                pair.second = true;
            }
        }

        for (auto& pair : T_lastplace) {
            if (pair.second) {
                pair.second = false;
            }
            if (pair.first.first == "forest") {
                pair.second = true;
            }
        }

        for (auto& pair : W_lastplace) {
            if (pair.second) {
                pair.second = false;
            }
            if (pair.first.first == "farm") {
                pair.second = true;
            }
        }


        //�ָ�Ϊ�ܹ�������Ʒ
        for (auto livestock : livestocks) {
            livestock->SetCanProduce ( true );
        }

        IsSleep = false;
        frombed = true;
        remainingTime = 10800;
        player1->removeFromParent();
        GameStateManager::getInstance().changeState(GameStateType::MYHOUSE, "fade", 1.0f);

    }


    // ���±�ǩλ��
    float currentx = 0, currenty = 0;
    if (playerPos.x <= 743) {
        currentx = 743;
    }
    else if (playerPos.x >= 1773) {
        currentx = 1773;
    }
    else {
        currentx = playerPos.x;
    }

    if (playerPos.y <= -82) {
        currenty = -82;
    }
    else {
        currenty = playerPos.y;
    }


    TimeUI->setPosition(currentx, currenty);
    button->setPosition(currentx + 690, currenty - 590);
   
    // �������Ƿ����Ŀ�����򣬲��Ұ��� Enter ��
    if (Region_Out.containsPoint(playerPos)) {
        // ��ҽ���Ŀ������
        opendoor->setVisible(true);
        opendoor->setPosition(playerPos.x + 110, playerPos.y + 30);


        if (isEnterKeyPressed) {
            // ��ӡ������Ϣ����� Enter ����״̬
            CCLOG("Player in target area, isEnterKeyPressed: %d", isEnterKeyPressed);
            // ���ó����л��߼�
            player1->removeFromParent();
            GameStateManager::getInstance().changeState(GameStateType::TOWN, "slide_right", 0.6f);
          
        }

    }
    else {
        opendoor->setVisible(false);
    }

    for (const auto& point : nonTransparentPixels)
    {
        // ���������������֮��ľ���
        float distance = 0;

        Vec2 temp;
        temp = playerPos;
        temp.x -= player1->speed;
        distance = temp.distance(point);
        if (distance <= 55) {
            player1->moveLeft = false;
           
        }
        else {
            if (player1->leftpressed == false) {
                player1->moveLeft = true;
            }
        }



        temp = playerPos;
        temp.y -= player1->speed;
        distance = temp.distance(point);
        if (distance <= 25) {
            player1->moveDown = false;
        }
        else {
            if (player1->downpressed == false) {
                player1->moveDown = true;
            }
        }

        temp = playerPos;
        temp.y += player1->speed;
        distance = temp.distance(point);
        if (distance <= 45) {
            player1->moveUp = false;
        }
        else {
            if (player1->uppressed == false) {
                player1->moveUp = true;
            }
        }

        temp = playerPos;
        temp.x += player1->speed;
        distance = temp.distance(point);
        if (distance <= 55) {
            player1->moveRight = false;
        }
        else {
            if (player1->rightpressed == false) {
                player1->moveRight = true;
            }
        }

    }



}




