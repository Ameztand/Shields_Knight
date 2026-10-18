#pragma once

#include "button.h"
#include "buttonID.h"
#include "../../include/common.h"
#include "../../include/type_render_data.h"
#include "../../include/config_rect.h"
#include "../../input/IInputLayer.h"
#include "../stateMachine/IGameState.h"
#include "../dataStore.h"


#include <array>
#include <vector>

class ButtonManager{
private:
    std::array<std::vector<Button>, kStateCount> buttons;//全部按钮

public:
    void registerButtons(const IGameState::GameSta nowSta);

    void deleteButtons(const IGameState::GameSta nowSta);

    const std::array<std::vector<Button>, kStateCount>& commitRenderData();

    void tick(Data& data, const IGameState::GameSta nowSta);

    void initButtonManager();
};