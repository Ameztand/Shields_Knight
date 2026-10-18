#pragma once

#include "IGameState.h"
#include "../../include/common.h"

#include <vector>

//大厅
class Lobby :public IGameState {
private:
    const IGameState::GameSta ID= IGameState::GameSta::Lobby;
    //ButtonPtr buttons;

public:
    void onEnter(Data& data)override;
    void onExit(Data& data) override;
    void tick(Data& data)override;
    IGameState::GameSta getID() const override;
};

//选择难度
class Select :public IGameState {
private:
    const IGameState::GameSta ID = IGameState::GameSta::Select;
    //ButtonPtr buttons;

public:
    void onEnter(Data& data)override;
    void onExit(Data& data) override;
    void tick(Data& data)override;
    IGameState::GameSta getID() const override;
};

//暂停
class Pause :public IGameState {
private:
    const IGameState::GameSta ID = IGameState::GameSta::Pause;
    //ButtonPtr buttons;

public:
    void onEnter(Data& data)override;
    void onExit(Data& data) override;
    void tick(Data& data)override;
    IGameState::GameSta getID() const override;
};

//模式一
class PuGame :public IGameState {
private:
    const IGameState::GameSta ID = IGameState::GameSta::PuGame;
    //ButtonPtr buttons;

public:
    void onEnter(Data& data)override;
    void onExit(Data& data) override;
    void tick(Data& data)override;
    IGameState::GameSta getID() const override;
};


