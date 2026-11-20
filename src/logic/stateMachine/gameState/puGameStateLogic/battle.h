#pragma once

//#include "logic/dataStore.h"
//#include "logic/contract/stateContext.h"
//#include "logic/contract/renderDataPu.h"

class GameData;
struct StateContext;
struct BattleContext;
struct RenderDataPu;


class Battle {
private:
public:
    void onEnter(StateContext& stateCtx);
    void onExit(StateContext& stateCtx);
    void tick(StateContext& stateCtx, BattleContext& battleCtx);
    void submitRender(RenderDataPu& out);
};