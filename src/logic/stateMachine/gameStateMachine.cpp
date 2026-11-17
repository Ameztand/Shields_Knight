#include "gameStateMachine.h"

#include "logic/stateMachine/gameState/gameState.h"
#include "logic/stateMachine/gameState/puGameState.h"

void State::pushSta(Data& data, std::unique_ptr<IGameState> newState) {
    if (newState) {
        if (!stack.empty()) stack.back()->onSuspend(data);//被压栈
        newState->onEnter(data);           // 入场调用 onEnter
        stack.push_back(std::move(newState));
    }
}

bool State::popSta(Data& data) {
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

void State::replaceSta(Data& data, std::unique_ptr<IGameState> newState) {
    if (!newState) return;
    popSta(data);
    pushSta(data, std::move(newState));
}

IGameState* State::getState() {
    return stack.empty() ? nullptr : stack.back().get();
}

const std::vector<GameStateKind> State::getStaStack() const
{
    std::vector<GameStateKind> stackID;
    stackID.reserve(stack.size());
    for(const auto& sta : stack){
        stackID.push_back(sta->getID());
    }
    return stackID;
}

bool State::empty() const {
    return stack.empty();
}

int State::getStaDepth() const {
    return (int)stack.size();
}

GameStateKind State::getStaID()const {
    return (stack.empty()) ? GameStateKind::null : stack.back()->getID();
}

void State::initGameSta(Data& data)
{
    while (!stack.empty()) {
        popSta(data);
    }
    pushSta(data, std::make_unique<Lobby>());
}
