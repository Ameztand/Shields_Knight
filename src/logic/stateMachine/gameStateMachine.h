#pragma once

#include <memory>
#include <stack>

//#include "common.h"
#include "IGameState.h"



// ========== 状态机管理器 ==========
class State {
private:
    std::stack<std::unique_ptr<IGameState>> stack;

public:
    void pushSta(Data& data, std::unique_ptr<IGameState> newState);

    bool popSta(Data& data);

    void replaceSta(Data& data, std::unique_ptr<IGameState> newState);

    IGameState* getState();//获取当前状态机

    bool empty();

    int getStaDepth() const;

    IGameState::GameSta getStaID()const;

    void initGameSta(Data& data);
};