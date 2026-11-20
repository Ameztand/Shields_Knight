#include "gameStateMachine.h"

#include "logic/stateMachine/gameState/gameState.h"
#include "logic/stateMachine/gameState/puGameState.h"

#include "logic/intendEvent.h"
#include "logic/button/button.h"
#include "input/msgData.h"

void GameStateMachine::pushSta(StateContext& ctx, std::unique_ptr<IGameState> newState) {
    if (newState) {
        if (!stack.empty()) stack.back()->onSuspend(ctx);//被压栈
        newState->onEnter(ctx);           // 入场调用 onEnter
        stack.push_back(std::move(newState));
    }
}

bool GameStateMachine::popSta(StateContext& ctx) {
    if (stack.empty()) {
        return true;//栈为空，无法弹出
    }

    stack.back()->onExit(ctx);          // 出场调用 onExit
    stack.pop_back();

    if (!stack.empty()) {
        stack.back()->onResume(ctx);//不再压栈
        return false;//栈不为空
    }

    return true;
}

void GameStateMachine::replaceSta(StateContext& ctx, std::unique_ptr<IGameState> newState) {
    if (!newState) return;
    popSta(ctx);
    pushSta(ctx, std::move(newState));
}

IGameState* GameStateMachine::getState() {
    return stack.empty() ? nullptr : stack.back().get();
}

const std::vector<GameStateKind> GameStateMachine::getStaStack() const
{
    std::vector<GameStateKind> stackID;
    stackID.reserve(stack.size());
    for(const auto& sta : stack){
        stackID.push_back(sta->getID());
    }
    return stackID;
}

bool GameStateMachine::empty() const {
    return stack.empty();
}

int GameStateMachine::getStaDepth() const {
    return (int)stack.size();
}

GameStateKind GameStateMachine::getStaID()const {
    return (stack.empty()) ? GameStateKind::Null : stack.back()->getID();
}

void GameStateMachine::initGameSta(StateContext& ctx)
{
    while (!stack.empty()) {
        popSta(ctx);
    }
    pushSta(ctx, std::make_unique<Lobby>());
}
