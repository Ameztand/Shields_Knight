#pragma once

#include <vector>

//#include "../../logic/stateMachine/IGameState.h"
#include"../../input/IInputLayer.h"

class Data;   // 前置声明即可：这里只持有引用
//class IInputLayer::MsgData;
struct Button;

struct StateContext {
    Data& world;   // 暂时就是原来的 Data；R5 改名 GameData 并移进 State 持有
    const IInputLayer::MsgData& msgData;
    const std::vector<Button>& buttonList;
};
