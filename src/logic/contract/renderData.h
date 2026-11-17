#pragma once

#include "include/common.h"
#include "include/buttonKind.h"
#include "include/gameStateKind.h"
#include "logic/buttonManager/button.h"
#include "logic/contract/renderDataPu.h"


#include <vector>

struct RenderData{
    std::array<std::vector<Button>, kStateCount> buttons_;//全部按钮
    std::vector<GameStateKind> gameSta_;//当前状态
    int gameStaDepth_ = 0;

    bool debugPanel_ = false;

    std::array<RenderDataEntity, kStateCount> entitys_;//全部实体
};
