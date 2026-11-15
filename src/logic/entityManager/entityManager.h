#pragma once

#include "entity.h"
#include "entityID.h"
#include "../../include/common.h"
//#include "../../include/type_render_data.h"
//#include "../../include/config_rect.h"
//#include "../stateMachine/IGameState.h"
#include "../dataStore.h"


#include <queue>
#include <vector>
#include <unordered_map>

class EntityManager {
private:
    std::vector<Entity> entitys_;//仅自己状态的实体
    std::queue<int> freeList_;//空位
    std::unordered_map<EntityID, int> setoff;//不知道怎么命名的映射表

public:
    void registerEntitys(const EntityID id);

    void deleteEntitys(const EntityID id);

    const std::vector<Entity>& commitEntitys() const;
    std::vector<Entity>& commitEntitys();//临时提高可修改数组，后续改为单个申请修改

    void tick(Data& data, double dt);

    void initEntityManager();
};
