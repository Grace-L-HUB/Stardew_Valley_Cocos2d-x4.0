#include "AppDelegate.h"
#include "Beach.h"
#include "Player.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "GameStateManager.h"


USING_NS_CC;

Beach::Beach () {}

Beach::~Beach () {}

bool Beach::init ()
{
    inventory->AddItem ( Fiberglass_Rod );
    inventory->AddItem ( Fiberglass_Rod );

    AudioEngine::pauseAll ();
    auto backgroundAudioID = AudioEngine::play2d ( "MUSIC/Mayten.mp3" , true );

    //��ȡ��Ļ��Χ������
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    Vec2 origin = Director::getInstance ()->getVisibleOrigin ();

    //�����˳���ť
    button = cocos2d::Sprite::create ( "CloseNormal.png" );
    this->addChild ( button , 11 );

    // ���ü�ʱ����ǩ
   // ���ü�ʱ����ǩ
    TimeUI = Timesystem::create ( "Beach" );
    this->addChild ( TimeUI , 13 );

    if (Weather == "Rainy") {
        // ����
        createRainEffect ();
    }


    // ���ñ���ͼƬ
    auto background_real = Sprite::create ( "Beach/Beach.png" );
    background_real->setPosition ( Vec2 ( visibleSize.width / 2 , visibleSize.height / 2 ) );
    this->addChild ( background_real , 1 );
    background_real->setScale ( 2.3f );

    auto immovable_background = Sprite::create ( "Beach/Beach_path.png" );
    this->addChild ( immovable_background , 5 );
    immovable_background->setPosition ( Vec2 ( visibleSize.width / 2 , visibleSize.height / 2 ) );
    immovable_background->setScale ( 2.3f );


    Vec2 spritePosition = immovable_background->getPosition ();   // ��ȡ�����λ�ã����ĵ㣩
    Size spriteSize = immovable_background->getContentSize ();    // ��ȡ����ĳߴ磨���Ⱥ͸߶ȣ�


    // �������½ǵ�����
    Vec2 leftBottomPosition = Vec2 (
        spritePosition.x - immovable_background->getScaleX () * spriteSize.width / 2 ,   // ���ĵ� x �����ȥ���ȵ�һ��
        spritePosition.y - immovable_background->getScaleY () * spriteSize.height / 2   // ���ĵ� y �����ȥ�߶ȵ�һ��
    );


    Image img;
    if (img.initWithImageFile ( "Beach/Beach_path.png" ))
    {
        int width = img.getWidth ();
        int height = img.getHeight ();

        // ��ȡ��������
        unsigned char* data = img.getData ();

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
                    float screenX = leftBottomPosition.x + x * immovable_background->getScaleX ();
                    float screenY = leftBottomPosition.y + (height - y - 1) * immovable_background->getScaleY ();  // ע�� Y �ᷴ��
                    non_transparent_pixels.push_back ( Vec2 ( screenX , screenY ) );  // ��¼��Ļ����
                }
            }
        }
    }


    // ��ʼ����ɫ���������ӵ�����
    if (player1->getParent () == NULL) {
        this->addChild ( player1 , 11 );
        player1->setScale ( 1.6f );
        player1->setPosition ( 320 , 1400 );
        player1->setAnchorPoint ( Vec2 ( 0.5f , 0.2f ) );
        

    }

    // ��������Ķ�ʱ��
    player1->schedule ( [=]( float dt ) {
        player1->player1_move ();
        } , 0.05f , "player1_move" );

    player1->schedule ( [=]( float dt ) {
        player1->player_change ();
        } , 0.3f , "player_change" );

    player1->speed = 3.1f;

    // ���㱳����������ź�Χ
    float scaledWidth = immovable_background->getContentSize ().width * immovable_background->getScaleX ();
    float scaledHeight = immovable_background->getContentSize ().height * immovable_background->getScaleY ();

    // ���� Follow �ı߽� Rect
    auto followRect = cocos2d::Rect ( leftBottomPosition.x , leftBottomPosition.y , scaledWidth , scaledHeight );

    // ���� Follow ��������������ڱ�����Χ���ƶ�
    auto followAction = Follow::create ( player1 , followRect );
    this->runAction ( followAction );

    // ���ڸ������״̬
    this->schedule ( [this]( float dt ) {
        this->CheckPlayerPosition ();  // �������Ƿ�ӽ�������
        } , 0.01f , "check_position_key" );

    auto listener = EventListenerMouse::create ();
    listener->onMouseDown = [this]( Event* event ) {

        // ��ȡ�������λ��
        auto mouseEvent = static_cast<EventMouse*>(event);
        Vec2 clickPos ( mouseEvent->getCursorX () , mouseEvent->getCursorY () );
        clickPos = this->convertToNodeSpace ( clickPos );

        // �жϵ��λ���Ƿ��ھ��鷶Χ��
        if (button != nullptr && button->getBoundingBox ().containsPoint ( clickPos )) {
            Director::getInstance ()->end ();
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , button );

    // ���ü��̼�����
    auto listenerWithPlayer = EventListenerKeyboard::create ();

    listenerWithPlayer->onKeyPressed = [this]( EventKeyboard::KeyCode key_code , Event* event ) {
        if (key_code == EventKeyboard::KeyCode::KEY_ENTER || key_code == EventKeyboard::KeyCode::KEY_KP_ENTER) {
            isEnterKeyPressed = true;
            CCLOG ( "Enter key pressed. " );
        }
        else if (key_code == EventKeyboard::KeyCode::KEY_H) {
            if (!this->getChildByName ( "FishingGameLayer" ) && strength >= 10) {
                strength -= 10;
                TimeUI->UpdateEnergy();
                //��������Ϸ������볡����
                auto fishing_game = FishingGame::create ( player1->getPosition () );
                this->addChild ( fishing_game , 10 , "FishingGameLayer" );
                ////��ͣ�����������ڵ�Ļ
                //player1->pause ();
                //this->pause ();
            }
        }
        else if (key_code == EventKeyboard::KeyCode::KEY_ESCAPE) {
            static int isOpen = 0;
            static InventoryUI* currentInventoryUI = nullptr;  // ���浱ǰ��ʾ�� InventoryUI  
            // �����ǰû�д� InventoryUI�������  
            if (currentInventoryUI == nullptr || isOpen == 0) {
                isOpen = 1;
                CCLOG ( "Opening inventory." );
                currentInventoryUI = InventoryUI::create ( inventory , "Beach" );
                this->addChild ( currentInventoryUI , 30 );  // �� InventoryUI ���ӵ��ϲ�  
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

    listenerWithPlayer->onKeyReleased = [this]( EventKeyboard::KeyCode keyCode , Event* event )
        {
            // �ͷ� Enter ��ʱ������Ϊ false
            if (keyCode == EventKeyboard::KeyCode::KEY_ENTER || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                isEnterKeyPressed = false;
            }
        };

 

    // �����������ӵ��¼��ַ�����
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listenerWithPlayer , this );


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

    if (Festival == "Fishing Festival") {
        string cloth = Season;
        cloth = "Beach";
        // �����Ľ����뾶  
        const float interactionRadius = 300.0f;
        // ʹ�� getAlexAnimations() ��ȡ NPC ����֡  
        auto alexAnimations = getAlexAnimations(cloth);
        // ���� NPC ʾ��  
        auto alex = NPCreate::CreateNPC("Alex", cocos2d::Vec2(380, 730), alexAnimations, non_transparent_pixels);
        if (alex) {
            // CCLOG ( "NPC Alex created successfully." );  
            auto alexSprite = alex->GetSprite();
            if (alexSprite) {
                // CCLOG ( "Alex sprite created successfully at position: (%f, %f)" , alexSprite->getPositionX () , alexSprite->getPositionY () );  
                this->addChild(alexSprite, 5); // ȷ�����ӵ�������  
                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule([alex](float dt) {
                    alex->RandomMove();
                    // ��ȡ Alex �ĵ�ǰλ��  
                    auto alexSprite = alex->GetSprite(); // ��ȡ����  
                    if (alexSprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = alexSprite->getPosition();
                        Size size = alexSprite->getContentSize();
                        // CCLOG ( "Alex's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    }, 1.0f, "random_move_key_alex"); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG("Alex sprite is nullptr.");
            }
        }
        else {
            CCLOG("Failed to create NPC Alex.");
        }
        // ʹ�� getElliottAnimations() ��ȡ NPC ����֡  
        auto elliottAnimations = getElliottAnimations(cloth);
        // ���� NPC ʾ��  
        auto elliott = NPCreate::CreateNPC("Elliott", cocos2d::Vec2(-560, 1100), elliottAnimations, non_transparent_pixels);
        if (elliott) {
            // CCLOG ( "NPC Elliott created successfully." );
            auto elliottSprite = elliott->GetSprite();
            if (elliottSprite) {
                // CCLOG ( "Elliott sprite created successfully at position: (%f, %f)" , elliottSprite->getPositionX () , elliottSprite->getPositionY () );
                this->addChild(elliottSprite, 5); // ȷ�����ӵ�������  
                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule([elliott](float dt) {
                    elliott->RandomMove();
                    // ��ȡ Elliott �ĵ�ǰλ��  
                    auto elliottSprite = elliott->GetSprite(); // ��ȡ����  
                    if (elliottSprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = elliottSprite->getPosition();
                        Size size = elliottSprite->getContentSize();
                        // CCLOG ( "Elliott's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    }, 1.0f, "random_move_key_elliott"); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG("Elliott sprite is nullptr.");
            }
        }
        else {
            CCLOG("Failed to create NPC Elliott.");
        }
        // ʹ�� getCarolineAnimations() ��ȡ NPC ����֡  
        auto carolineAnimations = getCarolineAnimations(cloth);
        // ���� NPC ʾ��  
        auto caroline = NPCreate::CreateNPC("Caroline", cocos2d::Vec2(800, 880), carolineAnimations, non_transparent_pixels);
        if (caroline) {
            // CCLOG ( "NPC Caroline created successfully." );  
            auto carolineSprite = caroline->GetSprite();
            if (carolineSprite) {
                // CCLOG ( "Caroline sprite created successfully at position: (%f, %f)" , carolineSprite->getPositionX () , carolineSprite->getPositionY () );  
                this->addChild(carolineSprite, 5); // ȷ�����ӵ�������  
                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule([caroline](float dt) {
                    caroline->RandomMove();
                    // ��ȡ Caroline �ĵ�ǰλ��  
                    auto carolineSprite = caroline->GetSprite(); // ��ȡ����  
                    if (carolineSprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = carolineSprite->getPosition();
                        Size size = carolineSprite->getContentSize();
                        // CCLOG ( "Caroline's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    }, 1.0f, "random_move_key_caroline"); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG("Caroline sprite is nullptr.");
            }
        }
        else {
            CCLOG("Failed to create NPC Caroline.");
        }
        // ʹ�� getAbigailAnimations() ��ȡ NPC ����֡  
        auto abigailAnimations = getAbigailAnimations(cloth);
        // ���� NPC ʾ��  
        auto abigail = NPCreate::CreateNPC("Abigail", cocos2d::Vec2(-220, 1230), abigailAnimations, non_transparent_pixels);
        if (abigail) {
            // CCLOG ( "NPC Abigail created successfully." );  
            auto abigailSprite = abigail->GetSprite();
            if (abigailSprite) {
                // CCLOG ( "Abigail sprite created successfully at position: (%f, %f)" , abigailSprite->getPositionX () , abigailSprite->getPositionY () );  
                this->addChild(abigailSprite, 5); // ȷ�����ӵ�������  
                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule([abigail](float dt) {
                    abigail->RandomMove();
                    // ��ȡ Abigail �ĵ�ǰλ��  
                    auto abigailSprite = abigail->GetSprite(); // ��ȡ����  
                    if (abigailSprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = abigailSprite->getPosition();
                        Size size = abigailSprite->getContentSize();
                        // CCLOG ( "Abigail's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    }, 1.0f, "random_move_key_abigail"); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG("Abigail sprite is nullptr.");
            }
        }
        else {
            CCLOG("Failed to create NPC Abigail.");
        }
        // ʹ�� getEmilyAnimations() ��ȡ NPC ����֡  
        auto emilyAnimations = getEmilyAnimations(cloth);
        // ���� NPC ʾ��  
        auto emily = NPCreate::CreateNPC("Emily", cocos2d::Vec2(350, 1060), emilyAnimations, non_transparent_pixels);
        if (emily) {
            // CCLOG ( "NPC Emily created successfully." );  
            auto emilySprite = emily->GetSprite();
            if (emilySprite) {
                // CCLOG ( "Emily sprite created successfully at position: (%f, %f)" , emilySprite->getPositionX () , emilySprite->getPositionY () );  
                this->addChild(emilySprite, 5); // ȷ�����ӵ�������  
                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule([emily](float dt) {
                    emily->RandomMove();
                    // ��ȡ Emily �ĵ�ǰλ��  
                    auto emilySprite = emily->GetSprite(); // ��ȡ����  
                    if (emilySprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = emilySprite->getPosition();
                        Size size = emilySprite->getContentSize();
                        // CCLOG ( "Emily's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    }, 1.0f, "random_move_key_emily"); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG("Emily sprite is nullptr.");
            }
        }
        else {
            CCLOG("Failed to create NPC Abigail.");
        }
        // ����¼�������
        auto listener = EventListenerMouse::create();
        listener->onMouseDown = [this, abigail, alex, caroline, elliott, emily, interactionRadius](Event* event) {
            // ��ȡ�������λ��  
            auto mouseEvent = static_cast<EventMouse*>(event);
            Vec2 clickPos(mouseEvent->getCursorX(), mouseEvent->getCursorY());
            clickPos = this->convertToNodeSpace(clickPos);
            // ����Ƿ�����NPC���򿪶Ի���  
            std::vector<std::pair<NPC*, std::string>> npcs = {
                {abigail, "Abigail"},
                {alex, "Alex"},
                {caroline, "Caroline"},
                {elliott, "Elliott"},
                {emily, "Emily"}
            };
            for (auto& npcPair : npcs) {
                NPC* npc = npcPair.first;
                const std::string& npcName = npcPair.second;
                if (npc) {
                    auto npcSprite = npc->GetSprite();
                    if (npcSprite && npcSprite->getBoundingBox().containsPoint(clickPos)) {
                        // ��ȡ��ҵ�λ��  
                        Vec2 playerPos = player1->getPosition();
                        // ���������NPC֮��ľ���  
                        float distance = playerPos.distance(npcSprite->getPosition());
                        // �������Ƿ��������ķ�Χ��  
                        if (distance <= interactionRadius) {
                            if (!miniBag->getSelectedSlot() || npc_relationship->NpcGiftTIme(npc->GetName()) == 2) {
                                // �򿪶Ի���ȷ���Ի���ÿ�ζ����µ�ʵ��  
                                NPCtalkUI* currentNPCtalkUI = NPCtalkUI::create(npc, "Beach");
                                this->addChild(currentNPCtalkUI, 12); // ����ǰNPC�Ի������ӵ�������  
                                return; // ������һ��NPC��ֱ�ӷ���  
                            }
                            else if (miniBag->getSelectedSlot()) {
                                Vec2 playerPos = player1->getPosition();
                                npc_relationship->increaseRelationship("player", npc->GetName(), 15.2);
                                inventory->RemoveItem(miniBag->getSelectedSlot());
                                inventory->is_updated = true;
                                npc_relationship->AddGiftTime(npc->GetName());
                                // ����ĳ������ͼ
                                auto ItemClickByminiBag = Sprite::create("npc/gift.png");
                                ItemClickByminiBag->setPosition(playerPos);
                                this->addChild(ItemClickByminiBag, 20);
                                // һ���ӳ٣�2����Ƴ� ItemClickByminiBag  
                                this->runAction(Sequence::create(
                                    DelayTime::create(1.5f), // �ȴ�2��  
                                    CallFunc::create([=]() {
                                        this->removeChild(ItemClickByminiBag); // �Ƴ� ItemClickByminiBag  
                                        }),
                                    nullptr
                                ));
                                miniBag->getSelectBack();
                            }
                        }
                        else {
                            CCLOG("Player is too far from %s to interact.", npcName.c_str());
                        }
                    }
                }
            }
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, button);
    }
    auto fishermanImages = getFisherman("Summer");
    Vec2 initialPosition(410, 220); // ��ʼλ��  
    for (size_t i = 0; i < fishermanImages.size(); ++i) {
        // ��������  
        auto sprite = Sprite::create(fishermanImages[i]);
        if (sprite) {
            sprite->setScale(4.0f);
            // ���þ���λ��  
            sprite->setPosition(initialPosition.x - (i * 100), initialPosition.y); // ÿ������λ������ƫ��50����λ  
            // ���������ӵ�������  
            this->addChild(sprite, 5);
        }
    }



    return true;
}


Beach* Beach::create ()
{
    Beach* scene = new Beach ();
    if (scene && scene->init ())
    {
        scene->autorelease ();
        return scene;
    }
    CC_SAFE_DELETE ( scene );
    return nullptr;
}


// �������Ƿ�ӽ�������������
void Beach::CheckPlayerPosition ()
{

    // ��ȡ��ҵ�λ��
    Vec2 playerPos = player1->getPosition ();

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


        for (auto it = Crop_information.begin (); it != Crop_information.end ();) {

            auto crop = *it;  // �����õ������Է��� Crop ����

            if (day == 1) {
                crop->watered = true;
            }
            if ((day - 1) % 3 == 1) {
                crop->watered = true;
            }

            // �ж�ǰһ���Ƿ�ˮ
            if ((crop->watered == false) && (crop->GetPhase () != Phase::MATURE)) {
                // �ж��Ƿ��Ѿ������ή״̬
                if (crop->GetPhase () != Phase::SAPLESS) {
                    crop->ChangePhase ( Phase::SAPLESS );
                    crop->ChangMatureNeeded ( 2 ); // �ӳ������ջ�
                    it++;
                }
                else {
                    // ɾ��Ԫ�ز����µ�����
                    it = Crop_information.erase ( it );
                }

            }
            else {
                // ����״̬
                crop->UpdateGrowth ();
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
            if (pair.first.first == "farm") {  // ��� bool ֵ�Ƿ�Ϊ true
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
        player1->removeFromParent ();
        GameStateManager::getInstance().changeState(GameStateType::MYHOUSE, "fade", 1.0f);

    }

    // ���±�ǩλ��
    float currentx = 0 , currenty = 0;
    if (playerPos.x <= -315) {
        currentx = -315;
    }
    else if (playerPos.x >= 20000) {
        currentx = 20000;
    }
    else {
        currentx = playerPos.x;
    }

    if (playerPos.y >= 920) {
        currenty = 920;
    }
    else if (playerPos.y <= 360) {
        currenty = 360;
    }
    else {
        currenty = playerPos.y;
    }

    button->setPosition ( currentx + 730 , currenty - 590 );
    miniBag->setPosition ( currentx , currenty );
    TimeUI->setPosition ( currentx , currenty );

    // �Ƿ����С��
    if (Out_Beach.containsPoint ( playerPos )) {
        if (isEnterKeyPressed) {
            player1->removeFromParent ();
            GameStateManager::getInstance().changeState(GameStateType::TOWN, "slide_left", 0.8f);
        }
    }


    for (const auto& point : non_transparent_pixels)
    {
        // ���������������֮��ľ���
        float distance = 0;

        Vec2 temp;
        temp = playerPos;
        temp.x -= player1->speed;
        distance = temp.distance ( point );
        if (distance <= 15) {
            player1->moveLeft = false;
        }
        else {
            if (player1->leftpressed == false) {
                player1->moveLeft = true;
            }
        }

        temp = playerPos;
        temp.y -= 10;
        distance = temp.distance ( point );
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
        distance = temp.distance ( point );
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
        distance = temp.distance ( point );
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

void Beach::createRainEffect () {

    emitter = ParticleRain::create ();
    emitter->setDuration ( ParticleSystem::DURATION_INFINITY );
    emitter->setScale ( 5.7f );
    emitter->setTotalParticles ( 100 );
    emitter->setSpeed ( 250 );

    addChild ( emitter , 10 );

    // ÿ֡����������������
    schedule ( [this]( float dt ) {
        updaterain ( dt );
        } , "update_rain_key" );

}

void Beach::updaterain ( float deltaTime ) {
    if (emitter) {
        // �������һ���������ڣ���Χ 1 �� 1.5 ��֮�䣩
        float newLife = cocos2d::rand_0_1 () * 1.5f;

        // �����µ���������
        emitter->setLife ( newLife );

        emitter->setEmissionRate ( emitter->getTotalParticles () / emitter->getLife () * 1.3 );
    }
}