#pragma once


#include "include/common.h"
#include "include/buttonKind.h"
#include "include/gameStateKind.h"
#include "include/config/buttonTable.h"
//#include "include/config/buttonTableRect.h"

#include "input/msgData.h"
#include "logic/button/button.h"
#include "logic/dataStore.h"
#include "logic/contract/renderData.h"


#include <array>
#include <vector>

class ButtonStore{
private:
    std::array<std::vector<Button>, kStateCount> buttonList;//全部按钮

public:
    void registerButtons(const GameStateKind nowSta);

    void deleteButtons(const GameStateKind nowSta);

    const std::array<std::vector<Button>, kStateCount>& commitButtons();

    void tick(Data& data, const GameStateKind nowSta);

    void initButtonStore();
};