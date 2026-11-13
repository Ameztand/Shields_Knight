#pragma once

#include "entity.h"
#include "entityID.h"
#include "../../include/common.h"
//#include "../../include/type_render_data.h"
//#include "../../include/config_rect.h"
#include "../stateMachine/IGameState.h"
#include "../dataStore.h"


#include <array>
#include <vector>

class EntityManager {
private:
    std::array<std::vector<Entity>, kStateCount> entitys;//全部实体

public:
    void registerEntitys(const IGameState::GameSta nowSta, const EntityID id);

    void deleteEntitys(const IGameState::GameSta nowSta, const EntityID id);

    const std::array<std::vector<Entity>, kStateCount>& commitEntitys();

    void tick(Data& data, const IGameState::GameSta nowSta);

    void initEntityManager();
};
