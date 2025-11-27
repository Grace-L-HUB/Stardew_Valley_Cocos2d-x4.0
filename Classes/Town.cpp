#include "AppDelegate.h"
#include "Town.h"
#include "Crop.h"
#include "Beach.h"
#include "supermarket.h"
#include "Player.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Town::Town() {}

Town::~Town() {}

bool Town::init()
{
    AudioEngine::pauseAll ();
    // auto backgroundAudioID = AudioEngine::play2d ( "MUSIC/Mayten.mp3" , true );

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    button = cocos2d::Sprite::create("CloseNormal.png");
    this->addChild(button, 11);


    // ���ü�ʱ����ǩ
    TimeUI = Timesystem::create("Town");
    
    this->addChild(TimeUI, 13);


    if (Weather == "Rainy") {
        // ����
        createRainEffect();
    }

    // ���ñ���ͼƬ
    auto background_real = Sprite::create("Town/Town.png");
    background_real->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background_real, 1);
    background_real->setScale(1.7f);

    auto background_up = Sprite::create("Town/Town_up.png");
    background_up->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background_up, 7);
    background_up->setScale(1.7f);

    auto background = Sprite::create("Town/Town_path.png");
    this->addChild(background, 5);
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(1.7f);


    Vec2 spritePosition = background->getPosition();   // ��ȡ�����λ�ã����ĵ㣩
    Size spriteSize = background->getContentSize();    // ��ȡ����ĳߴ磨���Ⱥ͸߶ȣ�


    // �������½ǵ�����
    Vec2 leftBottomPosition = Vec2(
        spritePosition.x - background->getScaleX() * spriteSize.width / 2,   // ���ĵ� x �����ȥ���ȵ�һ��
        spritePosition.y - background->getScaleY() * spriteSize.height / 2   // ���ĵ� y �����ȥ�߶ȵ�һ��
    );


    Image img;
    if (img.initWithImageFile("Town/Town_path.png"))
    {
        int width = img.getWidth();
        int height = img.getHeight();

        // ��ȡ��������
        unsigned char* data = img.getData();

        // �����������أ�����Ƿ������ݣ�͸���ȴ���0��
        for (int y = 0; y < height; y = y + 8)
        {
            for (int x = 0; x < width; x = x + 8)
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


    // ��ʼ�����ż�
    opendoor = Sprite::create("opendoor.png");
    this->addChild(opendoor, 11);

    // �ָ���ҵ�״̬
    if (player1)
    {
        for (auto& pair : T_lastplace) {
            if (pair.second == true) {  // ��� bool ֵ�Ƿ�Ϊ true
                player1->setPosition(pair.first.second);
                pair.second = false;
            }
        }
        player1->speed = 3.7f;
        //��Ϊ�������
       /* player1->speed = 20.0f;*/
    }

    //�����µı�����ʾ
    miniBag = mini_bag::create(inventory);
    miniBag->setScale(1.0f);
    Vec2 pos = miniBag->getPosition();
    if (miniBag != NULL) {
        cocos2d::log("miniBagtest %f", pos.x);
    }
    if (!this->getChildByName("mini_bag")) {
        this->addChild(miniBag, 10, "mini_bag");
    }


    // ������Ʒ��
    schedule([=](float deltaTime) {
        if (inventory->is_updated == true) {
            miniBag->updateDisplay();
            inventory->is_updated = false;
        }
        }, 0.1f, "item_update_key");

    //�������ӣ������������
    Box = Sprite::create ( "UIresource/xiangzi/xiangzi.png" );
    CCLOG ( "boxhavecreate" );
    Box->setPosition ( Vec2 ( -260 , 710 ) );
    Box->setAnchorPoint ( Vec2 ( 0 , 0 ) );
    Box->setScale ( 0.7f );
    this->addChild ( Box , 11 );

    // ��ʼ����ɫ���������ӵ�����
    if (player1->getParent() == NULL) {
        cocos2d::log("player1->get");
        this->addChild(player1, 6);
    }
    player1->setScale(1.5f);
    player1->setAnchorPoint(Vec2(0.5f, 0.2f));
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

    if (Festival == "Fishing Festival") {}
    else {
        // �����Ľ����뾶  
        const float interactionRadius = 300.0f;


        // ʹ�� getAlexAnimations() ��ȡ NPC ����֡  
        auto alexAnimations = getAlexAnimations ( Season );
        // ���� NPC ʾ��  
        auto alex = NPCreate::CreateNPC ( "Alex" , cocos2d::Vec2 ( -550 , 1450 ) , alexAnimations , nonTransparentPixels );
        if (alex) {
            // CCLOG ( "NPC Alex created successfully." );  
            auto alexSprite = alex->GetSprite ();
            if (alexSprite) {
                // CCLOG ( "Alex sprite created successfully at position: (%f, %f)" , alexSprite->getPositionX () , alexSprite->getPositionY () );  
                this->addChild ( alexSprite , 5 ); // ȷ�����ӵ�������  

                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule ( [alex]( float dt ) {
                    alex->RandomMove ();

                    // ��ȡ Alex �ĵ�ǰλ��  
                    auto alexSprite = alex->GetSprite (); // ��ȡ����  
                    if (alexSprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = alexSprite->getPosition ();
                        Size size = alexSprite->getContentSize ();
                        // CCLOG ( "Alex's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    } , 1.0f , "random_move_key_alex" ); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG ( "Alex sprite is nullptr." );
            }
        }
        else {
            CCLOG ( "Failed to create NPC Alex." );
        }

        // ʹ�� getElliottAnimations() ��ȡ NPC ����֡  
        auto elliottAnimations = getElliottAnimations ( Season );
        // ���� NPC ʾ��  
        auto elliott = NPCreate::CreateNPC ( "Elliott" , cocos2d::Vec2 ( 1550 , 600 ) , elliottAnimations , nonTransparentPixels );
        if (elliott) {
            // CCLOG ( "NPC Elliott created successfully." );
            auto elliottSprite = elliott->GetSprite ();
            if (elliottSprite) {
                // CCLOG ( "Elliott sprite created successfully at position: (%f, %f)" , elliottSprite->getPositionX () , elliottSprite->getPositionY () );
                this->addChild ( elliottSprite , 5 ); // ȷ�����ӵ�������  

                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule ( [elliott]( float dt ) {
                    elliott->RandomMove ();

                    // ��ȡ Elliott �ĵ�ǰλ��  
                    auto elliottSprite = elliott->GetSprite (); // ��ȡ����  
                    if (elliottSprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = elliottSprite->getPosition ();
                        Size size = elliottSprite->getContentSize ();
                        // CCLOG ( "Elliott's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    } , 1.0f , "random_move_key_elliott" ); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG ( "Elliott sprite is nullptr." );
            }
        }
        else {
            CCLOG ( "Failed to create NPC Elliott." );
        }

        // ʹ�� getCarolineAnimations() ��ȡ NPC ����֡  
        auto carolineAnimations = getCarolineAnimations ( Season );
        // ���� NPC ʾ��  
        auto caroline = NPCreate::CreateNPC ( "Caroline" , cocos2d::Vec2 ( 700 , 1400 ) , carolineAnimations , nonTransparentPixels );
        if (caroline) {
            // CCLOG ( "NPC Caroline created successfully." );  
            auto carolineSprite = caroline->GetSprite ();
            if (carolineSprite) {
                // CCLOG ( "Caroline sprite created successfully at position: (%f, %f)" , carolineSprite->getPositionX () , carolineSprite->getPositionY () );  
                this->addChild ( carolineSprite , 5 ); // ȷ�����ӵ�������  

                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule ( [caroline]( float dt ) {
                    caroline->RandomMove ();

                    // ��ȡ Caroline �ĵ�ǰλ��  
                    auto carolineSprite = caroline->GetSprite (); // ��ȡ����  
                    if (carolineSprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = carolineSprite->getPosition ();
                        Size size = carolineSprite->getContentSize ();
                        // CCLOG ( "Caroline's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    } , 1.0f , "random_move_key_caroline" ); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG ( "Caroline sprite is nullptr." );
            }
        }
        else {
            CCLOG ( "Failed to create NPC Caroline." );
        }

        // ʹ�� getAbigailAnimations() ��ȡ NPC ����֡  
        auto abigailAnimations = getAbigailAnimations ( Season );
        // ���� NPC ʾ��  
        auto abigail = NPCreate::CreateNPC ( "Abigail" , cocos2d::Vec2 ( 60 , 100 ) , abigailAnimations , nonTransparentPixels );
        if (abigail) {
            // CCLOG ( "NPC Abigail created successfully." );  
            auto abigailSprite = abigail->GetSprite ();
            if (abigailSprite) {
                // CCLOG ( "Abigail sprite created successfully at position: (%f, %f)" , abigailSprite->getPositionX () , abigailSprite->getPositionY () );  
                this->addChild ( abigailSprite , 5 ); // ȷ�����ӵ�������  

                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule ( [abigail]( float dt ) {
                    abigail->RandomMove ();

                    // ��ȡ Abigail �ĵ�ǰλ��  
                    auto abigailSprite = abigail->GetSprite (); // ��ȡ����  
                    if (abigailSprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = abigailSprite->getPosition ();
                        Size size = abigailSprite->getContentSize ();
                        // CCLOG ( "Abigail's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    } , 1.0f , "random_move_key_abigail" ); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG ( "Abigail sprite is nullptr." );
            }
        }
        else {
            CCLOG ( "Failed to create NPC Abigail." );
        }

        // ʹ�� getEmilyAnimations() ��ȡ NPC ����֡  
        auto emilyAnimations = getEmilyAnimations ( Season );
        // ���� NPC ʾ��  
        auto emily = NPCreate::CreateNPC ( "Emily" , cocos2d::Vec2 ( 180 , 1550 ) , emilyAnimations , nonTransparentPixels );
        if (emily) {
            // CCLOG ( "NPC Emily created successfully." );  
            auto emilySprite = emily->GetSprite ();
            if (emilySprite) {
                // CCLOG ( "Emily sprite created successfully at position: (%f, %f)" , emilySprite->getPositionX () , emilySprite->getPositionY () );  
                this->addChild ( emilySprite , 5 ); // ȷ�����ӵ�������  

                // ʹ�õ�����ÿ 1.0 ����� RandomMove  
                this->schedule ( [emily]( float dt ) {
                    emily->RandomMove ();

                    // ��ȡ Emily �ĵ�ǰλ��  
                    auto emilySprite = emily->GetSprite (); // ��ȡ����  
                    if (emilySprite) {
                        // ��ȡ��ǰ�����λ�úʹ�С  
                        Vec2 position = emilySprite->getPosition ();
                        Size size = emilySprite->getContentSize ();
                        // CCLOG ( "Emily's current position: (%f, %f)" , position.x , position.y ); // ��ӡλ��  
                    }
                    } , 1.0f , "random_move_key_emily" ); // ÿ 1.0 ������ƶ�һ��  
            }
            else {
                CCLOG ( "Emily sprite is nullptr." );
            }
        }
        else {
            CCLOG ( "Failed to create NPC Abigail." );
        }

        // ����¼�������
        auto listener = EventListenerMouse::create ();
        listener->onMouseDown = [this , abigail , alex , caroline , elliott , emily , interactionRadius]( Event* event ) {

            // ��ȡ�������λ��
            auto mouseEvent = static_cast<EventMouse*>(event);
            Vec2 clickPos ( mouseEvent->getCursorX () , mouseEvent->getCursorY () );
            clickPos = this->convertToNodeSpace ( clickPos );

            if (Box->getBoundingBox ().containsPoint ( clickPos )) {              
                // ��ȡ��ҵ�λ��  
                Vec2 playerPos = player1->getPosition ();

                Vec2 BoxPos = Box->getPosition ();

                // ���������NPC֮��ľ���  
                float distance = playerPos.distance ( BoxPos );

                // �������Ƿ��������ķ�Χ��  
                if (distance <= interactionRadius) {
                    if (miniBag->getSelectedSlot ()) {
                        std::string taskName = taskManager->findTaskByRequiredItems ( inventory->GetItemAt ( miniBag->getSelectedSlot () )->GetName () );
                        // CCLOG ( "%s" , inventory->GetItemAt ( miniBag->getSelectedSlot () )->GetName () );
                        if (taskName.empty ()) {
                            // ��������δ�ҵ������
                        }
                        else {
                            // CCLOG ( "CLICK ON BOX" );
                            taskManager->completeTask ( taskName );
                        }
                    }
                }
            }

            // ����Ƿ�����NPC���򿪶Ի���  
            std::vector<std::pair<NPC* , std::string>> npcs = {
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
                    auto npcSprite = npc->GetSprite ();
                    if (npcSprite && npcSprite->getBoundingBox ().containsPoint ( clickPos )) {
                        // ��ȡ��ҵ�λ��  
                        Vec2 playerPos = player1->getPosition ();

                        // ���������NPC֮��ľ���  
                        float distance = playerPos.distance ( npcSprite->getPosition () );

                        // �������Ƿ��������ķ�Χ��  
                        if (distance <= interactionRadius) {
                            if (!miniBag->getSelectedSlot () || npc_relationship->NpcGiftTIme ( npc->GetName () ) == 2) {
                                // �򿪶Ի���ȷ���Ի���ÿ�ζ����µ�ʵ��  
                                NPCtalkUI* currentNPCtalkUI = NPCtalkUI::create ( npc , "Town" );
                                this->addChild ( currentNPCtalkUI , 12 ); // ����ǰNPC�Ի������ӵ�������  
                                return; // ������һ��NPC��ֱ�ӷ���  
                            }
                            else if (miniBag->getSelectedSlot ()) {
                                Vec2 playerPos = player1->getPosition ();
                                npc_relationship->increaseRelationship ( "player" , npc->GetName () , 15.2 );
                                inventory->RemoveItem ( miniBag->getSelectedSlot () );
                                inventory->is_updated = true;
                                npc_relationship->AddGiftTime ( npc->GetName () );
                                // ����ĳ������ͼ
                                auto ItemClickByminiBag = Sprite::create ( "npc/gift.png" );
                                ItemClickByminiBag->setPosition ( playerPos );
                                this->addChild ( ItemClickByminiBag , 6 );
                                // һ���ӳ٣�2����Ƴ� ItemClickByminiBag  
                                this->runAction ( Sequence::create (
                                    DelayTime::create ( 1.5f ) , // �ȴ�2��  
                                    CallFunc::create ( [=]() {
                                        this->removeChild ( ItemClickByminiBag ); // �Ƴ� ItemClickByminiBag  
                                        } ) ,
                                    nullptr
                                ) );
                                miniBag->getSelectBack ();
                            }
                        }
                        else {
                            CCLOG ( "Player is too far from %s to interact." , npcName.c_str () );
                        }
                    }
                }
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , this );
    }

    // ���ü��̼�����
    auto listenerWithPlayer = EventListenerKeyboard::create();
    listenerWithPlayer->onKeyPressed = [this]( EventKeyboard::KeyCode keyCode , Event* event )
        {
            // ��¼ Enter ��������
            if (keyCode == EventKeyboard::KeyCode::KEY_ENTER || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                isEnterKeyPressed = true;

                CCLOG ( "Enter key pressed. " );
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                CCLOG ( "%d" , GoldAmount );
                static int isOpen = 0;
                static InventoryUI* currentInventoryUI = nullptr;  // ���浱ǰ��ʾ�� InventoryUI  
                // �����ǰû�д� InventoryUI�������  
                if (currentInventoryUI == nullptr || isOpen == 0) {
                    isOpen = 1;
                    CCLOG ( "Opening inventory." );
                    currentInventoryUI = InventoryUI::create ( inventory , "Town" );
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
            if (keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                isEnterKeyPressed = false;
                CCLOG("Enter key released. ");
            }
        };

    // �����������ӵ��¼��ַ�����
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerWithPlayer, this);

    return true;
}

Town* Town::create()
{
    Town* scene = new Town();
    if (scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

cocos2d::Scene* Town::createScene()
{
    auto scene = cocos2d::Scene::create();
    auto layer = Town::create();
    if (layer)
    {
        layer->setName("town");
        scene->addChild(layer);
    }
    return scene;
}

// �������Ƿ�ӽ�������������
void Town::checkPlayerPosition ()
{

    // ��ȡ��ҵ�λ��
    Vec2 playerPos = player1->getPosition ();


    // �������������֮��ľ���  
    float distance = playerPos.distance(Box->getPosition());
    // �������Ƿ��������ķ�Χ��  
    if (distance <= 200.0f) {
        Box->setTexture("UIresource/xiangzi/Open.png");
    }
    else {
        Box->setTexture("UIresource/xiangzi/xiangzi.png");
    }

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
        player1->removeFromParent ();
        auto nextday = Myhouse::create ();
        Director::getInstance ()->replaceScene ( nextday );

    }

    // ���±�ǩλ��
    float currentx = 0 , currenty = 0;
    if (playerPos.x <= -170) {
        currentx = -170;
    }
    else if (playerPos.x >= 1773) {
        currentx = 1773;
    }
    else {
        currentx = playerPos.x;
    }

    if (playerPos.y >= 1498) {
        currenty = 1498;
    }
    else if (playerPos.y <= -222) {
        currenty = -222;
    }
    else {
        currenty = playerPos.y;
    }

    TimeUI->setPosition ( currentx , currenty );
    button->setPosition ( currentx + 730 , currenty - 590 );
    miniBag->setPosition ( currentx , currenty );
    if (Weather == "Rainy") {
        emitter->setPositionY ( currenty + 350 );
    }
    // �������Ƿ����Ŀ�����򣬲��Ұ��� Enter ��
    if (Region_supermarket.containsPoint ( playerPos )) {
        // ��ҽ���Ŀ������
        opendoor->setVisible ( true );
        opendoor->setPosition ( playerPos.x + 110 , playerPos.y + 30 );
        CCLOG ( "Player in target area" );

        if (isEnterKeyPressed) {
            for (auto& pair : T_lastplace) {
                if (pair.first.first == "seedshop") {  // ��� bool ֵ�Ƿ�Ϊ true
                    pair.second = true;
                }
            }
            // ��ӡ������Ϣ����� Enter ����״̬
            CCLOG ( "Player in target area, isEnterKeyPressed: %d" , isEnterKeyPressed );
            // ���ó����л��߼�
            player1->removeFromParent ();
            AudioEngine::pauseAll ();
            auto backgroundAudioID = AudioEngine::play2d ( "MUSIC/MayYouBeHappyAndProsperous.mp3" , true );
            auto seedshop = supermarket::create ();
            // miniBag->removeFromParent();
            Director::getInstance ()->replaceScene ( seedshop );
        }

    }
    else {
        opendoor->setVisible ( false );
    }

    if (Region_forest.containsPoint ( playerPos )) {
        if (isEnterKeyPressed) {
            for (auto& pair : T_lastplace) {
                if (pair.first.first == "forest") {
                    pair.second = true;
                }
            }
            // ���ó����л��߼�
            // 使用GameStateManager切换到森林状态
            GameStateManager::getInstance()->changeState(GameStateType::FOREST);
        }

    }

    if (Region_beach.containsPoint ( playerPos )) {
        if (isEnterKeyPressed) {
            for (auto& pair : T_lastplace) {
                if (pair.first.first == "beach") {
                    pair.second = true;
                }
            }
            // ���ó����л��߼�
            // 使用GameStateManager切换到海滩状态
            GameStateManager::getInstance()->changeState(GameStateType::BEACH);
        }
    }

    for (const auto& point : nonTransparentPixels)
    {
        // ���������������֮��ľ���
        float distance = 0;

        Vec2 temp;
        temp = playerPos;
        temp.x -= player1->speed;
        distance = temp.distance ( point );
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

void Town::createRainEffect() {

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

void Town::updaterain(float deltaTime) {
    if (emitter) {
        // �������һ���������ڣ���Χ 1 �� 1.5 ��֮�䣩
        float newLife = cocos2d::rand_0_1() * 1.5f;

        // �����µ���������
        emitter->setLife(newLife);

        emitter->setEmissionRate(emitter->getTotalParticles() / emitter->getLife() * 1.3);
    }
}
