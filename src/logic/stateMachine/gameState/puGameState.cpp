#include "puGameState.h"

#include "../../dataStore.h"
#include "../../buttonManager/button.h"
#include "../../buttonManager/buttonID.h"

#include <iostream>

namespace HandleInput2 {
    bool EscPop(Data& world, const IInputLayer::MsgData& msgData)
    {
        using KeySta = IInputLayer::KeySta;
        bool temp = (msgData.Esc == KeySta::Falling);
        world.gameIntend.popSta = temp;
        return temp;
    }

    bool EscPuase(Data& world, const IInputLayer::MsgData& msgData)
    {
        using KeySta = IInputLayer::KeySta;
        bool temp = (msgData.Esc == KeySta::Falling);
        world.gameIntend.pushPause = temp;
        return temp;
    }
}
//目前这个HandleInput是直接复制粘贴，考虑提炼utils？

const std::vector<Entity>& PuGame::getEntitys() const
{
    return entityManager_.commitEntitys();
}

void PuGame::onEnter(Data& data)
{
    gameData_.initGameData();//或许需要世界信息？
    entityManager_.initEntityManager();
    entityManager_.registerEntitys(EntityID::testEntity);
}

void PuGame::onExit(Data& data)
{
    //测试
    entityManager_.deleteEntitys(EntityID::testEntity);
}

void PuGame::onSuspend(Data& world)
{
    int temp = gameData_.getCnt();
    temp /= 2;
    gameData_.setCnt(temp);
    //printf("触发onSuspend，cnt减半，及cnt = %d\n", temp);//不知道为什么用不了，只能用endl强行刷新
    std::cout << "onSuspend: cnt = " << gameData_.getCnt() << std::endl;   // ← 换成 cout + endl
}

void PuGame::onResume(Data& world)
{
    int temp = gameData_.getCnt();
    temp += 1;
    gameData_.setCnt(temp);
    //printf("触发onResume，cnt加一，及cnt = %d\n", temp);
    std::cout << "onResume: cnt = " << gameData_.getCnt() << std::endl;
}

void PuGame::tick(StateContext& ctx)
{
    //退出
    if (HandleInput2::EscPuase(ctx.world, ctx.msgData))return;

    //游戏逻辑

    //按钮判断
    for (const auto& btn : ctx.buttonList) {          // ← 用新路径
        if (!btn.isClicked) continue;
        switch (btn.status.ID) {
        case ButtonID::puGameTestButton:
        {
            int temp = gameData_.getCnt();
            std::cout << "test , cnt = " << temp <<  ", now = " << ctx.world.getNow()/1000 << std::endl;
            temp++;
            gameData_.setCnt(temp);

            break;
        }
        default: break;
        }
    }

    //测试方块判定
    for (auto& ety : entityManager_.commitEntitys()) {
        if (ety.ID == EntityID::testEntity)
        {
            const auto& msgData = ctx.msgData;
            float dx = 0.f;
            float dy = 0.f;
            if (IInputLayer::held(msgData.W)) dy -= 1.f;
            if (IInputLayer::held(msgData.S)) dy += 1.f;
            if (IInputLayer::held(msgData.A)) dx -= 1.f;
            if (IInputLayer::held(msgData.D)) dx += 1.f;

            if (dx != 0.f || dy != 0.f) {
                const float len = std::sqrt(dx * dx + dy * dy);
                dx /= len;
                dy /= len;                           // 斜走不加速
            }

            ety.vel = { dx * ety.speed, dy * ety.speed };
            ety.updateMove(ctx.world.getDeltaSec());                    // 只传"时长"
        }
    }
}

void PuGame::submitRender(RenderDataEntity& out)
{
    out.entitys_ = entityManager_.commitEntitys();
}

IGameState::GameSta PuGame::getID() const
{
    return ID;
}
