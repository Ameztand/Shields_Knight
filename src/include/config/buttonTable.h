#pragma once

#include "include/common.h"
#include "include/buttonKind.h"
#include "include/gameStateKind.h"
#include "include/config/buttonTableRect.h"



#include <cstddef>//size_t
#include <array>

struct ButtonConfig {
    //不再const
    Rect area;
    ButtonKind kind;
	//const IGameState::GameSta belong;

};

//配置单状态按钮数组
inline constexpr std::array<ButtonConfig, 1> kLobbyButtons {{ 
    { lobbystartButton , ButtonKind::lobbystartButton }
}};
inline constexpr std::array<ButtonConfig, 3> kSelectButtons {{ 
    {selectLeftButton ,ButtonKind::selectLeftButton},
    {selectMiddleButton, ButtonKind::selectMiddleButton}, 
    {selectRightButton, ButtonKind::selectRightButton} 
}};
inline constexpr std::array<ButtonConfig, 3> kPauseButtons {{ 
    {pauseLeftButton, ButtonKind::pauseLeftButton},
    {pauseMiddleButton, ButtonKind::pauseMiddleButton}, 
    {pauseRightButton, ButtonKind::pauseRightButton} 
}};
inline constexpr std::array<ButtonConfig, 1> kPuGameButtons{ {
    { puGameTestButton, ButtonKind::puGameTestButton }
} };

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
    { kPuGameButtons.data(), kPuGameButtons.size() },            // PuGame  ← 改这行
}};
