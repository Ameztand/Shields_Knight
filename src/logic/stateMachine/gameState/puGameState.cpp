#include "puGameState.h"

#include "../../dataStore.h"
#include "../../buttonManager/button.h"
#include "../../buttonManager/buttonID.h"
//#include "../../entityManager/entity.h"
//#include "../../entityManager/entityID.h"

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

void PuGame::onEnter(Data& data)
{
    gameData_.initGameData();//或许需要世界信息？
}

void PuGame::onExit(Data& data)
{
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
}

IGameState::GameSta PuGame::getID() const
{
    return ID;
}
