#pragma once

#include "../../include/common.h"
//#include "../stateMachine/IGameState.h"
#include "buttonID.h"

struct ButtonConfig {
    //不再const
    Rect area;
    ButtonID ID;
	//const IGameState::GameSta belong;

};

struct Button{
    ButtonConfig statu;

    bool isClicked = false;//点击（短按）//下降沿
	bool isHovered = false;//悬停
    bool isLongPress = false;//长按
    bool isLongHover = false;//长悬停

    double hoverThreshold = 200; // 长悬停时间阈值（毫秒）
    double hoverStartTime = 0; // 悬停时间（毫秒）

    // 保留默认构造（以防别的地方要空构造）
    Button() = default;

    // 【新增】接收配置的构造函数
    Button(const ButtonConfig& config) : statu(config) {}
};