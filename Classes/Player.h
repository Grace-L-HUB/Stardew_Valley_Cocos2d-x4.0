#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Inventory.h"
#include "Crop.h"
#include "AppDelegate.h"
#include "EventManager.h"
#include "GameEvent.h"

const int kDefaultEnergy = 100;

USING_NS_CC;

class Player : public cocos2d::Sprite
{
private:
    // 发送玩家位置变化事件
    void notifyPositionChanged(const cocos2d::Vec2& newPosition);
    
    // 发送玩家状态变化事件
    void notifyStateChanged(const std::string& state, bool value);

public:
    Player();
    ~Player();

    // ���� Player ����ķ���
    static Player* create();

    // ��ʼ����ɫ
    bool init();

    // ��������ʱ�����Ļص�����
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // �����ͷ�ʱ�����Ļص�����
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void player1_move();

    void player_change();


    bool moveLeft, moveDown, moveUp, moveRight;

    bool leftpressed = false, downpressed = false, uppressed = false, rightpressed = false;

    // ��ɫ���ƶ��ٶ�
    float speed;

    int look_state = 0;

    std::string pic_path;

    int energy_limit = kDefaultEnergy;
    int current_energy = kDefaultEnergy;
    
    // 等级相关属性
    int current_level = 1;
    int experience = 0;
    int exp_to_next_level = 100; // 升级所需经验
    
    // 设置玩家能量并发送事件
    void setEnergy(int newEnergy);
    
    // 获取玩家当前位置
    cocos2d::Vec2 getPosition() const;
    
    // 设置玩家经验
    void setExperience(int newExp);
    
    // 设置玩家等级
    void setLevel(int newLevel);
    
    // 发送等级提升事件
    void notifyLevelUp(int oldLevel, int newLevel);

};

#endif // __PLAYER_H__
