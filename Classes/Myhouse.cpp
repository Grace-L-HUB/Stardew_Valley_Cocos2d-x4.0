#include "AppDelegate.h"
#include "Myhouse.h"
#include "farm.h"
#include "Crop.h" 
#include "Player.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "GameStateManager.h"


USING_NS_CC;

Myhouse::Myhouse() {}

Myhouse::~Myhouse() {}

bool Myhouse::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    button = cocos2d::Sprite::create("CloseNormal.png");
    this->addChild(button, 11);

    
    if (frombed) {
        if (IsSleep) {
            strength = 100.0f;
        }
        else {
            GoldAmount -= 150;
            if (GoldAmount <= 0) {
                GoldAmount = 0;
            }
            strength = 70.0f;
        }
        frombed = false;
        IsSleep = false;
    }
    

    // ���ñ���ͼƬ
    auto background_real = Sprite::create("Myhouse/myhouse.png");
    background_real->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background_real, 1);
    background_real->setScale(5.5f);

    auto background = Sprite::create("Myhouse/myhouse_path.png");
    this->addChild(background, 5);
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(5.5f);

    auto background_up = Sprite::create("Myhouse/myhouse_up.png");
    background_up->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background_up, 7);
    background_up->setScale(5.5f);

    Vec2 spritePosition = background->getPosition();   // ��ȡ�����λ�ã����ĵ㣩
    Size spriteSize = background->getContentSize();    // ��ȡ����ĳߴ磨���Ⱥ͸߶ȣ�


    // �������½ǵ�����
    Vec2 leftBottomPosition = Vec2(
        spritePosition.x - background->getScaleX() * spriteSize.width / 2,   // ���ĵ� x �����ȥ���ȵ�һ��
        spritePosition.y - background->getScaleY() * spriteSize.height / 2   // ���ĵ� y �����ȥ�߶ȵ�һ��
    );


    Image img;
    if (img.initWithImageFile("Myhouse/myhouse_path.png"))
    {
        int width = img.getWidth();
        int height = img.getHeight();

        // ��ȡ��������
        unsigned char* data = img.getData();

        // �����������أ�����Ƿ������ݣ�͸���ȴ���0��
        for (int y = 0; y < height; y = y + 2)
        {
            for (int x = 0; x < width; x = x + 2)
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
    if (player1->getParent() == NULL) {
        this->addChild(player1, 11);
        player1->setScale(2.7f);
        player1->speed = 7.0f;
        player1->setAnchorPoint(Vec2(0.5f, 0.2f));
        player1->setPosition(1050, 550);
        
    }

    // ��������Ķ�ʱ��
    player1->schedule([=](float dt) {
        player1->player1_move();
        }, 0.05f, "player1_move");

    player1->schedule([=](float dt) {
        player1->player_change();
        }, 0.3f, "player_change");


    // ���㱳����������ź�Χ
    float scaledWidth = background->getContentSize().width * background->getScaleX();
    float scaledHeight = background->getContentSize().height * background->getScaleY();

    // ���� Follow �ı߽� Rect
    auto followRect = cocos2d::Rect(leftBottomPosition.x, leftBottomPosition.y, scaledWidth, scaledHeight);

    // ���� Follow ��������������ڱ�����Χ���ƶ�
    auto followAction = Follow::create(player1, followRect);
    this->runAction(followAction);

    // ���ü�ʱ����ǩ
    TimeUI = Timesystem::create("Myhouse");
    this->addChild(TimeUI, 17);

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

        // �жϵ��λ���Ƿ��ھ��鷶Χ��
        if (button != nullptr && button->getBoundingBox().containsPoint(clickPos)) {
            Director::getInstance()->end();
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, button);

    // ���ü��̼�����
    auto listenerWithPlayer = EventListenerKeyboard::create();
    listenerWithPlayer->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event)
        {
            if (keyCode == EventKeyboard::KeyCode::KEY_ENTER || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                isEnterKeyPressed = true;
                CCLOG("Enter key pressed. ");
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                static int isOpen = 0;
                static InventoryUI* currentInventoryUI = nullptr;  // ���浱ǰ��ʾ�� InventoryUI  
                // �����ǰû�д� InventoryUI�������  
                if (currentInventoryUI == nullptr || isOpen == 0) {
                    isOpen = 1;
                    CCLOG ( "Opening inventory." );
                    currentInventoryUI = InventoryUI::create ( inventory , "Myhouse" );
                    this->addChild ( currentInventoryUI , 20 );  // �� InventoryUI ���ӵ� Town ���ϲ�  
                }
                // ����Ѿ��� InventoryUI����ر���  
                else {
                    isOpen = 0;
                    CCLOG ( "Closing inventory." );
                    this->removeChild ( currentInventoryUI , true );  // �ӵ�ǰ�������Ƴ� InventoryUI  
                    currentInventoryUI = nullptr;  // ����ָ��  
                }
            }
        };

    listenerWithPlayer->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event)
        {
            // �ͷ� Enter ��ʱ������Ϊ false
            if (keyCode == EventKeyboard::KeyCode::KEY_ENTER || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                isEnterKeyPressed = false;
            }
        };

    // �����������ӵ��¼��ַ�����
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerWithPlayer, this);

    return true;
}


Myhouse* Myhouse::create()
{
    Myhouse* scene = new Myhouse();
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}


// �������Ƿ�ӽ�������������
void Myhouse::checkPlayerPosition()
{

    auto visibleSize = Director::getInstance()->getVisibleSize();
    TimeUI->setPosition(visibleSize.width / 2, visibleSize.height / 2);

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
            if (pair.first.first == "myhouse") {  // ��� bool ֵ�Ƿ�Ϊ true
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

    // �Ƿ����ũ��
    if (OutDoor.containsPoint(playerPos)) {
        if (isEnterKeyPressed) {
            player1->removeFromParent();
            GameStateManager::getInstance().changeState(GameStateType::FARM, "slide_left", 0.8f);
        }
    }

    // �Ƿ�˯��
    if (GoBed.containsPoint(playerPos)) {
        if (isEnterKeyPressed) {

            IsNextDay = true;

            isEnterKeyPressed = false;

            day++;

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
                if (pair.first.first == "myhouse") {  // ��� bool ֵ�Ƿ�Ϊ true
                    pair.second = true;
                }
            }

            //�ָ�Ϊ�ܹ�������Ʒ
            for (auto livestock : livestocks) {
                livestock->SetCanProduce ( true );
            }

            IsSleep = true;
            frombed = true;
            remainingTime = 10800;
            player1->removeFromParent();
            auto TempSleep = Sprite::create("character1/player_sleep.png");
            TempSleep->setPosition(1195, 545);
            TempSleep->setScale(1.0f);
            this->addChild(TempSleep, 7);
            GameStateManager::getInstance().changeState(GameStateType::MYHOUSE, "fade", 3.0f);

        }
    }


    for (const auto& point : nonTransparentPixels)
    {
        // ���������������֮��ľ���
        float distance = 0;

        Vec2 temp;
        temp = playerPos;
        temp.x -= player1->speed;
        distance = temp.distance(point);
        if (distance <= 17) {
            player1->moveLeft = false;
        }
        else {
            if (player1->leftpressed == false) {
                player1->moveLeft = true;
            }
        }

        temp = playerPos;
        temp.y -= 10;
        distance = temp.distance(point);
        if (distance <= 15) {
            player1->moveDown = false;
        }
        else {
            if (player1->downpressed == false) {
                player1->moveDown = true;
            }
        }

        temp = playerPos;
        temp.y += 10;
        distance = temp.distance(point);
        if (distance <= 15) {
            player1->moveUp = false;
        }
        else {
            if (player1->uppressed == false) {
                player1->moveUp = true;
            }
        }

        temp = playerPos;
        temp.x += 10;
        distance = temp.distance(point);
        if (distance <= 15) {
            player1->moveRight = false;
        }
        else {
            if (player1->rightpressed == false) {
                player1->moveRight = true;
            }
        }

    }


}








