#pragma once

#include "../IGameState.h"
#include "../../../include/common.h"
#include "../../gameData.h"
#include "../../entityManager/entityManager.h"

#include <vector>

class PuGame :public IGameState {
private:
    static constexpr IGameState::GameSta ID = IGameState::GameSta::PuGame;//关键词问题？

    PuGameData gameData_;

    EntityManager entityManager_;

public:

    const std::vector<Entity>& getEntitys() const;//临时直接返回

    void onEnter(Data& world)override;
    void onExit(Data& world) override;
    void onSuspend(Data& world) override;
    void onResume(Data& world) override;
    void tick(StateContext& ctx)override;
    void submitRender(RenderDataEntity& out) override;
    IGameState::GameSta getID() const override;
};
