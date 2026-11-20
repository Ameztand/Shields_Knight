#pragma once

#include "include/common.h"
#include "logic/stateMachine/IGameState.h"
#include "logic/gameData.h"
#include "logic/entity/entityStore.h"
#include "logic/stateMachine/gameState/puGameStateLogic/battle.h"

#include <vector>

class PuGame :public IGameState {
private:
    static constexpr GameStateKind ID = GameStateKind::PuGame;//关键词问题？

    PuGameData gameData;
    EntityStore entityPool;
    Battle battle;

    void translateMsg(StateContext& ctx);

public:

    //const std::vector<Entity>& getEntitys() const;//临时直接返回

    void onEnter(StateContext& ctx)override;
    void onExit(StateContext& ctx) override;
    void onSuspend(StateContext& ctx) override;
    void onResume(StateContext& ctx) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataPu& out) override;
    GameStateKind getID() const override;
};
