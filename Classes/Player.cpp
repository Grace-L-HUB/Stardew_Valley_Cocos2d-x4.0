#include "Player.h"
#include "EventManager.h"
#include "GameEvent.h"

USING_NS_CC;  // ʹ�� Cocos2d-x �����ռ�

// Player ��Ĺ��캯��
Player::Player() : speed(10.0f),pic_path("character1 / player_down3.png") {}

// Player �����������
Player::~Player() {}

// Player ��ĳ�ʼ������
bool Player::init()
{
    if (!Sprite::init())  // ��ʼ������ Sprite������Ƿ�ɹ�
    {
        return false;  // �����ʼ��ʧ�ܣ����� false
    }

    // ���ؽ�ɫ��ͼƬ����ҳ��µ�վ��ͼƬ��
    this->initWithFile("character1/player_down3.png");

    // ���������¼�������
    auto keyboardListener = EventListenerKeyboard::create();

    // ���¼���ʱ���·���
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);

    // �����ͷ�ʱֹͣ�ƶ�
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);

    // �����������ӵ��¼��ɷ����У������¼�
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // ÿ 0.05 �����һ�� player1_move() ��������������ƶ�
    this->schedule([this](float dt) {
        this->player1_move();
        }, 0.05f, "player1_move");

    // ÿ 0.3 �����һ�� player_change() ������������Ҷ����л�
    this->schedule([this](float dt) {
        this->player_change();
        }, 0.3f, "player_change");

    return true;  // ��ʼ���ɹ������� true
}

// Player ��ľ�̬��������
Player* Player::create()
{
    Player* player = new Player();  // ���� Player ����
    if (player && player->init())  // �����������ʼ���ɹ�
    {
        player->autorelease();  // �Զ��ͷ��ڴ�
        return player;  // �����´����� Player ����
    }
    CC_SAFE_DELETE(player);  // �������ʧ�ܣ�ɾ�� player ����
    return nullptr;  // ���ؿ�ָ��
}

// ��������ʱ�Ļص�����
void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    float X = this->getPositionX();  // ��ȡ��ǰ��ҵ� X ����
    float Y = this->getPositionY();  // ��ȡ��ǰ��ҵ� Y ����

    // �жϰ��µķ�����������½�ɫ���ƶ�״̬
    if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW && !uppressed)  // �ϼ�ͷ
    {
        uppressed = true;  // ����ϼ��Ѱ���
        notifyStateChanged("movingUp", true);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW && !downpressed)  // �¼�ͷ
    {
        downpressed = true;  // ����¼��Ѱ���
        notifyStateChanged("movingDown", true);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW && !leftpressed)  // ���ͷ
    {
        leftpressed = true;  // �������Ѱ���
        notifyStateChanged("movingLeft", true);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && !rightpressed)  // �Ҽ�ͷ
    {
        rightpressed = true;  // ����Ҽ��Ѱ���
        notifyStateChanged("movingRight", true);
    }
}

// �����ͷ�ʱ�Ļص�����
void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    // �ж��ɿ��ķ�����������½�ɫ���ƶ�״̬
    if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)  // �ϼ�ͷ
    {
        this->look_state = 0;  // ��λ look_state ״̬
        this->setTexture("character1/player_up3.png");  // ������ҳ��ϵ�ͼƬ
        this->pic_path = "character1/player_up3.png";
        uppressed = false;  // ����ϼ����ɿ�
        notifyStateChanged("movingUp", false);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)  // �¼�ͷ
    {
        this->look_state = 0;  // ��λ look_state ״̬
        this->setTexture("character1/player_down3.png");  // ������ҳ��µ�ͼƬ
        this->pic_path = "character1/player_down3.png";
        downpressed = false;  // ����¼����ɿ�
        notifyStateChanged("movingDown", false);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)  // ���ͷ
    {
        this->look_state = 0;  // ��λ look_state ״̬
        this->setTexture("character1/player_left3.png");  // ������ҳ����ͼƬ
        this->pic_path = "character1/player_left3.png";
        leftpressed = false;  // ���������ɿ�
        notifyStateChanged("movingLeft", false);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)  // �Ҽ�ͷ
    {
        this->look_state = 0;  // ��λ look_state ״̬
        this->setTexture("character1/player_right3.png");  // ������ҳ��ҵ�ͼƬ
        this->pic_path = "character1/player_right3.png";
        rightpressed = false;  // ����Ҽ����ɿ�
        notifyStateChanged("movingRight", false);
    }
}

// ����ƶ����߼�
void Player::player1_move() {
    cocos2d::Vec2 oldPosition = this->getPosition();
    
    bool hasMoved = false;

    // ����������ͷ�������������ƶ�
    if (this->leftpressed && this->moveLeft) {
        if (this->look_state == 0) {
            this->look_state++;  // �����Ҿ�ֹ״̬������״̬
            return;
        }
        this->setPositionX(this->getPositionX() - speed);  // �����ƶ�
        hasMoved = true;
    }
    // ��������¼�ͷ�������������ƶ�
    else if (this->downpressed && this->moveDown) {
        if (this->look_state == 0) {
            this->look_state++;  // �����Ҿ�ֹ״̬������״̬
            return;
        }
        this->setPositionY(this->getPositionY() - speed);  // �����ƶ�
        hasMoved = true;
    }
    // ��������ϼ�ͷ�������������ƶ�
    else if (this->uppressed && this->moveUp) {
        if (this->look_state == 0) {
            this->look_state++;  // �����Ҿ�ֹ״̬������״̬
            return;
        }
        this->setPositionY(this->getPositionY() + speed);  // �����ƶ�
        hasMoved = true;
    }
    // ��������Ҽ�ͷ�������������ƶ�
    else if (this->rightpressed && this->moveRight) {
        if (this->look_state == 0) {
            this->look_state++;  // �����Ҿ�ֹ״̬������״̬
            return;
        }
        this->setPositionX(this->getPositionX() + speed);  // �����ƶ�
        hasMoved = true;
    }
    
    // 如果位置发生变化，发送位置变化事件
    if (hasMoved) {
        notifyPositionChanged(this->getPosition());
    }
}

// 发送玩家位置变化事件
void Player::notifyPositionChanged(const cocos2d::Vec2& newPosition) {
    std::shared_ptr<PlayerPositionChangedEventData> data = 
        std::make_shared<PlayerPositionChangedEventData>(newPosition);
    std::shared_ptr<GameEvent> event = 
        std::make_shared<GameEvent>(GameEventType::PLAYER_POSITION_CHANGED, data);
    EventManager::getInstance()->dispatchEvent(event);
}

// 发送玩家状态变化事件
void Player::notifyStateChanged(const std::string& state, bool value) {
    std::shared_ptr<PlayerStateChangedEventData> data = 
        std::make_shared<PlayerStateChangedEventData>(state, value);
    std::shared_ptr<GameEvent> event = 
        std::make_shared<GameEvent>(GameEventType::PLAYER_STATE_CHANGED, data);
    EventManager::getInstance()->dispatchEvent(event);
}

// 获取玩家当前位置
cocos2d::Vec2 Player::getPosition() const {
    return cocos2d::Sprite::getPosition();
}

// 设置玩家能量并发送事件
void Player::setEnergy(int newEnergy) {
    int oldEnergy = current_energy;
    current_energy = newEnergy;
    
    // 确保能量不超过上限
    if (current_energy > energy_limit) {
        current_energy = energy_limit;
    }
    
    // 确保能量不为负数
    if (current_energy < 0) {
        current_energy = 0;
    }
    
    // 如果能量发生变化，发送事件
    if (oldEnergy != current_energy) {
        std::shared_ptr<PlayerEnergyChangedEventData> data = 
            std::make_shared<PlayerEnergyChangedEventData>(oldEnergy, current_energy, energy_limit);
        std::shared_ptr<GameEvent> event = 
            std::make_shared<GameEvent>(GameEventType::PLAYER_ENERGY_CHANGED, data);
        EventManager::getInstance()->dispatchEvent(event);
    }
}

// 设置玩家经验
void Player::setExperience(int newExp) {
    experience = newExp;
    
    // 检查是否可以升级
    while (experience >= exp_to_next_level) {
        // 扣除升级所需经验
        experience -= exp_to_next_level;
        
        // 升级并计算下一级所需经验
        int oldLevel = current_level;
        current_level++;
        exp_to_next_level = static_cast<int>(exp_to_next_level * 1.5f); // 每级升级所需经验增加50%
        
        // 发送等级提升事件
        notifyLevelUp(oldLevel, current_level);
    }
}

// 设置玩家等级
void Player::setLevel(int newLevel) {
    if (newLevel > current_level) {
        int oldLevel = current_level;
        current_level = newLevel;
        
        // 更新升级所需经验
        exp_to_next_level = static_cast<int>(100 * std::pow(1.5f, current_level - 1));
        
        // 发送等级提升事件
        notifyLevelUp(oldLevel, current_level);
    }
}

// 发送等级提升事件
void Player::notifyLevelUp(int oldLevel, int newLevel) {
    std::shared_ptr<PlayerLevelUpEventData> data = 
        std::make_shared<PlayerLevelUpEventData>(oldLevel, newLevel, experience, exp_to_next_level);
    std::shared_ptr<GameEvent> event = 
        std::make_shared<GameEvent>(GameEventType::PLAYER_LEVEL_UP, data);
    EventManager::getInstance()->dispatchEvent(event);
}

// ���״̬�仯���߼����л�������
void Player::player_change() {

    // ����������ͷ�������������ƶ�
    if (this->leftpressed && this->moveLeft) {
        if (this->look_state % 2 == 1) {  // ���������֡���л�Ϊ��һ������
            this->look_state++;
            this->setTexture("character1/player_left1.png");
        }
        else {  // �����ż��֡���л�Ϊ�ڶ�������
            this->look_state++;
            this->setTexture("character1/player_left2.png");
        }
    }
    // ��������¼�ͷ�������������ƶ�
    else if (this->downpressed && this->moveDown) {
        if (this->look_state % 2 == 1) {  // ���������֡���л�Ϊ��һ������
            this->look_state++;
            this->setTexture("character1/player_down1.png");
        }
        else {  // �����ż��֡���л�Ϊ�ڶ�������
            this->look_state++;
            this->setTexture("character1/player_down2.png");
        }
    }
    // ��������ϼ�ͷ�������������ƶ�
    else if (this->uppressed && this->moveUp) {
        if (this->look_state % 2 == 1) {  // ���������֡���л�Ϊ��һ������
            this->look_state++;
            this->setTexture("character1/player_up1.png");
        }
        else {  // �����ż��֡���л�Ϊ�ڶ�������
            this->look_state++;
            this->setTexture("character1/player_up2.png");
        }
    }
    // ��������Ҽ�ͷ�������������ƶ�
    else if (this->rightpressed && this->moveRight) {
        if (this->look_state % 2 == 1) {  // ���������֡���л�Ϊ��һ������
            this->look_state++;
            this->setTexture("character1/player_right1.png");
        }
        else {  // �����ż��֡���л�Ϊ�ڶ�������
            this->look_state++;
            this->setTexture("character1/player_right2.png");
        }
    }
}
