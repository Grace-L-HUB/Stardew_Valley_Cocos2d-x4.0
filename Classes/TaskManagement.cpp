#include "TaskManagement.h"
#include "EventManager.h"

TaskManagement::TaskManagement () {}

// ��������
void TaskManagement::createTask ( const Task& task ) {
	tasks.push_back ( task );
    EventManager::getInstance().publishTaskCreated(task.name, task.npcName);
}

// �����Ѿ����ܵ�����
void TaskManagement::AddAcceptTask ( const Task& task ) {
    acceptTasks.push_back ( task );
    EventManager::getInstance().publishTaskAccepted(task.name, task.npcName);
}

// ������񲢻�ý���
void TaskManagement::completeTask ( const std::string& task_name ) {
    // �����ѽ��ܵ�����
    auto it = std::find_if ( acceptTasks.begin () , acceptTasks.end () ,
                            [&task_name]( const Task& task ) {
                                return task.name == task_name;
                            } );

    // ����ҵ�������
    if (it != acceptTasks.end ()) {
        const Task& task = *it;

        // ���������������ӽ�Һ�/����Ʒ
        if (task.type == NPC_TASK || task.type == SYSTEM_TASK) {
            int oldGold = GoldAmount;
            GoldAmount += 400; // ����GoldAmount����ĳ�Ա����
            EventManager::getInstance().publishGoldChanged(oldGold, GoldAmount, "task_reward");
            for (const auto& requiredItem : task.requiredItems) {
                inventory->RemoveItem ( requiredItem , 1 ); // �Ƴ���Ʒ
            }
        }

        // �����NPC�������Ӷ�ӦNPC�ĺøж�
        if (task.type == NPC_TASK) {
            npc_relationship->increaseRelationship ( "player" , task.npcName , task.relationshipPoints );
        }
        // ����ǽ���������������NPC�ĺøжȣ����������⽱��
        else if (task.type == FESTIVAL_TASK) {
            for (const Item& item : task.specialRewards) {
                inventory->AddItem ( item ); // ����inventory����ĳ�Ա��������Ҫ������Ʒ
            }
            npc_relationship->increaseRelationship ( "player" , "Alex" , task.relationshipPoints );
            npc_relationship->increaseRelationship ( "player" , "Abigail" , task.relationshipPoints );
            npc_relationship->increaseRelationship ( "player" , "Caroline" , task.relationshipPoints );
            npc_relationship->increaseRelationship ( "player" , "Elliott" , task.relationshipPoints );
            npc_relationship->increaseRelationship ( "player" , "Emily" , task.relationshipPoints );
        }

        // ���ѽ��ܵ������б����Ƴ�����
        acceptTasks.erase ( it );
        EventManager::getInstance().publishTaskCompleted(task.name,
                                                         task.npcName,
                                                         task.rewardCoins,
                                                         task.relationshipPoints);
    }
    else {
        // ���û���ҵ������׳��쳣
        // throw std::runtime_error ( "Task not found in accepted tasks." );
    }
}


//������ܺ�ӷ���������ɾ��
void TaskManagement::DeleteAcceptTask ( const Task& task ) {
    // �� Tasks ��ɾ������
    tasks.erase ( std::remove_if ( tasks.begin () , tasks.end () ,
        [&task]( const Task& t ) {
            return t.name == task.name && t.npcName == task.npcName && t.type == task.type;  // ���ݶ����������ƥ��
        } ) ,
        tasks.end () );
}

// ������Ʒ��Ӧ������
std::string TaskManagement::findTaskByRequiredItems ( const std::string& itemName ) {
    // ����acceptTasksѰ�Ұ���ָ����Ʒ������
    for (const auto& task : acceptTasks) {
        for (const auto& item : task.requiredItems) {
            if (item.GetName() == itemName) {
                return task.name; // �ҵ�ƥ������񣬷�����������
            }
        }
    }

    // ���û���ҵ����׳��쳣
    // throw std::runtime_error ( "No task found with the required item." );
    return "";
}