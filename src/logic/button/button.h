#pragma once

#include "include/common.h"
//#include "../stateMachine/IGameState.h"
#include "include/buttonKind.h"
#include "include/config/buttonTable.h"//为了ButtonConfig



struct Button{
    ButtonConfig status;

    bool isClicked = false;//点击（短按）//下降沿
	bool isHovered = false;//悬停
    bool isLongPress = false;//长按
    bool isLongHover = false;//长悬停

    float hoverThreshold = 0.2f; // 长悬停时间阈值（秒）
    float hoverTime = 0; // 悬停时间（秒）

    // 保留默认构造（以防别的地方要空构造）
    Button() = default;

    // 【新增】接收配置的构造函数
    explicit Button(const ButtonConfig& config) : status(config) {}
};