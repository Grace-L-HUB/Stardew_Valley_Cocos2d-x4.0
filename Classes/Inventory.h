#pragma once
#include <algorithm>
#include <memory>
#include <map>
#include "Item.h"
#include "EventManager.h"
#include "GameEvent.h"

const int kRowSize = 12;
const int kDefaultSize = kRowSize * 2;

class Inventory {
private:
	//��������
	int capacity;

	//��ǰѡ�е���Ʒλ��
	int selected_position;

	//map�洢����Ʒ�ڱ����е�λ����Ϊkey�������Ͻ�Ϊ1���������ҡ����ϵ��µ�����
	//std::pair�洢��Ӧ��Itemָ��(ʹ��shared_ptr)�ͱ�����λ��/�����ж����ŵĸ�����Ʒ��������ľͷ��99��ʾ�����еĸ�һ�������д洢��99��ľͷ��
	//������һ�������д�ŵ�Item��ֻ��Ӧһ��shared_ptr
	std::map<int , std::pair<std::shared_ptr<Item> , int>> package;

	// 事件通知方法
	void notifyItemAdded(std::shared_ptr<Item> item, int count) {
        auto data = std::make_shared<ItemAddedEventData>(item, count);
        auto event = std::make_shared<GameEvent>(GameEventType::ITEM_ADDED, data);
        EventManager::getInstance()->dispatchEvent(event);
    }

    void notifyItemRemoved(std::shared_ptr<Item> item, int count, bool isCompletelyRemoved = false) {
        auto data = std::make_shared<ItemRemovedEventData>(item, count, isCompletelyRemoved);
        auto event = std::make_shared<GameEvent>(GameEventType::ITEM_REMOVED, data);
        EventManager::getInstance()->dispatchEvent(event);
    }

	void notifySelectedItemChanged(int oldPosition, int newPosition) {
		std::shared_ptr<SelectedItemChangedEventData> data = std::make_shared<SelectedItemChangedEventData>(oldPosition, newPosition);
		std::shared_ptr<GameEvent> event = std::make_shared<GameEvent>(GameEventType::SELECTED_ITEM_CHANGED, data);
		EventManager::getInstance()->dispatchEvent(event);
	}

public:
	Inventory ( const int& size = kDefaultSize );

	Inventory ( const Inventory& other );

	~Inventory ();

	bool is_updated = false;//��ⱳ���Ƿ���¹�

	//���ɹ�����ʱ����true,����ʧ��ʱ����false
bool AddItem ( const Item& item );

	//���ɹ���������`add_num`��itemʱ����true,���򷵻�false
bool AddItem ( const Item& item , const int& add_num );

	//�Ƴ�`remove_num`���ڱ�����`position`λ�õ���Ʒ
	//��`remove_num`�����ø�����������Ʒ������������ոø���
	//����ֵ��
	//������`position`λ�ô�δ������Ʒʱ����-1
	//`remove_num`�����ø�����������Ʒ������������ոø��ӣ�������1
	//�����Ƴ�������`position`����`remove_num`����Ʒʱ������0;
	int RemoveItem ( const int& position , const int& remove_num = 1 );

	// ʹ�� Item �������Ƴ���Ʒ  
	int RemoveItem ( const Item& item , const int& remove_num = 1 );

	//��ձ�����`position`λ�õĸ���
	bool ClearGrid ( const int& position );

	//��ȡ`selected_position`��Item
	//δ�ҵ��򷵻�nullptr
	std::shared_ptr<Item> GetSelectedItem ()const;

	//��ȡ`selected_position`��Item�ĸ�����������������ֲ����Ҫ��ȡ�����ͬItemʵ���ĳ���)
	//δ�ҵ��򿽱�ʧ���򷵻�nullptr
	std::shared_ptr<Item> GetSelectedItemCopy ();

	//��ȡָ��λ���е���Ʒ
	std::shared_ptr<Item> GetItemAt ( int position ) const;

	// ��ȡָ��λ���е���Ʒ����  
	int GetItemCountAt ( int position ) const;

	// ��ȡ������ָ�����Ƶ� Item  
	Item GetItemByName ( const std::string& itemName ) const;

	//��`new_position`�Ϸ�ʱ����`selected_position`����Ϊ`new_position`
	// �Ϸ���`new_position`Ӧ��Ϊ��[1,kRowSize]���������ֻ��������Ʒ���һ����λ��Ϊ`selected_position`
	//��ԭ`selected_position`����Item��������Ϊunusable״̬
	//��`new_position`<1 || `new_position`>kRowSize ����-1
	//��`new_position`����Item,������Ϊusable״̬������0
	//��`new_position`����Item,����-1
	int SetSelectedItem(const int new_position);

	//�����̨���Package��Ϣ�������ڵ���
	void DisplayPackageInCCLOG ();

	Inventory& operator=( const Inventory& other );
};