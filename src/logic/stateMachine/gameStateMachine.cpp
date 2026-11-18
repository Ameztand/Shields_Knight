#include "gameStateMachine.h"

#include "logic/stateMachine/gameState/gameState.h"
#include "logic/stateMachine/gameState/puGameState.h"

void GameStateMachine::pushSta(Data& data, std::unique_ptr<IGameState> newState) {  
    if (newState) {
        if (!stack.empty()) stack.back()->onSuspend(data);//被压栈
        newState->onEnter(data);           // 入场调用 onEnter
        stack.push_back(std::move(newState));
    }
}

bool GameStateMachine::popSta(Data& data) {
    if (stack.empty()) {
        return true;//栈为空，无法弹出
    }

    stack.back()->onExit(data);          // 出场调用 onExit
    stack.pop_back();

    if (!stack.empty()) {
        stack.back()->onResume(data);//不再压栈
        return false;//栈不为空
    }

    return true;
}

void GameStateMachine::replaceSta(Data& data, std::unique_ptr<IGameState> newState) {
    if (!newState) return;
    popSta(data);
    pushSta(data, std::move(newState));
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

void GameStateMachine::initGameSta(Data& data)
{
    while (!stack.empty()) {
        popSta(data);
    }
    pushSta(data, std::make_unique<Lobby>());
}
