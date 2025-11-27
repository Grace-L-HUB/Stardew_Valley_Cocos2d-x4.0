#include "Player.h"

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

    // 先保存当前能量值


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
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW && !downpressed)  // �¼�ͷ
    {
        downpressed = true;  // ����¼��Ѱ���
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW && !leftpressed)  // ���ͷ
    {
        leftpressed = true;  // �������Ѱ���
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && !rightpressed)  // �Ҽ�ͷ
    {
        rightpressed = true;  // ����Ҽ��Ѱ���
    }
}

void Player::setEnergy(int energy) {
    // 确保能量值在有效范围内
    current_energy = std::max(0, std::min(energy, energy_limit));
    
    // 发布能量变化事件
    EventManager::getInstance().publishPlayerEnergyChanged(current_energy);
}

void Player::changeEnergy(int delta) {
    // 先保存当前能量值
    int oldEnergy = current_energy;
    
    // 改变能量值并限制在有效范围内
    current_energy = std::max(0, std::min(current_energy + delta, energy_limit));
    
    // 如果能量值确实发生了变化，发布事件
    if (current_energy != oldEnergy) {
        EventManager::getInstance().publishPlayerEnergyChanged(current_energy);
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
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)  // �¼�ͷ
    {
        this->look_state = 0;  // ��λ look_state ״̬
        this->setTexture("character1/player_down3.png");  // ������ҳ��µ�ͼƬ
        this->pic_path = "character1/player_down3.png";
        downpressed = false;  // ����¼����ɿ�
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)  // ���ͷ
    {
        this->look_state = 0;  // ��λ look_state ״̬
        this->setTexture("character1/player_left3.png");  // ������ҳ����ͼƬ
        this->pic_path = "character1/player_left3.png";
        leftpressed = false;  // ���������ɿ�
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)  // �Ҽ�ͷ
    {
        this->look_state = 0;  // ��λ look_state ״̬
        this->setTexture("character1/player_right3.png");  // ������ҳ��ҵ�ͼƬ
        this->pic_path = "character1/player_right3.png";
        rightpressed = false;  // ����Ҽ����ɿ�
    }
}

// ����ƶ����߼�
void Player::player1_move() {

    // ����������ͷ�������������ƶ�
    if (this->leftpressed && this->moveLeft) {
        if (this->look_state == 0) {
            this->look_state++;  // �����Ҿ�ֹ״̬������״̬
            return;
        }
        this->setPositionX(this->getPositionX() - speed);  // �����ƶ�
    }
    // ��������¼�ͷ�������������ƶ�
    else if (this->downpressed && this->moveDown) {
        if (this->look_state == 0) {
            this->look_state++;  // �����Ҿ�ֹ״̬������״̬
            return;
        }
        this->setPositionY(this->getPositionY() - speed);  // �����ƶ�
    }
    // ��������ϼ�ͷ�������������ƶ�
    else if (this->uppressed && this->moveUp) {
        if (this->look_state == 0) {
            this->look_state++;  // �����Ҿ�ֹ״̬������״̬
            return;
        }
        this->setPositionY(this->getPositionY() + speed);  // �����ƶ�
    }
    // ��������Ҽ�ͷ�������������ƶ�
    else if (this->rightpressed && this->moveRight) {
        if (this->look_state == 0) {
            this->look_state++;  // �����Ҿ�ֹ״̬������״̬
            return;
        }
        this->setPositionX(this->getPositionX() + speed);  // �����ƶ�
    }
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
