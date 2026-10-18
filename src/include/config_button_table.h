#pragma once

#include "common.h"
#include "config_rect.h"
#include "../logic/stateMachine/IGameState.h"
#include "../logic/buttonManager/button.h"
#include "../logic/buttonManager/buttonID.h"

#include <cstddef>//size_t
#include <array>

//配置单状态按钮数组
inline constexpr std::array<ButtonConfig, 1> kLobbyButtons {{ 
    { lobbystartButton , ButtonID::lobbystartButton }
}};
inline constexpr std::array<ButtonConfig, 3> kSelectButtons {{ 
    {selectLeftButton ,ButtonID::selectLeftButton},
    {selectMiddleButton, ButtonID::selectMiddleButton}, 
    {selectRightButton, ButtonID::selectRightButton} 
}};
inline constexpr std::array<ButtonConfig, 3> kPauseButtons {{ 
    {pauseLeftButton, ButtonID::pauseLeftButton},
    {pauseMiddleButton, ButtonID::pauseMiddleButton}, 
    {pauseRightButton, ButtonID::pauseRightButton} 
}};

//按钮矩阵
struct ButtonRange {
    const ButtonConfig* data_ = nullptr;
    size_t count_  = 0;

    const size_t size() const { return count_ ; }

    //配合for循环使用
    const ButtonConfig* begin() const { return data_; }
    const ButtonConfig* end() const { return data_ + count_ ; }
    bool empty() const { return count_  == 0; }
};

//按钮对照表
inline constexpr std::array<ButtonRange, kStateCount> kButtonTable {{
    {},                                          // null
    { kLobbyButtons.data(),  kLobbyButtons.size()  },  // Lobby
    { kSelectButtons.data(), kSelectButtons.size() },  // Select
    { kPauseButtons.data(),  kPauseButtons.size()  },  // Pause
    {},                                          // PuGame
}};
