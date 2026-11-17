#pragma once

#include "button.h"
#include "buttonID.h"
#include "../../include/common.h"
#include "../../include/type_render_data.h"
#include "../../include/config_rect.h"
#include "../../input/msgData.h"
#include "../stateMachine/gameStateKind.h"
#include "../dataStore.h"


#include <array>
#include <vector>

class ButtonManager{
private:
    std::array<std::vector<Button>, kStateCount> buttons;//全部按钮

public:
    void registerButtons(const GameStateKind nowSta);

    void deleteButtons(const GameStateKind nowSta);

    const std::array<std::vector<Button>, kStateCount>& commitButtons();

    void tick(Data& data, const GameStateKind nowSta);

    void initButtonManager();
};