#pragma once

#include "include/common.h"
#include "logic/stateMachine/IGameState.h"
#include "logic/gameData.h"
#include "logic/entity/entityStore.h"

#include <vector>

class PuGame :public IGameState {
private:
    static constexpr GameStateKind ID = GameStateKind::PuGame;//关键词问题？

    PuGameData gameData;

    EntityStore entityPool;

public:

    //const std::vector<Entity>& getEntitys() const;//临时直接返回

    void onEnter(Data& world)override;
    void onExit(Data& world) override;
    void onSuspend(Data& world) override;
    void onResume(Data& world) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataEntity& out) override;
    GameStateKind getID() const override;
};
