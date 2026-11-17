#pragma once

#include "../IGameState.h"
#include "../../../include/common.h"

#include <vector>

//大厅
class Lobby :public IGameState {
private:
    const GameStateKind ID= GameStateKind::Lobby;
    //ButtonPtr buttons;

public:
    void onEnter(Data& world)override;
    void onExit(Data& world) override;
    void onSuspend(Data& world) override;
    void onResume(Data& world) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataEntity& out) override;
    GameStateKind getID() const override;
};

//选择难度
class Select :public IGameState {
private:
    const GameStateKind ID = GameStateKind::Select;
    //ButtonPtr buttons;

public:
    void onEnter(Data& world)override;
    void onExit(Data& world) override;
    void onSuspend(Data& world) override;
    void onResume(Data& world) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataEntity& out) override;
    GameStateKind getID() const override;
};

//暂停
class Pause :public IGameState {
private:
    const GameStateKind ID = GameStateKind::Pause;
    //ButtonPtr buttons;

public:
    void onEnter(Data& world)override;
    void onExit(Data& world) override;
    void onSuspend(Data& world) override;
    void onResume(Data& world) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataEntity& out) override;
    GameStateKind getID() const override;
};


