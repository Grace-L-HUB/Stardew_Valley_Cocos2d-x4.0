#include "AppDelegate.h"
#include "farm.h"
#include "Forest.h"
#include "Player.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "vector"
#include "GameStateManager.h"

USING_NS_CC;

Forest::Forest() {}

Forest::~Forest() {}

bool Forest::init()
{

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    button = cocos2d::Sprite::create("CloseNormal.png");
    this->addChild(button, 11);

    // ���ü�ʱ����ǩ
    TimeUI = Timesystem::create ( "Forest" );
    this->addChild(TimeUI, 13);

    // �ָ���ֲ
    AllInitialize_ore();

    if (Weather == "Rainy") {
        // ����
        createRainEffect();
    }

    // ���ñ���ͼƬ
    auto background_real = Sprite::create("forest/forest.png");
    background_real->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background_real, 1);
    background_real->setScale(2.3f);

    auto background = Sprite::create("forest/forest_path.png");
    this->addChild(background, 5);
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(2.3f);

    auto background_up = Sprite::create("forest/forest_up.png");
    background_up->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background_up, 7);
    background_up->setScale(2.3f);


    Vec2 spritePosition = background->getPosition();   // ��ȡ�����λ�ã����ĵ㣩
    Size spriteSize = background->getContentSize();    // ��ȡ����ĳߴ磨���Ⱥ͸߶ȣ�
  

    // �������½ǵ�����
    Vec2 leftBottomPosition = Vec2(
        spritePosition.x - background->getScaleX() * spriteSize.width / 2,   // ���ĵ� x �����ȥ���ȵ�һ��
        spritePosition.y - background->getScaleY() * spriteSize.height / 2   // ���ĵ� y �����ȥ�߶ȵ�һ��
    );
   

    Image img;
    if (img.initWithImageFile("forest/forest_path.png"))
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
                int index = (y * width + x) * 4;    // ÿ������ռ�� 4 ���ֽ� (RGBA)
                unsigned char a = data[index + 3];  // ͸����

                // ���͸���� (alpha) ���� 0����ʾ������������
                if (a > 0)
                {
                    float screenX = leftBottomPosition.x + x * background->getScaleX();
                    float screenY = leftBottomPosition.y + (height - y - 1) * background->getScaleY();  // ע�� Y �ᷴ��
                    nonTransparentPixels.push_back(Vec2(screenX, screenY));                             // ��¼��Ļ����
                }
            }
        }
    }

    // ��ʼ����ɫ���������ӵ�����
    if (player1->getParent() == NULL) {
        this->addChild(player1, 6);
        for (auto& pair : W_lastplace) {
            if (pair.second == true) { 
                player1->setPosition(pair.first.second);
                pair.second = false;
            }
        }
        player1->speed = 6.1f;
        //��Ϊ�������
       /* player1->speed = 15.0f;*/
        player1->setScale(2.3f);
        player1->setAnchorPoint(Vec2(0.5f, 0.2f));
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
            else if (keyCode == EventKeyboard::KeyCode::KEY_L) {
                isLKeyPressed = true;
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                static int isOpen = 0;
                static InventoryUI* currentInventoryUI = nullptr;  // ���浱ǰ��ʾ�� InventoryUI  
                // �����ǰû�д� InventoryUI�������  
                if (currentInventoryUI == nullptr || isOpen == 0) {
                    isOpen = 1;
                    CCLOG ( "Opening inventory." );
                    currentInventoryUI = InventoryUI::create ( inventory , "Forest" );
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
            if (keyCode == EventKeyboard::KeyCode::KEY_L) {
                isLKeyPressed = false;
            }
        };

    // �����������ӵ��¼��ַ�����
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerWithPlayer, this);

    //�����µı�����ʾ
    miniBag = mini_bag::create ( inventory );
    miniBag->setScale ( 1.0f );
    Vec2 pos = miniBag->getPosition ();
    if (miniBag != NULL) {
        cocos2d::log ( "miniBagtest %f" , pos.x );
    }
    if (!this->getChildByName ( "mini_bag" )) {
        this->addChild ( miniBag , 10 , "mini_bag" );
    }


    // ������Ʒ��
    schedule ( [=]( float deltaTime ) {
        if (inventory->is_updated == true) {
            miniBag->updateDisplay ();
            inventory->is_updated = false;
        }
        } , 0.1f , "item_update_key" );

    return true;
}


Forest* Forest::create()
{
    Forest* scene = new Forest();
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

void  Forest::AllInitialize_ore() {

    for (auto it = Tree_information.begin(); it != Tree_information.end(); /* no increment here */) {

        auto tree = *it;  // �����õ������Է��� Crop ����

      
        tree->recover_time = 2;

        if (!tree->available) {
            if (season[Season] * 7 + day - tree->mining_day >= tree->recover_time) {
                tree->available = true;
                tree->removetimes = 2;
            }
        }
        

        if (tree->available) {

            if (Season == "Autumn" || Season == "Winter") {
                auto temp = Sprite::create(tree->Y_Ini_pic);
                this->addChild(temp, 5);
                temp->setPosition(tree->position);
                temp->setScale(3.1f);
            }
            else {
                auto temp = Sprite::create(tree->initial_pic);
                this->addChild(temp, 5);
                temp->setPosition(tree->position);
                temp->setScale(3.1f);
            }
        }
        else {
            auto temp = Sprite::create(tree->G_Cut_pic);
            this->addChild(temp, 5);
            temp->setPosition(tree->position);
            temp->setScale(3.1f);
        }


        it++;


    }

}

// �������Ƿ�ӽ�������������
void  Forest::checkPlayerPosition()
{

    // ��ȡ��ҵ�λ��
    Vec2 playerPos = player1->getPosition();
    
    // ���¼�ʱ����ʾ
    remainingTime++;
    if (remainingTime == 43200 || strength == 0) {

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
        auto nextday = Myhouse::create();
        Director::getInstance()->replaceScene(nextday);

    }

    // ���±�ǩλ��
    float currentx = 0, currenty = 0;
    if (playerPos.x <= -600) {
        currentx = -600;
    }
    else if (playerPos.x >= 2197) {
        currentx = 2197;
    }
    else {
        currentx = playerPos.x;  
    }

    if (playerPos.y >= 2200) {
        currenty = 2200;
    }
    else if (playerPos.y <= -850) {
        currenty = -850;
    }
    else {
        currenty = playerPos.y;
    }

    TimeUI->setPosition(currentx, currenty);
    button->setPosition(currentx + 730, currenty - 590);
    miniBag->setPosition ( currentx , currenty );
    if (Weather == "Rainy") {
        emitter->setPositionY ( currenty + 350 );
    }
    if (isLKeyPressed) {

        isLKeyPressed = false;

        for (auto it = Tree_information.begin(); it != Tree_information.end(); ) {

            auto tree = *it;  

            float distance = tree->position.distance(playerPos);
            if ((distance <= 250 && tree->available) && strength >= 10) {

                strength -= 10;
                TimeUI->UpdateEnergy();

                if (skill_tree->GetSkillLevels()[foraging_skill] >= 5) {
                    tree->removetimes -= 2;
                }
                else {
                    tree->removetimes -= 1;
                }

                if (tree->removetimes <= 0) {
                   
                    inventory->AddItem(Wood);

                    // ����
                    skill_tree->AddExperience(foraging_skill, 10);


                    cocos2d::log("Cut Down");

                    tree->available = false;

                    tree->mining_day = season[Season] * 7 + day;


                    if (player1->pic_path == "character1/player_right3.png") {
                        player1->setTexture("character1/player_plant3.png");
                        player1->setScale(3.5f);

                        // �ӳ�0.3����л����ڶ���ͼƬ
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_plant4.png");  // ����Ϊplayer_plant2
                            player1->setScale(3.7f);
                            }, 0.15f, "change_image1_key");

                        // �ӳ�0.6����л���������ͼƬ
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_right3.png"); // ����Ϊplayer_left3
                            player1->setScale(2.3f);
                            auto temp = Sprite::create(tree->G_Cut_pic);
                            this->addChild(temp, 5);
                            temp->setPosition(tree->position);
                            temp->setScale(3.1f);
                            }, 0.35f, "change_image2_key");
                    }
                    else {
                        player1->setTexture("character1/player_plant1.png");
                        player1->setScale(3.5f);

                        // �ӳ�0.3����л����ڶ���ͼƬ
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_plant2.png");  // ����Ϊplayer_plant2
                            player1->setScale(3.7f);
                            }, 0.15f, "change_image1_key");

                        // �ӳ�0.6����л���������ͼƬ
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_left3.png"); // ����Ϊplayer_left3
                            player1->setScale(2.3f);
                            auto temp = Sprite::create(tree->G_Cut_pic);
                            this->addChild(temp, 5);
                            temp->setPosition(tree->position);
                            temp->setScale(3.1f);
                            }, 0.35f, "change_image2_key");
                    }

                }
                else {
                    if (player1->pic_path == "character1/player_right3.png") {
                        player1->setTexture("character1/player_plant3.png");
                        player1->setScale(3.5f);

                        // �ӳ�0.3����л����ڶ���ͼƬ
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_plant4.png");  // ����Ϊplayer_plant2
                            player1->setScale(3.7f);
                            }, 0.15f, "change_image1_key");

                        // �ӳ�0.6����л���������ͼƬ
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_right3.png"); // ����Ϊplayer_left3
                            player1->setScale(2.3f);
                            }, 0.35f, "change_image2_key");
                    }
                    else {
                        player1->setTexture("character1/player_plant1.png");
                        player1->setScale(3.5f);

                        // �ӳ�0.3����л����ڶ���ͼƬ
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_plant2.png");  // ����Ϊplayer_plant2
                            player1->setScale(3.7f);
                            }, 0.15f, "change_image1_key");

                        // �ӳ�0.6����л���������ͼƬ
                        player1->scheduleOnce([=](float dt) {
                            player1->setTexture("character1/player_left3.png"); // ����Ϊplayer_left3
                            player1->setScale(2.3f);
                            }, 0.35f, "change_image2_key");
                    }
                }
            }


            it++;

        }
    }

    // �Ƿ�ǰ��ũ��
    if (farm_area.containsPoint(playerPos)) {
        if (isEnterKeyPressed) {
            for (auto& pair : W_lastplace) {
                if (pair.first.first == "farm") {  // ��� bool ֵ�Ƿ�Ϊ true
                    pair.second = true;
                }
            }
            // 使用GameStateManager切换到农场状态
            GameStateManager::getInstance()->changeState(GameStateType::FARM);
        }
    }

    // �Ƿ�ǰ��С��
    if (town_area.containsPoint(playerPos)) {
        if (isEnterKeyPressed) {
            for (auto& pair : W_lastplace) {
                if (pair.first.first == "town") {  // ��� bool ֵ�Ƿ�Ϊ true
                    pair.second = true;
                }
            }
            // 使用GameStateManager切换到小镇状态
            GameStateManager::getInstance()->changeState(GameStateType::TOWN);
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
        else{
            if (player1->rightpressed == false) {
                player1->moveRight = true;
            }
        }
       
    }
    

}

int Forest::getRegionNumber(Vec2 pos) {

    int region_number = 1;

    return region_number;
}

void Forest::createRainEffect() {

    emitter = ParticleRain::create();
    emitter->setDuration(ParticleSystem::DURATION_INFINITY);
    emitter->setScale(5.7f);
    emitter->setTotalParticles(100);
    emitter->setSpeed(250);

    addChild(emitter, 10);

    // ÿ֡����������������
    schedule([this](float dt) {
        updaterain(dt);
        }, "update_rain_key");

}

void Forest::updaterain(float deltaTime) {
    if (emitter) {
        // �������һ���������ڣ���Χ 1 �� 1.5 ��֮�䣩
        float newLife = cocos2d::rand_0_1() * 1.5f;

        // �����µ���������
        emitter->setLife(newLife);

        emitter->setEmissionRate(emitter->getTotalParticles() / emitter->getLife() * 1.3);
    }
}
