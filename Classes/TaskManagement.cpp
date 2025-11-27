#include "TaskManagement.h"
#include "EventManager.h"
#include "GameEvent.h"

TaskManagement::TaskManagement () {}

// ��������
void TaskManagement::createTask ( const Task& task ) {
	tasks.push_back ( task );
    
    // 发布任务创建事件
    auto eventData = std::make_shared<TaskEventData>(task.name, "created", task.description);
    auto event = std::make_shared<GameEvent>(GameEventType::TASK_CREATED, eventData, "TaskManagement");
    EventManager::getInstance()->dispatchEvent(event);
}

// �����Ѿ����ܵ�����
void TaskManagement::AddAcceptTask ( const Task& task ) {
    acceptTasks.push_back ( task );
    
    // 发布任务状态变更事件（从可接到已接）
    auto eventData = std::make_shared<TaskEventData>(task.name, "accepted", task.description);
    auto event = std::make_shared<GameEvent>(GameEventType::TASK_STATUS_CHANGED, eventData, "TaskManagement");
    EventManager::getInstance()->dispatchEvent(event);
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
            // 注意：这里应该通过EconomicSystem修改金钱，而不是直接修改全局变量
            // 但为了保持原有逻辑，暂时保留
            GoldAmount += 400; // ����GoldAmount��Ϊ��ȫ�ֱ���
            
            // 发布金钱变化事件
            auto goldEventData = std::make_shared<GoldChangedEventData>(GoldAmount - 400, GoldAmount);
            auto goldEvent = std::make_shared<GameEvent>(GameEventType::GOLD_AMOUNT_CHANGED, goldEventData, "TaskManagement");
            EventManager::getInstance()->dispatchEvent(goldEvent);
            
            for (const auto& requiredItem : task.requiredItems) {
                if (inventory) {
                    inventory->RemoveItem ( requiredItem , 1 ); // �Ƴ���Ʒ
                }
            }
        }

        // �����NPC�������Ӷ�ӦNPC�ĺøж�
        if (task.type == NPC_TASK && npc_relationship) {
            npc_relationship->increaseRelationship ( "player" , task.npcName , task.relationshipPoints );
        }
        // ����ǽ���������������NPC�ĺøжȣ����������⽱��
        else if (task.type == FESTIVAL_TASK) {
            for (const Item& item : task.specialRewards) {
                if (inventory) {
                    inventory->AddItem ( item ); // ����inventory���ӻ���
                }
            }
            if (npc_relationship) {
                npc_relationship->increaseRelationship ( "player" , "Alex" , task.relationshipPoints );
                npc_relationship->increaseRelationship ( "player" , "Abigail" , task.relationshipPoints );
                npc_relationship->increaseRelationship ( "player" , "Caroline" , task.relationshipPoints );
                npc_relationship->increaseRelationship ( "player" , "Elliott" , task.relationshipPoints );
                npc_relationship->increaseRelationship ( "player" , "Emily" , task.relationshipPoints );
            }
        }

        // 发布任务完成事件
        auto eventData = std::make_shared<TaskEventData>(task.name, "completed", task.description);
        auto event = std::make_shared<GameEvent>(GameEventType::TASK_COMPLETED, eventData, "TaskManagement");
        EventManager::getInstance()->dispatchEvent(event);

        // ���ѽ��ܵ������б����Ƴ�����
        acceptTasks.erase ( it );
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
    
    // 发布任务状态变更事件（任务被删除）
    auto eventData = std::make_shared<TaskEventData>(task.name, "deleted", task.description);
    auto event = std::make_shared<GameEvent>(GameEventType::TASK_STATUS_CHANGED, eventData, "TaskManagement");
    EventManager::getInstance()->dispatchEvent(event);
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