#include"Inventory.h"

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
			// 发送物品添加事件
			notifyItemAdded(pair.second.first, 1);
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
		auto item_copy = item.GetCopy();
		package[index] = std::make_pair ( item_copy , 1 );
		is_updated = true;
		// 发送物品添加事件
		notifyItemAdded(item_copy, 1);
		return true;
	}

	return false;
}

bool Inventory::AddItem ( const Item& item , const int& add_num ) {
	int remaining = add_num;
	int total_added = 0;
	for (auto& pair : package) {
		if (pair.second.first->CanBeDepositTogether ( item )
			&& pair.second.second < item.max_count_in_one_grid) {
			int space_left = item.max_count_in_one_grid - pair.second.second;
			int to_add = std::min ( remaining , space_left );
			pair.second.second += to_add;
			total_added += to_add;
			// 发送物品添加事件
			notifyItemAdded(pair.second.first, to_add);
			remaining -= to_add;
			if (remaining <= 0) {
				is_updated = true;
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
		auto item_copy = item.GetCopy();
		package[index] = std::make_pair ( item_copy , to_add );
		++size;
		total_added += to_add;
		// 发送物品添加事件
		notifyItemAdded(item_copy, to_add);
		remaining -= to_add;
		if (remaining <= 0) {
			is_updated = true;
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
		if (it->second.second > remove_num) {
			it->second.second -= remove_num;
			// 发送物品移除事件（未完全移除）
			notifyItemRemoved(it->second.first, remove_num, false);
			return 0;
		}
		// 保存物品指针以便发送事件
		auto item = it->second.first;
		int remove_count = it->second.second;
		package.erase ( it );
		// 发送物品移除事件（完全移除）
		notifyItemRemoved(item, remove_count, true);
		return 1;
	}
	return -1;
}

bool Inventory::ClearGrid ( const int& position ) {
	auto it = package.find ( position );
	if (it != package.end ()) {
		// 保存物品指针以便发送事件
		auto item = it->second.first;
		int remove_count = it->second.second;
		package.erase ( it );
		is_updated = true;
		// 发送物品移除事件（完全移除）
		notifyItemRemoved(item, remove_count, true);
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
		int old_position = selected_position;
		selected_position = new_position;
		// 发送选中物品变化事件
		notifySelectedItemChanged(old_position, new_position);
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
