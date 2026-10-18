#pragma once

#include "common.h"
#include "../logic/buttonManager/buttonID.h"
#include "../logic/buttonManager/button.h"
#include "../logic/stateMachine/IGameState.h"

#include <vector>

struct RenderData{
    std::vector<Button> buttons;//所有按钮
};