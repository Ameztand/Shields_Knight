#include "puGameState.h"

#include <iostream>
#include "logic/button/button.h"
#include "logic/contract/battleContext.h"
#include "logic/intendEvent.h"
#include "input/msgData.h"
#include "logic/contract/stateContext.h"
#include "logic/contract/renderDataPu.h"


namespace HandleInput2 {
    bool EscPop(StateContext& ctx)
    {
        bool temp = (ctx.msgData.Esc == KeySta::Falling);
        ctx.intend.popSta = temp;
        return temp;
    }

    bool EscPuase(StateContext& ctx)
    {
        bool temp = (ctx.msgData.Esc == KeySta::Falling);
        ctx.intend.pushPause = temp;
        return temp;
    }
}
//目前这个HandleInput是直接复制粘贴，考虑提炼utils？

//const std::vector<Entity>& PuGame::getEntitys() const
//{
//    return entityStore_.commitEntitys();
//}

void PuGame::translateMsg(StateContext& ctx)
{
    gameData.F1_.update(ctx.msgData.F1);
    gameData.Alt_.update(ctx.msgData.Alt);
}

void PuGame::onEnter(StateContext& ctx)
{
    gameData.initGameData();//或许需要世界信息？
    entityPool.initEntityStore();
    entityPool.registerEntitys(EntityKind::testEntity);
    battle.onEnter(ctx);
}

void PuGame::onExit(StateContext& ctx)
{
    //测试
    //entityStore_.deleteEntitys(EntityKind::testEntity);
}

void PuGame::onSuspend(StateContext& ctx)
{
    int temp = gameData.getCnt();
    temp /= 2;
    gameData.setCnt(temp);
    //printf("触发onSuspend，cnt减半，及cnt = %d\n", temp);//不知道为什么用不了，只能用endl强行刷新
    std::cout << "onSuspend: cnt = " << gameData.getCnt() << std::endl;   // ← 换成 cout + endl
}

void PuGame::onResume(StateContext& ctx)
{
    int temp = gameData.getCnt();
    temp += 1;
    gameData.setCnt(temp);
    //printf("触发onResume，cnt加一，及cnt = %d\n", temp);
    std::cout << "onResume: cnt = " << gameData.getCnt() << std::endl;
}

void PuGame::tick(StateContext& ctx)
{
    //退出
    if (HandleInput2::EscPuase(ctx))return;

    BattleContext battleCtx{ gameData, entityPool };
    battle.tick(ctx, battleCtx);
}

void PuGame::submitRender(RenderDataPu& out)
{
    //还没跳过墓碑
    out.entitys_ = entityPool.commitEntitys();
    out.selectedID_ = gameData.getID();
    //out.selectedHoverID_ = gameData_.getHoverID();
    //if (gameData_.getHoverID() != 0) out.loogTimeHover_ = entityStore_.getEntity(gameData_.getHoverID()).isLongHover;
}

GameStateKind PuGame::getID() const
{
    return ID;
}
