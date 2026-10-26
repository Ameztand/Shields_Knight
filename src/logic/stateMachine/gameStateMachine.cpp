#include "gameStateMachine.h"

#include "gameState.h"

void State::pushSta(Data& data, std::unique_ptr<IGameState> newState) {
    if (newState) {
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

    return stack.empty();
}

void State::replaceSta(Data& data, std::unique_ptr<IGameState> newState) {
    if (!newState) return;
    popSta(data);
    pushSta(data, std::move(newState));
}

IGameState* State::getState() {
    return stack.empty() ? nullptr : stack.back().get();
}

const std::vector<IGameState::GameSta> State::getStaStack() const
{
    std::vector<IGameState::GameSta> stackID;
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

IGameState::GameSta State::getStaID()const {
    return (stack.empty()) ? IGameState::GameSta::null : stack.back()->getID();
}

void State::initGameSta(Data& data)
{
    while (!stack.empty()) {
        popSta(data);
    }
    pushSta(data, std::make_unique<Lobby>());
}
