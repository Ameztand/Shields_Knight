#pragma once

#include "common.h"
#include "type_render_entity.h"
#include "../logic/buttonManager/buttonID.h"
#include "../logic/buttonManager/button.h"
#include "../logic/stateMachine/IGameState.h"

#include <vector>

struct RenderData{
    std::array<std::vector<Button>, kStateCount> buttons_;//全部按钮
    std::vector<IGameState::GameSta> gameSta_;//当前状态
    int gameStaDepth_ = 0;

    bool debugPanel_ = false;

    std::array<RenderDataEntity, kStateCount> entitys_;//全部实体
};
