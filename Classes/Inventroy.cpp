#include "Inventory.h"
#include "EventManager.h"

Inventory::Inventory ( const int& size )
	:capacity ( size ) , selected_position ( -1 ) {};
Inventory::Inventory ( const Inventory& other )
	:capacity ( other.capacity ) , selected_position ( -1 ) , package ( other.package ) {};
Inventory::~Inventory () {}

bool Inventory::AddItem ( const Item& item ) {
	for (auto& pair : package) {
		if (pair.second.first->CanBeDepositTogether ( item )
			&& pair.second.second < item.max_count_in_one_grid) {
			++pair.second.second;
			is_updated = true;
            EventManager::getInstance().publishItemAdded(item.GetName(), 1, pair.first);
			return true;
		}
	}

	int size = package.size ();
	if (size < capacity) {
		int index = 0;
		for (int i = 1;; i++) {
			if (package.find ( i ) == package.end ()) {
				index = i;
				break;
			}
		}
		package[index] = std::make_pair ( item.GetCopy () , 1 );
		is_updated = true;
        EventManager::getInstance().publishItemAdded(item.GetName(), 1, index);
		return true;
	}

	return false;
}

bool Inventory::AddItem ( const Item& item , const int& add_num ) {
	int remaining = add_num;
	for (auto& pair : package) {
		if (pair.second.first->CanBeDepositTogether ( item )
			&& pair.second.second < item.max_count_in_one_grid) {
			int space_left = item.max_count_in_one_grid - pair.second.second;
			int to_add = std::min ( remaining , space_left );
			pair.second.second += to_add;
			remaining -= to_add;
			if (remaining <= 0) {
				is_updated = true;
                EventManager::getInstance().publishItemAdded(item.GetName(), to_add, pair.first);
				return true;
			}
		}
	}
	int size = package.size ();
	while (remaining > 0 && size < capacity) {
		int index = 0;
		for (int i = 1;; i++) {
			if (package.find ( i ) == package.end ()) {
				index = i;
				break;
			}
		}
		int to_add = std::min ( remaining , item.max_count_in_one_grid );
		package[index] = std::make_pair ( item.GetCopy () , to_add );
		++size;
		remaining -= to_add;
		if (remaining <= 0) {
			is_updated = true;
            EventManager::getInstance().publishItemAdded(item.GetName(), to_add, index);
			return true;
		}
	}
    is_updated = remaining < add_num ? true : false;
	return remaining <= 0;
}

int Inventory::RemoveItem ( const int& position , const int& remove_num ) {
	auto it = package.find ( position );
	if (it != package.end ()) {
		is_updated = true;
        const std::string itemName = it->second.first ? it->second.first->GetName() : "";
        const int removedCount = std::min(remove_num, it->second.second);
		if (it->second.second > remove_num) {
			it->second.second -= remove_num;
            EventManager::getInstance().publishItemRemoved(itemName, removedCount, position);
			return 0;
		}
		package.erase ( it );
        EventManager::getInstance().publishItemRemoved(itemName, removedCount, position);
		return 1;
	}
	return -1;
}

bool Inventory::ClearGrid ( const int& position ) {
	auto it = package.find ( position );
	if (it != package.end ()) {
		package.erase ( it );
		is_updated = true;
		return true;
	}
	return false;
}

std::shared_ptr<Item> Inventory::GetSelectedItem ()const {
	if (selected_position >= 1 && selected_position <= kRowSize) {
		auto it = package.find ( selected_position );
		if (it != package.end ()) {
			return it->second.first;
		}
	}
	return nullptr;
}

std::shared_ptr<Item> Inventory::GetSelectedItemCopy () {
	if (selected_position >= 1 && selected_position <= kRowSize) {
		auto it = package.find ( selected_position );
		if (it != package.end ()) {
			auto item_copy = it->second.first->GetCopy();
			return item_copy;
		}
	}
	return nullptr;
}

int Inventory::SetSelectedItem (const int new_position) {
	if (new_position == selected_position) {
		return 0;
	}
	auto it_previous = package.find ( selected_position );
	//��ԭ��`selected_position`λ�ô�����Ʒ�������Ϊunusable
	if (it_previous != package.end ()) {
		it_previous->second.first->SetUnusable ();
	}
	//ֻ��������Ʒ������е�ItemΪ��ǰѡ����Ʒ
	if (new_position >= 1 && new_position <= kRowSize) {
		selected_position = new_position;
		auto it_new = package.find ( new_position );
		//����λ����Item,������Ϊusable
		if (it_new != package.end () && it_new->second.first != nullptr) {
			it_new->second.first->SetUsable ();
			return 0;
		}
		return 1;
	}
	return -1;
}

void Inventory::DisplayPackageInCCLOG () {
	CCLOG ( "PACKAGE_INFO\n" );
	for (const auto& it : package) {
		int position = it.first;
		CCLOG ( "%s" , typeid(it.second.first).name() );
		std::string name = it.second.first->GetName ();
		int num = it.second.second;
		int value = it.second.first->GetValue ();
		CCLOG ( "pos: %d  name: %s num: %d value: %d\n" , position , name.c_str () , num , value );
	}
}
