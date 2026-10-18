#include "gameStateMachine.h"

#include "gameState.h"

void State::pushSta(Data& data, std::unique_ptr<IGameState> newState) {
    if (newState) {
        newState->onEnter(data);           // 入场调用 onEnter
        stack.push(std::move(newState));
    }
}

bool State::popSta(Data& data) {
    if (stack.empty()) {
        return true;//栈为空，无法弹出
    }

    stack.top()->onExit(data);          // 出场调用 onExit
    stack.pop();

    return stack.empty();
}

void State::replaceSta(Data& data, std::unique_ptr<IGameState> newState) {
    popSta(data);
    pushSta(data, std::move(newState));
}

IGameState* State::getState() {
    return stack.empty() ? nullptr : stack.top().get();
}

bool State::empty() {
    return stack.empty();
}

int State::getStaDepth() const {
    return (int)stack.size();
}

IGameState::GameSta State::getStaID()const {
    return (stack.empty()) ? IGameState::GameSta::null : stack.top()->getID();
}

void State::initGameSta(Data& data)
{
    while (!stack.empty()) {
        stack.pop();
    }
    pushSta(data, std::make_unique<Lobby>());
}
