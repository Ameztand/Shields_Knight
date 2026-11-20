#pragma once

#include "include/common.h"
#include "logic/stateMachine/IGameState.h"


#include <vector>

//大厅
class Lobby :public IGameState {
private:
    const GameStateKind ID= GameStateKind::Lobby;
    //ButtonPtr buttons;

public:
    void onEnter(StateContext& ctx)override;
    void onExit(StateContext& ctx) override;
    void onSuspend(StateContext& ctx) override;
    void onResume(StateContext& ctx) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataPu& out) override;
    GameStateKind getID() const override;
};

//选择难度
class Select :public IGameState {
private:
    const GameStateKind ID = GameStateKind::Select;
    //ButtonPtr buttons;

public:
    void onEnter(StateContext& ctx)override;
    void onExit(StateContext& ctx) override;
    void onSuspend(StateContext& ctx) override;
    void onResume(StateContext& ctx) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataPu& out) override;
    GameStateKind getID() const override;
};

//暂停
class Pause :public IGameState {
private:
    const GameStateKind ID = GameStateKind::Pause;
    //ButtonPtr buttons;

public:
    void onEnter(StateContext& ctx)override;
    void onExit(StateContext& ctx) override;
    void onSuspend(StateContext& ctx) override;
    void onResume(StateContext& ctx) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataPu& out) override;
    GameStateKind getID() const override;
};


