#include "GameBeginUI.h"  
#include "CreateCharacterUI.h"  
#include "Player.h"  
#include "Town.h"  
#include "GameStateManager.h"

USING_NS_CC;
///////////////////////////////
//����ΪBeginScene��ʼ������ʵ�ֲ���
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}
bool BeginScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto layer = cocos2d::LayerColor::create(cocos2d::Color4B(255, 255, 255, 255)); // ���ñ�����ɫΪ��ɫ  
    this->addChild(layer);

    // ���ſ�������  
    playSplashAnimation();

    return true;
}

void BeginScene::playSplashAnimation()
{
    //��������

    cocos2d::Vector<cocos2d::SpriteFrame*> animationFrames;
    std::string frameName1 = "UIresource/hebing1.png";
    auto frame1 = SpriteFrame::create("UIresource/hebing1.png", Rect(0, 0, 214, 67));
    auto frame2 = SpriteFrame::create("UIresource/hebing2.png", Rect(0, 0, 220, 69));
    auto frame3 = SpriteFrame::create("UIresource/hebing3.png", Rect(0, 0, 214, 67));
    auto frame4 = SpriteFrame::create("UIresource/hebing4.png", Rect(0, 0, 220, 69));
    auto frame5 = SpriteFrame::create("UIresource/hebing5.png", Rect(0, 0, 214, 67));
    auto frame6 = SpriteFrame::create("UIresource/hebing6.png", Rect(0, 0, 220, 69));
    animationFrames.pushBack(frame1);
    animationFrames.pushBack(frame2);
    animationFrames.pushBack(frame3);
    animationFrames.pushBack(frame4);
    animationFrames.pushBack(frame5);
    animationFrames.pushBack(frame6);
    auto animation = cocos2d::Animation::createWithSpriteFrames(animationFrames, 0.2f); // 0.2��Ϊÿ֡�ĳ���ʱ��  

    auto sprite = cocos2d::Sprite::createWithSpriteFrame(animationFrames.at(0));

    sprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

    auto visibleSize = Director::getInstance()->getVisibleSize();
    sprite->setPosition(visibleSize.width / 2.5 + visibleSize.width * 0.139, visibleSize.height / 2 );
    this->addChild(sprite); // ���ӵ�����  

    // 4. ʹ�ö�������  
    auto animate = cocos2d::Animate::create(animation);
    auto repeat = Repeat::create ( animate , 3 );  // �����ظ�3��
    // �ڶ�����ɺ��þ���͸��
    auto fadeOut = FadeOut::create(1.0f); // 1.0������ʧ
    auto sequence1 = Sequence::create(repeat, fadeOut, nullptr); // ������ɺ�ִ��FadeOut
    auto animateSequence = Sequence::create(
        sequence1,
        CallFunc::create([this, sprite]() {
            // ȷ���ڶ����������Ա���������λ��  
            auto visibleSize = Director::getInstance()->getVisibleSize();
            sprite->setPosition(visibleSize.width / 2.5 + visibleSize.width * 0.139, visibleSize.height / 2); // ȷ������λ����ԭ��  
            onAnimationComplete();
            }),
        nullptr
    );
    sprite->setScale(3.0f);  // ������������������
    sprite->runAction(animateSequence); // ���Ŷ���   
}

void BeginScene::onAnimationComplete()
{
    Director::getInstance()->replaceScene(NextScene::create());
}

/////////////////////
//����ΪNextSceneʵ�ֲ��֣�����ʵ�ֽ����ϲ��֡��������˵��Ķ���Ч��
bool NextScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    // ��ʼ������������л��˶���
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto layer = cocos2d::LayerColor::create(cocos2d::Color4B(255, 255, 255, 255),
        visibleSize.width,
        visibleSize.height);
    this->addChild(layer, 5);
    // ʹ�� fadeIn ������ʹ���͸������ȫ��͸��
    layer->runAction(cocos2d::FadeTo::create(2.0f, 0)); // 2���ڴ�͸������ȫ��͸��
    addLogo_sec  (visibleSize , origin);
    cloudsAni ( 1.0f );
    this->schedule ( [this]( float dt ) {
        cloudsAni ( dt );
     } , 20.0f , "create_sprite_key" );
    addfirstscene(visibleSize , origin);
    birdfly();
    return true;
}

void NextScene::addfirstscene(const cocos2d::Size& visibleSize, const cocos2d::Vec2& origin)
{
    //���ӱ���
    auto background = Sprite::create ( "UIresource/background2.png" );
    if (background) {
        background->setContentSize ( Size ( visibleSize.width , visibleSize.height * 2 ) );
        background->setPosition    ( Vec2 ( visibleSize.width / 2 + origin.x , origin.y + visibleSize.height ) );
        this->addChild ( background , 0 );
    }
    else {
        problemLoading ( "'background2.png'" );
    }

    auto mountain1 = Sprite::create("UIresource/mountainfront.png");
    mountain1->setTag ( 101 );
    auto mountain2 = Sprite::create("UIresource/mountainbehind.png");
    mountain2->setTag ( 102 );
    auto treeleft  = Sprite::create("UIresource/treeleft.png");
    treeleft->setTag (  103 );
    auto treeright = Sprite::create("UIresource/treeright.png");
    treeright->setTag ( 104 );
    if (mountain1) {
        // ��ȡ�����ԭʼ�ߴ�
        Size spriteSize = mountain1->getContentSize();

        // �������ű���
        float scaleX = visibleSize.width / spriteSize.width; // ������Ļ���Ⱥ;�����ȼ���ˮƽ���ű���
        float scaleY = scaleX; // ���ֳ����Ȳ��䣬���Y������ű�������X������ű���

        // ���ݼ�����ı������þ��������
        mountain1->setScale(2 * scaleX); // ����ʹ�� setScale(scaleX, scaleY);
        mountain1->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height * 0.25));
        this->addChild(mountain1, 3);
    }
    else {
        problemLoading("'mountain1.png'");
    }
    if (mountain2) {
        Size spriteSize = mountain2->getContentSize();

        // �������ű���
        float scaleX = visibleSize.width / spriteSize.width; // ������Ļ���Ⱥ;�����ȼ���ˮƽ���ű���
        float scaleY = scaleX; // ���ֳ����Ȳ��䣬���Y������ű�������X������ű���

        // ���ݼ�����ı������þ��������
        mountain2->setScale(2 * scaleX);
        mountain2->setPosition(Vec2(visibleSize.width / 2 + origin.x - visibleSize.width * 0.05, origin.y + visibleSize.height * 0.35));
        this->addChild(mountain2, 2);
    }
    else {
        problemLoading("'mountain2.png'");
    }
    if (treeleft) {
        Size spriteSize = treeleft->getContentSize();

        // �������ű���
        float maxWidth = visibleSize.width * 0.6f;
        float maxHeight = visibleSize.height * 0.6f;
        float scaleX = maxWidth / spriteSize.width;
        float scaleY = maxHeight / spriteSize.height;
        // ʹ�� setScale ������ͼƬ
        treeleft->setScale(std::min(scaleX, scaleY));  // ���ֿ��߱�
        // ����ê��Ϊ���½�
        treeleft->setAnchorPoint(Vec2(0, 0));
        // �������λ������Ϊ��Ļ���½�
        treeleft->setPosition(Vec2(0, 0));
        this->addChild(treeleft, 4);
    }
    else {
        problemLoading("'treeleft.png'");
    }
    if (treeright) {
        Size spriteSize = treeright->getContentSize();

        // �������ű���
        float maxWidth = visibleSize.width * 0.6f;
        float maxHeight = visibleSize.height * 0.6f;
        float scaleX = maxWidth / spriteSize.width;
        float scaleY = maxHeight / spriteSize.height;
        // ʹ�� setScale ������ͼƬ
        treeright->setScale(std::min(scaleX, scaleY));  // ���ֿ��߱�
        // ����ê��Ϊ���½�
        treeright->setAnchorPoint(Vec2(1, 0));
        // �������λ������Ϊ��Ļ���½�
        treeright->setPosition(Vec2(visibleSize.width + origin.x , 0));
        this->addChild(treeright, 4);
    }
    else {
        problemLoading("'treeright.png'");
    }
}

void NextScene::birdfly()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // ��������֡
    Vector<cocos2d::SpriteFrame*> animFrames;

    // ����4��ͼƬ
    animFrames.pushBack(cocos2d::SpriteFrame::create("UIresource/fly1.png", cocos2d::Rect(0, 0, 26, 26)));
    animFrames.pushBack(cocos2d::SpriteFrame::create("UIresource/fly2.png", cocos2d::Rect(0, 0, 26, 26)));
    animFrames.pushBack(cocos2d::SpriteFrame::create("UIresource/fly3.png", cocos2d::Rect(0, 0, 26, 26)));
    animFrames.pushBack(cocos2d::SpriteFrame::create("UIresource/fly4.png", cocos2d::Rect(0, 0, 26, 26)));

    // ��������
    auto animation1 = cocos2d::Animation::createWithSpriteFrames(animFrames, 0.2f);  // ÿ֡0.2��
    auto animation2 = cocos2d::Animation::createWithSpriteFrames(animFrames, 0.2f);  // ÿ֡0.2��
    // ������������
    auto animate1 = cocos2d::Animate::create(animation1);
    auto animate2 = cocos2d::Animate::create(animation2);
    // �����ظ��Ĵ�����ʹ��ʱ�����Ҫ��
    float totalDuration = 4.5f;  // �趨��ʱ��
    int repeatCount1 = totalDuration / animation1->getDuration();
    auto repeatedAction1 = Repeat::create(animate1, repeatCount1);
    int repeatCount2 = totalDuration / animation2->getDuration();
    auto repeatedAction2 = Repeat::create(animate2, repeatCount2);
    // �����ƶ�����
    auto moveAction1 = MoveBy::create(5.0f, Vec2(-visibleSize.width / 1.5, 0)); // 5������X���ƶ�
    auto moveAction2 = MoveBy::create(5.0f, Vec2(-visibleSize.width / 1.5, 0)); // 5������X���ƶ�
    // �������鲢���Ŷ���
     auto sprite1 = cocos2d::Sprite::createWithSpriteFrame(animFrames.at(0));
     auto sprite2 = cocos2d::Sprite::createWithSpriteFrame(animFrames.at(0));
     sprite1->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 3));  // ���þ���λ��
     sprite2->setPosition(cocos2d::Vec2(visibleSize.width / 1.8, visibleSize.height / 3.2));  // ���þ���λ��
     // ͬʱִ�ж������ƶ�
     auto spawnAction1 = Spawn::create(repeatedAction1, moveAction1, nullptr);
     auto spawnAction2 = Spawn::create(repeatedAction2, moveAction2, nullptr);
     auto sequence = Sequence::create(
         spawnAction1,
         //��ɳ���Ļ��ʼ����ת��
         CallFunc::create([this, sprite1]() {
             scenechangedown();
              }),
         nullptr);
     sprite1->runAction(sequence); // ѭ�����Ŷ���
     sprite2->runAction(spawnAction2);  // ѭ�����Ŷ���
    // ���������ӵ�������
    sprite1->setScale(2.0f);  // ������������������
    sprite2->setScale(2.0f);  // ������������������
    this->addChild(sprite1, 3);
    this->addChild(sprite2, 3);
}

void NextScene::addLogo_sec ( const Size& visibleSize , const Vec2& origin )
{
    auto logo = Sprite::create ( "UIresource/title.png" );
    if (logo) {
        Size logoSize = logo->getContentSize ();
        float maxWidth = visibleSize.width * 0.7f;
        float maxHeight = visibleSize.height * 0.7f;
        float scaleX = maxWidth / logoSize.width;
        float scaleY = maxHeight / logoSize.height;
        logo->setScale ( std::min ( scaleX , scaleY ) );
        logo->setPosition( Vec2 ( visibleSize.width / 2 + origin.x , visibleSize.height / 4 * 2.5 + origin.y + visibleSize.height) );
        this->addChild ( logo , 2 );
    }
    else {
        problemLoading ( "'title.png'" );
    }
}

void NextScene::cloudsAni ( float dt )
{
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    Vec2 origin = Director::getInstance ()->getVisibleOrigin ();
    auto cloud0 = Sprite::create ( "UIresource/Cloud.png" );
    auto clouds = Sprite::create ( "UIresource/Clouds.png" );
    if (cloud0) {
        Size spriteSize = cloud0->getContentSize ();
        // �������ű���
        float maxWidth = visibleSize.width * 0.6f;
        float maxHeight = visibleSize.height * 0.6f;
        float scaleX = maxWidth / spriteSize.width;
        float scaleY = maxHeight / spriteSize.height;
        // ʹ�� setScale ������ͼƬ
        cloud0->setScale ( std::min ( scaleX , scaleY ) );  // ���ֿ��߱�
        // ����ê��Ϊ���½�
        cloud0->setAnchorPoint ( Vec2 ( 1 , 0 ) );
        // �������λ������Ϊ��Ļ���½ǿ���һ�㿿��һ��
        cloud0->setPosition ( Vec2 ( visibleSize.width * 1.5 + origin.x , visibleSize.height / 1.8 + origin.y ) );
        this->addChild ( cloud0 , 1 );
    }
    else {
        problemLoading ( "'Cloud.png'" );
    }
    if (clouds) {
        Size cloudsSize = clouds->getContentSize ();
        float maxWidth = visibleSize.width * 1.1f;
        float maxHeight = visibleSize.height * 1.1f;
        float scaleX = maxWidth / cloudsSize.width;
        float scaleY = maxHeight / cloudsSize.height;
        clouds->setScale ( std::min ( scaleX , scaleY ) );
        cloud0->setAnchorPoint ( Vec2 ( 1 , 0 ) );
        clouds->setPosition ( Vec2 ( visibleSize.width * 1.8 + origin.x , visibleSize.height * 0.8 + origin.y + visibleSize.height ) );
        this->addChild ( clouds , 1 );
    }
    else {
        problemLoading ( "'Clouds.png'" );
    }

    // ���㾫���ƶ���Ŀ��λ�ã���Ļ����ࣩ
    auto moveTo_0 = cocos2d::MoveTo::create ( 50.0f , cocos2d::Vec2 ( 0 , cloud0->getPosition ().y ) );
    auto moveTo_s = cocos2d::MoveTo::create ( 50.0f , cocos2d::Vec2 ( -visibleSize.width * 0.6 , clouds->getPosition ().y ) );
    cloud0->runAction ( cocos2d::RepeatForever::create ( moveTo_0 ) );
    clouds->runAction ( cocos2d::RepeatForever::create ( moveTo_s ) );
}

void NextScene::scenechangedown ()
{
    auto visibleSize = Director::getInstance ()->getVisibleSize ();
    Vec2 origin = Director::getInstance ()->getVisibleOrigin ();
    // ��ȡ��ǰ���еĳ���
    auto scene = Director::getInstance ()->getRunningScene ();

    // ���������ƶ���λ��������Ҫ�����ƶ�������������
    Vec2 moveDistance = Vec2 ( 0 , -visibleSize.height );

    // ���� MoveBy ����������ʱ��Ϊ 8 ��
    auto moveAction = MoveBy::create ( 8.0f , moveDistance );


    auto mountain1 = this->getChildByTag ( 101 );
    auto mountain2 = this->getChildByTag ( 102 );
    auto treeleft = this->getChildByTag  ( 103 );
    auto treeright = this->getChildByTag ( 104 );
    auto moveDown1 = cocos2d::MoveBy::create ( 20.0f , cocos2d::Vec2 ( 0 , -visibleSize.height * 3 ) ); // �����ƶ�
    auto moveDown2 = cocos2d::MoveBy::create ( 40.0f ,cocos2d::Vec2 ( 0 , -visibleSize.height * 3 ) );  // �����ƶ�
    auto moveDown3 = cocos2d::MoveBy::create ( 15.0f , cocos2d::Vec2 ( 0 , -visibleSize.height * 3 ) );  // �����ƶ�
    auto moveDown4 = cocos2d::MoveBy::create ( 15.0f , cocos2d::Vec2 ( 0 , -visibleSize.height * 3 ) );  // �����ƶ�

    scene    ->runAction ( moveAction);
    mountain1->runAction ( moveDown1 );
    mountain2->runAction ( moveDown2 );
    treeleft ->runAction ( moveDown3 );
    treeright->runAction ( moveDown4 );

    // �����˵���  
    auto BeginItem = createMenuItem ( "UIresource/start1.png" , "UIresource/start1.png" , CC_CALLBACK_1 ( NextScene::menuNewCallback , this ) , origin , -visibleSize.width * 0.338 );
    auto LoadItem = createMenuItem ( "UIresource/load1.png" , "UIresource/load1.png" , CC_CALLBACK_1 ( NextScene::menuNewCallback , this ) , origin , -visibleSize.width * 0.113 );
    auto coopItem = createMenuItem ( "UIresource/coop1.png" , "UIresource/coop1.png" , CC_CALLBACK_1 ( NextScene::menuNewCallback , this ) , origin , visibleSize.width * 0.113 );
    auto closeItem = createMenuItem ( "UIresource/quit1.png" , "UIresource/quit1.png" , CC_CALLBACK_1 ( NextScene::menuCloseCallback , this ) , origin , visibleSize.width * 0.338 );

    // ʹ����ʱ��˳�򶯻�����ʾ�˵���
    auto delay0 = DelayTime::create ( 5.0f ); // 5���ӳ�
    auto delay1 = DelayTime::create ( 1.0f ); // 1���ӳ�
    auto delay2 = DelayTime::create ( 1.0f ); // 1���ӳ�
    auto delay3 = DelayTime::create ( 1.0f ); // 1���ӳ�

    auto showBeginItem = FadeIn::create ( 0.5f );  // ��ʼ��ť����
    auto showLoadItem  = FadeIn::create ( 0.5f );   // ���ذ�ť����
    auto showCoopItem  = FadeIn::create ( 0.5f );   // ������ť����
    auto showCloseItem = FadeIn::create ( 0.5f );  // �˳���ť����

    auto sequence1 = Sequence::create ( delay0 , showBeginItem , nullptr );                             // BeginItem ���Ƚ���
    auto sequence2 = Sequence::create ( delay0 , delay1 , showLoadItem , nullptr );                     // LoadItem �ڵȴ� 1 �����
    auto sequence3 = Sequence::create ( delay0 , delay1 , delay2 , showCoopItem , nullptr );            // CoopItem �ڵȴ� 2 �����
    auto sequence4 = Sequence::create ( delay0 , delay1 , delay2 , delay3 , showCloseItem , nullptr );  // CloseItem �ڵȴ� 3 �����

    BeginItem->runAction ( sequence1->clone () );
    LoadItem->runAction  ( sequence2->clone () );
    coopItem->runAction  ( sequence3->clone () );
    closeItem->runAction ( sequence4->clone () );

    // �����˵�  
    auto menu = Menu::create ( BeginItem , LoadItem , coopItem , closeItem , nullptr );
    menu->setPosition ( Vec2 ( 0 , visibleSize.height ) );
    this->addChild ( menu , 3 );

    // ������������  
    addMouseListener ( BeginItem , LoadItem , coopItem , closeItem );
}


MenuItemImage* NextScene::createMenuItem(const std::string& normalImage, const std::string& selectedImage, const ccMenuCallback& callback, const Vec2& origin, float offsetX)
{
    auto item = MenuItemImage::create(normalImage, selectedImage, callback);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (item) {
        Size itemSize = item->getContentSize();
        // ��ȡ��Ļ��������Ĵ�С
        auto visibleSize = Director::getInstance()->getVisibleSize();
        // ����ͼ������ű���
        float maxWidth = visibleSize.width * 0.2f;
        float maxHeight = visibleSize.height * 0.2f;
        float scaleX = maxWidth / itemSize.width ;
        float scaleY = maxHeight / itemSize.height ;
        // ʹ�� setScale ������ͼƬ
        item->setScale(std::min(scaleX, scaleY));  // ���ֿ��߱�
        float x = origin.x + visibleSize.width / 2 + offsetX;
        float y = origin.y + visibleSize.height / 720 * 100;
        item->setPosition(Vec2(x, y));
        item->setOpacity(0);//��ʼ͸��
    }
    else {
        problemLoading(normalImage.c_str());
    }
    return item;
}

void NextScene::addMouseListener(MenuItemImage* BeginItem, MenuItemImage* LoadItem, MenuItemImage* coopItem, MenuItemImage* closeItem)
{
    auto listener = EventListenerMouse::create();
    listener->onMouseMove = [this, BeginItem, LoadItem, coopItem, closeItem](EventMouse* event) {
        Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());

        updateMenuItem(BeginItem, mousePos, "UIresource/start1.png", "UIresource/start2.png");
        updateMenuItem(LoadItem,  mousePos, "UIresource/load1.png" , "UIresource/load2.png" );
        updateMenuItem(coopItem,  mousePos, "UIresource/coop1.png" , "UIresource/coop2.png" );
        updateMenuItem(closeItem, mousePos, "UIresource/quit1.png" , "UIresource/quit2.png" );
        };
    listener->onMouseDown = [this , BeginItem , LoadItem , coopItem , closeItem]( EventMouse* event ) {
        Vec2 mousePos = Vec2 ( event->getCursorX () , event->getCursorY () );
        if (BeginItem->getBoundingBox ().containsPoint ( mousePos )) {
            menuNewCallback ( BeginItem );  // ���������� BeginItem������ûص�
        }
        else if (LoadItem->getBoundingBox ().containsPoint ( mousePos )) {
            menuNewCallback ( LoadItem );  // ���������� LoadItem������ûص�
        }
        else if (coopItem->getBoundingBox ().containsPoint ( mousePos )) {
            menuNewCallback ( coopItem );  // ���������� coopItem������ûص�
        }
        else if (closeItem->getBoundingBox ().containsPoint ( mousePos )) {
            menuCloseCallback( closeItem );  // ���������� closeItem������ûص�
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void NextScene::updateMenuItem(MenuItemImage* item, const Vec2& mousePos, const std::string& normalImage, const std::string& selectedImage)
{
    Size itemSize = item->getContentSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    // ����ͼ������ű���
    float maxWidth = visibleSize.width * 0.2f;
    float maxHeight = visibleSize.height * 0.2f;
    float scaleX = maxWidth / itemSize.width;
    float scaleY = maxHeight / itemSize.height;

    if (item->getBoundingBox().containsPoint(mousePos)) {
        item->setNormalImage(cocos2d::Sprite::create(selectedImage));
        item->setSelectedImage(cocos2d::Sprite::create(selectedImage));
        item->setScale(1.2* std::min(scaleX, scaleY));
    }
    else {
        item->setNormalImage(cocos2d::Sprite::create(normalImage));
        item->setSelectedImage(cocos2d::Sprite::create(normalImage));
        item->setScale(std::min(scaleX, scaleY));
    }
}

// ��ת���³���  
void NextScene::menuNewCallback(Ref* pSender)
{
    GameStateManager::getInstance().changeState(GameStateType::CHARACTER_CREATION, "fade", 3.0f);
}

void NextScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}