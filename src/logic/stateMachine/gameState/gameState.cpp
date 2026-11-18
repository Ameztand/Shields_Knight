#include "gameState.h"

#include "include/buttonKind.h"
#include "logic/dataStore.h"
#include "logic/button/button.h"



namespace HandleInput{
    bool EscPop(Data& world, const MsgData& msgData)
    {
        bool temp = (msgData.Esc == KeySta::Falling);
        world.gameIntend.popSta = temp;
        return temp;
    }

    bool EscPuase(Data& world, const MsgData& msgData)
    {
        bool temp = (msgData.Esc == KeySta::Falling);
        world.gameIntend.pushPause = temp;
        return temp;
    }
}

void Lobby::onEnter(Data& world)
{
    world.gameIntend.resetTime = true;
}

void Lobby::onExit(Data& world)
{
    
}

void Lobby::onSuspend(Data& world)
{

}

void Lobby::onResume(Data& world)
{
}

void Lobby::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx.world, ctx.msgData))return;

    //轮询判断
    for (const auto& btn : ctx.buttonList ) {
        if (!btn.isClicked) continue;

        switch (btn.status.kind) {
            case ButtonKind::lobbystartButton:
                //选择难度
                ctx.world.gameIntend.replaceSelect = true;
                break;
            default: break;
        }
    }
}

void Lobby::submitRender(RenderDataEntity& out)
{
}

GameStateKind Lobby::getID() const
{
    return ID;
}

void Select::onEnter(Data& data)
{
    //buttons.clear();
    //buttons.emplace_back(selectLeftButton, ButtonKind::selectLeftButton);
    //buttons.emplace_back(selectMiddleButton, ButtonKind::selectMiddleButton);
    //buttons.emplace_back(selectRightButton, ButtonKind::selectRightButton);
}

void Select::onExit(Data& data)
{
    //ctx.renderData.isSelect = false;
}

void Select::onSuspend(Data& world)
{
}

void Select::onResume(Data& world)
{
}

void Select::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx.world, ctx.msgData))return;

    //轮询判断
    for (const auto& btn : ctx.world.getButtonOf(getID())) {
        if (!btn.isClicked) continue;

        switch (btn.status.kind) {
            case ButtonKind::selectRightButton:
                //进入PU
                ctx.world.gameIntend.replacePuGame = true;
                break;
            default: break;
        }
    }
}

void Select::submitRender(RenderDataEntity& out)
{
}

GameStateKind Select::getID() const
{
    return ID;
}

void Pause::onEnter(Data& data)
{
    //buttons.clear();
    //buttons.emplace_back(pauseLeftButton, ButtonKind::pauseLeftButton);
    //buttons.emplace_back(pauseMiddleButton, ButtonKind::pauseMiddleButton);
    //buttons.emplace_back(pauseRightButton, ButtonKind::pauseRightButton);
}

void Pause::onExit(Data& data)
{
}

void Pause::onSuspend(Data& world)
{
}

void Pause::onResume(Data& world)
{
}

void Pause::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx.world, ctx.msgData))return;

    //轮询判断
    for (const auto& btn : ctx.world.getButtonOf(getID())) {
        if (!btn.isClicked) continue;

        switch (btn.status.kind) {
            case ButtonKind::pauseLeftButton:
                //继续游戏
                ctx.world.gameIntend.popSta = true;
                break;

            case ButtonKind::pauseMiddleButton:
                //重新开始
                ctx.world.gameIntend.restartPuGame = true;
                break;

            case ButtonKind::pauseRightButton:
                //返回大厅
                ctx.world.gameIntend.backLobby = true;
                break;  
            default: break;
        }
    }
}

void Pause::submitRender(RenderDataEntity& out)
{
}

GameStateKind Pause::getID() const
{
    return ID;
}
