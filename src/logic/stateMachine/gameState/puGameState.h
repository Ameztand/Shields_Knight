#pragma once

#include "../IGameState.h"
#include "../../../include/common.h"
#include "../../gameData.h"

#include <vector>

class PuGame :public IGameState {
private:
    const IGameState::GameSta ID = IGameState::GameSta::PuGame;

    PuGameData gameData_;

public:


    void onEnter(Data& world)override;
    void onExit(Data& world) override;
    void onSuspend(Data& world) override;
    void onResume(Data& world) override;
    void tick(StateContext& ctx)override;
    IGameState::GameSta getID() const override;
};
