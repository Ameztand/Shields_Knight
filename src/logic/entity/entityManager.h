#pragma once

#include <cstdint>//为了uint32_t

#include "include/common.h"
#include "logic/entity/entity.h"
//#include "logic/entity/entityKind.h"

//#include "../../include/type_render_data.h"
//#include "../../include/config_rect.h"
//#include "../stateMachine/IGameState.h"
//#include "../dataStore.h"


#include <queue>
#include <vector>
#include <unordered_map>

class EntityManager {
private:
    uint32_t id = 0;//实体对象id，只增不减

    std::vector<Entity> entitys_;//仅自己状态的实体
    std::queue<int> freeList_;//空位
    std::unordered_map<uint32_t, int> setoff;//不知道怎么命名的映射表

    int findIndex(const uint32_t id) const;

public:
    void registerEntitys(const EntityKind kind);
    void deleteEntitys(const uint32_t id);

    const std::vector<Entity>& commitEntitys() const;
    //template<class F> void forEachAlive(F&& f) const { for (auto& e : entitys_) if (e.kind != EntityKind::Null) f(e); }
    const Entity& getEntity(const uint32_t id) const;//getEntity 改成返回 const Entity*（找不到 nullptr）？？？减少哨兵Eentity?
    void setEntity(const Entity& ety, const uint32_t id);

    void updateHover(const uint32_t id, const double dt);
    void clearHover(const uint32_t id);
    void clearAllHover();

    void initEntityManager();
};
