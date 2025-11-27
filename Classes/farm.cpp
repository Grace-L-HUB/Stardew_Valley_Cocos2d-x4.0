#include "AppDelegate.h"
#include "Myhouse.h"
#include "Barn.h"
#include "farm.h"
#include "Cave.h"
#include "Crop.h"
#include "CookingPot.h"
#include "Player.h"
#include "BasicInformation.h" 
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "vector"
#include "mailBoxUI.h"
#include "GameStateManager.h"

USING_NS_CC;

farm::farm () {}

farm::~farm () {}

bool farm::init ()
{
    AudioEngine::pauseAll ();
    auto backgroundAudioID = AudioEngine::play2d ( "MUSIC/TheShire.mp3" , true );

    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    Vec2 origin = Director::getInstance ()->getVisibleOrigin ();

    button = cocos2d::Sprite::create ( "CloseNormal.png" );
    this->addChild ( button , 11 );

    // ���ü�ʱ����ǩ
    TimeUI = Timesystem::create ( "farm" );
    this->addChild ( TimeUI , 13 );

    // �ָ���ֲ
    AllInitialize_crop ();

    if (Weather == "Rainy") {
        // ����
        createRainEffect ();
    }

    // ���ñ���ͼƬ
    auto background_real = Sprite::create ( "farm/farm.png" );
    background_real->setPosition ( Vec2 ( visibleSize.width / 2 , visibleSize.height / 2 ) );
    this->addChild ( background_real , 1 );
    background_real->setScale ( 1.5f );

    auto background = Sprite::create ( "farm/farm_path.png" );
    this->addChild ( background , 5 );
    background->setPosition ( Vec2 ( visibleSize.width / 2 , visibleSize.height / 2 ) );
    background->setScale ( 1.5f );


    Vec2 spritePosition = background->getPosition ();   // ��ȡ�����λ�ã����ĵ㣩
    Size spriteSize = background->getContentSize ();    // ��ȡ����ĳߴ磨���Ⱥ͸߶ȣ�


    // �������½ǵ�����
    Vec2 leftBottomPosition = Vec2 (
        spritePosition.x - background->getScaleX () * spriteSize.width / 2 ,   // ���ĵ� x �����ȥ���ȵ�һ��
        spritePosition.y - background->getScaleY () * spriteSize.height / 2   // ���ĵ� y �����ȥ�߶ȵ�һ��
    );


    Image img;
    if (img.initWithImageFile ( "farm/farm_path.png" ))
    {
        int width = img.getWidth ();
        int height = img.getHeight ();

        // ��ȡ��������
        unsigned char* data = img.getData ();

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
                    float screenX = leftBottomPosition.x + x * background->getScaleX ();
                    float screenY = leftBottomPosition.y + (height - y - 1) * background->getScaleY ();  // ע�� Y �ᷴ��
                    nonTransparentPixels.push_back ( Vec2 ( screenX , screenY ) );  // ��¼��Ļ����
                }
            }
        }
    }

    // ��ʼ����ɫ���������ӵ�����
    if (player1->getParent () == NULL) {
        this->addChild ( player1 , 17 );
        for (auto& pair : F_lastplace) {
            if (pair.second == true) {  // ��� bool ֵ�Ƿ�Ϊ true
                player1->setPosition ( pair.first.second );
                pair.second = false;
            }
        }
         player1->speed = 4.7f;
         //��Ϊ�������
       /* player1->speed = 20.0f;*/
        player1->setScale ( 1.5f );
        player1->setAnchorPoint ( Vec2 ( 0.5f , 0.2f ) );
    }

    // ��������Ķ�ʱ��
    player1->schedule ( [=]( float dt ) {
        player1->player1_move ();
        } , 0.05f , "player1_move" );

    player1->schedule ( [=]( float dt ) {
        player1->player_change ();
        } , 0.3f , "player_change" );


    // ���㱳����������ź�Χ
    float scaledWidth = background->getContentSize ().width * background->getScaleX ();
    float scaledHeight = background->getContentSize ().height * background->getScaleY ();

    // ���� Follow �ı߽� Rect
    auto followRect = cocos2d::Rect ( leftBottomPosition.x , leftBottomPosition.y , scaledWidth , scaledHeight );

    // ���� Follow ��������������ڱ�����Χ���ƶ�
    auto followAction = Follow::create ( player1 , followRect );
    this->runAction ( followAction );

    // ���ڸ������״̬
    this->schedule ( [this]( float dt ) {
        this->checkPlayerPosition ();  // �������Ƿ�ӽ�������
        } , 0.01f , "check_position_key" );

    //�������ӣ�������ȡ����
    auto mailBox = Sprite::create ( "UIresource/xinxiang/xinxiang.png" );
    mailBox->setPosition ( Vec2 ( 260 , 1050 ) );
    mailBox->setScale ( 0.7f );
    this->addChild ( mailBox , 10 );

    //�������ӣ�����������
    Box = Sprite::create ( "UIresource/xiangzi/xiangzi.png" );
    Box->setPosition ( Vec2 ( 260 , 1150 ) );
    Box->setAnchorPoint ( Vec2 ( 0 , 0 ) );
    Box->setScale ( 0.7f );
    this->addChild ( Box , 10 );

    //������� ��������
    auto cooking_pot = CookingPot::create ();
    cooking_pot->setPosition ( -50 , 1000 );
    cooking_pot->setAnchorPoint ( Vec2 ( 0 , 0 ) );
    cooking_pot->setScale ( 2.7f );
    this->addChild ( cooking_pot , 10 );
    

    //��������
    float interactionRadius = 200.0f;

    auto listener = EventListenerMouse::create ();
    listener->onMouseDown = [this , mailBox , interactionRadius, cooking_pot]( Event* event ) {

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
                    GoldAmount += inventory->GetItemAt ( miniBag->getSelectedSlot () )->GetValue ();
                    inventory->RemoveItem ( miniBag->getSelectedSlot () );
                    inventory->is_updated = true;
                    miniBag->getSelectBack ();
                }
            }
        }

        //�ж��������λ��
        if (cooking_pot->getBoundingBox ().containsPoint ( clickPos )) {
            auto selected_item = miniBag->getSelectedItem ();
            if (selected_item != nullptr && std::dynamic_pointer_cast<Food>(selected_item) != nullptr) {
                cooking_pot->AddIngredient ( *selected_item );
            }
            else if (selected_item == nullptr) {
                auto id = cooking_pot->GetDishesId ();
                auto dishes = cooking_pot->GetDishes ( id );
                if (dishes != nullptr) {
                    auto size = cooking_pot->GetPotSize ();
                    bool can_get_dishes = true;
                    for (int i = 0; i < size; i++) {
                        auto ingredient = cooking_pot->GetPotBack ();
                        if (inventory->RemoveItem ( *ingredient ) == -1) {
                            can_get_dishes = false;
                            break;
                        }
                    }
                    if (can_get_dishes) {
                        inventory->AddItem ( *dishes );
                    }
                }
                cooking_pot->ClearPot ();
            }
            cooking_pot->DisplayPotInCCLOG ();
        }

        // �жϵ��λ���Ƿ������丽����Χ��
        if (button != nullptr && button->getBoundingBox ().containsPoint ( clickPos )) {
            Director::getInstance ()->end ();
        }
        if (mailBox->getBoundingBox ().containsPoint ( clickPos )) {
            // ��ȡ��ҵ�λ��  
            Vec2 playerPos = player1->getPosition ();

            // �������������֮��ľ���  
            float distance = playerPos.distance ( mailBox->getPosition () );
            // �������Ƿ��������ķ�Χ��  
            if (distance <= interactionRadius) {
                mailBoxUI* mailbox = mailBoxUI::create ();
                this->addChild ( mailbox , 20 );
            }
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener , this );

    // ���ü��̼�����
    auto listenerWithPlayer = EventListenerKeyboard::create ();
    listenerWithPlayer->onKeyPressed = [this]( EventKeyboard::KeyCode keyCode , Event* event )
        {
            if (keyCode == EventKeyboard::KeyCode::KEY_ENTER || keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
                isEnterKeyPressed = true;
                CCLOG ( "Enter key pressed. " );
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_P) {
                isPKeyPressed = true;
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_W) {
                isWKeyPressed = true;
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_G) {
                isGKeyPressed = true;
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                static int isOpen = 0;
                static InventoryUI* currentInventoryUI = nullptr;  // ���浱ǰ��ʾ�� InventoryUI  
                // �����ǰû�д� InventoryUI�������  
                if (currentInventoryUI == nullptr || isOpen == 0) {
                    isOpen = 1;
                    CCLOG ( "Opening inventory." );
                    currentInventoryUI = InventoryUI::create ( inventory , "farm" );
                    this->addChild ( currentInventoryUI , 20 );  // �� InventoryUI ���ӵ��ϲ�  
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
            else if (keyCode == EventKeyboard::KeyCode::KEY_P) {
                isPKeyPressed = false;
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_W) {
                isWKeyPressed = false;
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_G) {
                isGKeyPressed = false;
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

    return true;
}


farm* farm::create ()
{
    farm* scene = new farm ();
    if (scene && scene->init ())
    {
        scene->autorelease ();
        return scene;
    }
    CC_SAFE_DELETE ( scene );
    return nullptr;
}

cocos2d::Scene* farm::createScene()
{
    auto scene = cocos2d::Scene::create();
    auto layer = farm::create();
    if (layer)
    {
        layer->setName("farm");
        scene->addChild(layer);
    }
    return scene;
}

void farm::AllInitialize_crop () {


    for (auto it = Crop_information.begin (); it != Crop_information.end (); /* no increment here */) {

        auto crop = *it;  // �����õ������Է��� Crop ����
        int nums = crop->nums;

        cocos2d::log ( "set nums = %d" , nums );

        // ��ȡ����
        std::string type = crop->GetName ();

        // ����ֲ���״̬ѡ����ʾ�ľ���
        if (crop->GetPhase () == Phase::MATURE) {
            auto test = Sprite::create ( crop->mature_pic );
            this->addChild ( test , 15 - nums / 19 );
            test->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            test->setScale ( 1.9f );
        }
        else if (crop->GetPhase () == Phase::GROWING) {
            auto test = Sprite::create ( crop->growing_pic );
            this->addChild ( test , 15 - nums / 19 );
            test->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            test->setScale ( 2.1f );
        }
        else if (crop->GetPhase () == Phase::SAPLESS) {
            auto test = Sprite::create ( "crop/sapless.png" );
            this->addChild ( test , 15 - nums / 19 );
            test->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            test->setScale ( 2.1f );
        }
        else {
            auto test = Sprite::create ( crop->initial_pic );
            this->addChild ( test , 15 - nums / 19 );
            test->setPosition ( 545 + ((nums % 19) - 2) * 48 , 910 - ((nums / 19) - 1) * 48 );
            test->setScale ( 2.1f );
        }

        // ������һ��Ԫ��
        ++it;
    }


}

// �������Ƿ�ӽ�������������
void farm::checkPlayerPosition ()
{

    // ��ȡ��ҵ�λ��
    Vec2 playerPos = player1->getPosition ();
    //����λ��������ʧ
    if (playerPos.y < 0) {
        miniBag->setLocalZOrder ( 0 );
    }
    else {
        miniBag->setLocalZOrder ( 11 );  
    }


    // �������������֮��ľ���  
    float distance = playerPos.distance ( Box->getPosition () );
    // �������Ƿ��������ķ�Χ��  
    if (distance <= 200.0f) {
        Box->setTexture ( "UIresource/xiangzi/Open.png" );
    }
    else {
        Box->setTexture ( "UIresource/xiangzi/xiangzi.png" );
    }

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
            if (pair.first.first == "myhouse") {  // ��� bool ֵ�Ƿ�Ϊ true
                pair.second = true;
            }
        }

        IsSleep = false;
        frombed = true;
        remainingTime = 10800;
        player1->removeFromParent ();
        // 使用GameStateManager切换场景
        GameStateManager::getInstance()->changeState(GameStateType::MYHOUSE);

    }
    // ���±�ǩλ��
    float currentx = 0 , currenty = 0;
    if (playerPos.x <= 637) {
        currentx = 637;
    }
    else if (playerPos.x >= 960) {
        currentx = 960;
    }
    else {
        currentx = playerPos.x;
    }

    if (playerPos.y >= 777) {
        currenty = 777;
    }
    else if (playerPos.y <= 500) {
        currenty = 500;
    }
    else {
        currenty = playerPos.y;
    }

    TimeUI->setPosition ( currentx , currenty );
    button->setPosition ( currentx + 730 , currenty - 590 );
    miniBag->setPosition ( currentx , currenty );

    // ����ֲ�йصĲ���
    if (plant_area.containsPoint ( playerPos )) {

        // �Ƿ�ִ����ֲ
        if (isPKeyPressed) {

            int nums = getRegionNumber ( Vec2 ( playerPos.x + 10 , playerPos.y - 10 ) );

            bool Isplant = false;

            cocos2d::log ( "plant nums = %d" , nums );

            for (auto it = Crop_information.begin (); it != Crop_information.end (); /* no increment here */) {
                if ((*it)->nums == nums) {  // ʹ�� *it �����õ�����
                    Isplant = true;
                    break;
                }
                else {
                    ++it;
                }
            }

            if (Isplant == false) {

                auto temp = miniBag->getSelectedItem ();
                if (temp != nullptr) {
                    cocos2d::log ( "copy item" );
                    std::string TypeName = temp->GetName ();
                    auto point = cropbasicinformation.find ( TypeName );
                    if (point != cropbasicinformation.end ()) {
                        cocos2d::log ( "find crop" );
                        // �ж��Ƿ������ֲ�ļ���
                        if (((cropbasicinformation[TypeName].GetSeason () == Season) || (cropbasicinformation[TypeName].GetSeason () == "All")) && strength >= 10) {
                            inventory->RemoveItem ( *temp );

                            if (nums == 13 && RainBowfirst) {
                                inventory->AddItem(RainBow);
                                RainBowfirst = false;
                            }

                            strength -= 10;
                            TimeUI->UpdateEnergy ();

                            Crop_information.push_back ( cropbasicinformation[TypeName].GetCropCopy () );
                            Crop_information.back ()->plant_day = season[Season] * 7 + day;
                            Crop_information.back ()->nums = nums;

                            if (player1->pic_path == "character1/player_right3.png") {
                                // ��ʼ���ã����õ�һ��ͼƬ���Ŵ�
                                player1->setTexture ( "character1/player_plant3.png" );
                                player1->setScale ( 2.5f );

                                // �ӳ�0.3����л����ڶ���ͼƬ
                                player1->scheduleOnce ( [=]( float dt ) {
                                    player1->setTexture ( "character1/player_plant4.png" );  // ����Ϊplayer_plant2
                                    player1->setScale ( 2.7f );
                                    } , 0.15f , "change_image1_key" );

                                // �ӳ�0.6����л���������ͼƬ
                                player1->scheduleOnce ( [=]( float dt ) {
                                    player1->setTexture ( "character1/player_right3.png" ); // ����Ϊplayer_left3
                                    player1->setScale ( 1.5f );
                                    auto temp = Sprite::create ( Crop_information.back ()->initial_pic );
                                    this->addChild ( temp , 15 - nums / 19 );
                                    temp->setPosition ( 500 + ((nums % 19) - 1) * 48 , 910 - ((nums / 19) - 1) * 48 );
                                    temp->setScale ( 2.1f );
                                    } , 0.35f , "change_image2_key" );
                            }
                            else {
                                // ��ʼ���ã����õ�һ��ͼƬ���Ŵ�
                                player1->setTexture ( "character1/player_plant1.png" );
                                player1->setScale ( 2.5f );

                                // �ӳ�0.3����л����ڶ���ͼƬ
                                player1->scheduleOnce ( [=]( float dt ) {
                                    player1->setTexture ( "character1/player_plant2.png" );  // ����Ϊplayer_plant2
                                    player1->setScale ( 2.7f );
                                    } , 0.15f , "change_image1_key" );

                                // �ӳ�0.6����л���������ͼƬ
                                player1->scheduleOnce ( [=]( float dt ) {
                                    player1->setTexture ( "character1/player_left3.png" ); // ����Ϊplayer_left3
                                    player1->setScale ( 1.5f );
                                    auto temp = Sprite::create ( Crop_information.back ()->initial_pic );
                                    this->addChild ( temp , 15 - nums / 19 );
                                    temp->setPosition ( 500 + ((nums % 19) - 1) * 48 , 910 - ((nums / 19) - 1) * 48 );
                                    temp->setScale ( 2.1f );
                                    } , 0.35f , "change_image2_key" );
                            }

                            

                        }
                    }
                }
            }
        }
        // �Ƿ�ִ�н�ˮ
        else if (isWKeyPressed) {

            int nums = getRegionNumber ( Vec2 ( playerPos.x + 30 , playerPos.y - 10 ) );

            for (auto it : Crop_information) {
                if (it->nums == nums) {
                    // ��Ϊ�ѽ�ˮ
                    it->watered = true;

                    if (player1->pic_path == "character1/player_left3.png") {
                        // ��ʼ���ã����õ�һ��ͼƬ���Ŵ�
                        player1->setTexture ( "character1/player_water4.png" );
                        player1->setScale ( 1.7f );

                        // �ӳ�0.3����л����ڶ���ͼƬ
                        player1->scheduleOnce ( [=]( float dt ) {
                            player1->setTexture ( "character1/player_water3.png" );  // ����Ϊplayer_water1
                            player1->setScale ( 1.7f );
                            } , 0.15f , "change_image1_key" );

                        // �ӳ�0.6����л���������ͼƬ
                        player1->scheduleOnce ( [=]( float dt ) {
                            player1->setTexture ( "character1/player_left3.png" ); // ����Ϊplayer_right3
                            player1->setScale ( 1.5f );
                            // �ָ���ɫ�����Ķ���Ȩ��
                            player1->moveDown = true;
                            player1->moveLeft = true;
                            player1->moveUp = true;
                            player1->moveRight = true;
                            } , 0.35f , "change_image2_key" );

                    }
                    else {
                        // ��ʼ���ã����õ�һ��ͼƬ���Ŵ�
                        player1->setTexture ( "character1/player_water2.png" );
                        player1->setScale ( 1.7f );

                        // �ӳ�0.3����л����ڶ���ͼƬ
                        player1->scheduleOnce ( [=]( float dt ) {
                            player1->setTexture ( "character1/player_water1.png" );  // ����Ϊplayer_water1
                            player1->setScale ( 1.7f );
                            } , 0.15f , "change_image1_key" );

                        // �ӳ�0.6����л���������ͼƬ
                        player1->scheduleOnce ( [=]( float dt ) {
                            player1->setTexture ( "character1/player_right3.png" ); // ����Ϊplayer_right3
                            player1->setScale ( 1.5f );
                            // �ָ���ɫ�����Ķ���Ȩ��
                            player1->moveDown = true;
                            player1->moveLeft = true;
                            player1->moveUp = true;
                            player1->moveRight = true;
                            } , 0.35f , "change_image2_key" );
                    }

                }
            }

        }
        // �Ƿ�ִ���ջ�
        else if (isGKeyPressed) {

            int nums = getRegionNumber ( Vec2 ( playerPos.x + 10 , playerPos.y - 10 ) );


            for (auto it = Crop_information.begin (); it != Crop_information.end (); /* no increment here */) {
                if ((*it)->nums == nums) {  // ʹ�� *it �����õ�����
                    if ((*it)->GetPhase () == Phase::MATURE && strength >= 10) {

                        skill_tree->AddExperience ( farming_skill , 10 );

                        auto find_temp = (*it);

                        if (find_temp->GetName () == "potato") {
                            inventory->AddItem ( potato );
                        }

                        strength -= 10;
                        TimeUI->UpdateEnergy ();

                        // ���Ǿ���
                        auto test = Sprite::create ( "farm/tile.png" );
                        this->addChild ( test , 15 - nums / 19 );
                        test->setPosition ( 495 + ((nums % 19) - 1) * 48 , 910 - ((nums / 19) - 1) * 48 );
                        test->setScaleX ( 2.5f );
                        test->setScaleY ( 1.9f );

                        // ɾ��Ԫ�ز����µ�����
                        it = Crop_information.erase ( it );  // erase �����µĵ����� 

                        // ��ʼ���ã����õ�һ��ͼƬ���Ŵ�
                        player1->setTexture ( "character1/player_plant1.png" );
                        player1->setScale ( 2.5f );

                        // �ӳ�0.3����л����ڶ���ͼƬ
                        player1->scheduleOnce ( [=]( float dt ) {
                            player1->setTexture ( "character1/player_plant2.png" );  // ����Ϊplayer_plant2
                            player1->setScale ( 2.7f );
                            } , 0.15f , "change_image1_key" );

                        // �ӳ�0.6����л���������ͼƬ
                        player1->scheduleOnce ( [=]( float dt ) {
                            player1->setTexture ( "character1/player_left3.png" ); // ����Ϊplayer_left3
                            player1->setScale ( 1.5f );
                            } , 0.35f , "change_image2_key" );

                    }
                    else {
                        break;
                    }
                }
                else {
                    ++it;  // ����������һ��Ԫ��
                }
            }

        }

    }

    // �Ƿ������ҵļ�
    if (myhouse_area.containsPoint ( playerPos )) {
        if (isEnterKeyPressed) {
            for (auto& pair : F_lastplace) {
                if (pair.first.first == "myhouse") {
                    pair.second = true;
                }
            }
            // 使用GameStateManager切换到房子状态
            GameStateManager::getInstance()->changeState(GameStateType::HOUSE);
        }
    }

    // �Ƿ��뿪ũ��
    if (Out_Farm.containsPoint ( playerPos )) {
        if (isEnterKeyPressed) {

        }
    }

    // �Ƿ��������
    if (barn_area.containsPoint ( playerPos )) {
        if (isEnterKeyPressed) {
            for (auto& pair : F_lastplace) {
                if (pair.first.first == "barn") {  // ��� bool ֵ�Ƿ�Ϊ true
                    pair.second = true;
                }
            }
            // 使用GameStateManager切换到谷仓状态
            GameStateManager::getInstance()->changeState(GameStateType::BARN);
        }
    }

    // �Ƿ����ɽ��
    if (cave_area.containsPoint ( playerPos )) {
        if (isEnterKeyPressed) {
            for (auto& pair : F_lastplace) {
                if (pair.first.first == "cave") {  // ��� bool ֵ�Ƿ�Ϊ true
                    pair.second = true;
                }
            }
            // 使用GameStateManager切换到洞穴状态
            GameStateManager::getInstance()->changeState(GameStateType::CAVE);
        }
    }

    // �Ƿ����ɭ��
    if (forest_area.containsPoint ( playerPos )) {
        if (isEnterKeyPressed) {
            for (auto& pair : F_lastplace) {
                if (pair.first.first == "forest") {  // ��� bool ֵ�Ƿ�Ϊ true
                    pair.second = true;
                }
            }
            // 使用GameStateManager切换到森林状态
            GameStateManager::getInstance()->changeState(GameStateType::FOREST);
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

int farm::getRegionNumber ( Vec2 pos ) {

    // �����������Ĳ���
    int left_bottom_x = 496;  // ���½�x����
    int left_bottom_y = 467;  // ���½�y����
    int width = 912;          // ���ο���
    int height = 480;         // ���θ߶�
    int block_size = 48;      // ÿ��Ĵ�С

    // �����ܵ�����������
    int rows = height / block_size;  // ����
    int cols = width / block_size;   // ����

    // �������������к��б��
    int col = (pos.x - left_bottom_x) / block_size;
    int row = (left_bottom_y + height - pos.y) / block_size;
    // ��ֹԽ��
    if (col < 0) {
        col = 0;
    }
    if (row < 0) {
        row = 0;
    }
    // ���������ţ����к���
    int region_number = (row)*cols + col + 1;  // ��Ŵ�1��ʼ

    return region_number;
}

void farm::createRainEffect () {

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


void farm::updaterain ( float deltaTime ) {
    if (emitter) {
        // �������һ���������ڣ���Χ 1 �� 1.5 ��֮�䣩
        float newLife = cocos2d::rand_0_1 () * 1.5f;

        // �����µ���������
        emitter->setLife ( newLife );

        emitter->setEmissionRate ( emitter->getTotalParticles () / emitter->getLife () * 1.3 );
    }
}







