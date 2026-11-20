#pragma once

#include <memory>
//#include <stack>
#include <vector>

//#include "common.h"
#include "IGameState.h"
//#include "logic/contract/stateContext.h"
struct StateContext;



// ========== 状态机管理器 ==========
class GameStateMachine {
private:
    std::vector<std::unique_ptr<IGameState>> stack;

public:
    void pushSta(StateContext& ctx, std::unique_ptr<IGameState> newState);

    bool popSta(StateContext& ctx);

    void replaceSta(StateContext& ctx, std::unique_ptr<IGameState> newState);

    IGameState* getState();//获取当前状态机

    const std::vector<GameStateKind> getStaStack() const;

    bool empty() const;

    int getStaDepth() const;

    GameStateKind getStaID()const;

    void initGameSta(StateContext& ctx);
};