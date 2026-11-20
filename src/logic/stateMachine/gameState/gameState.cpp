#include "gameState.h"

#include "include/buttonKind.h"
#include "input/msgData.h"
#include "logic/button/button.h"
#include "logic/intendEvent.h"
#include "logic/contract/stateContext.h"



namespace HandleInput{
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

void Lobby::onEnter(StateContext& ctx)
{
    ctx.intend.resetTime = true;
}

void Lobby::onExit(StateContext& ctx)
{
    
}

void Lobby::onSuspend(StateContext& ctx)
{

}

void Lobby::onResume(StateContext& ctx)
{
}

void Lobby::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx))return;

    //轮询判断
    for (const auto& btn : ctx.buttonList ) {
        if (!btn.isClicked) continue;

        switch (btn.status.kind) {
            case ButtonKind::lobbystartButton:
                //选择难度
                ctx.intend.replaceSelect = true;
                break;
            default: break;
        }
    }
}

void Lobby::submitRender(RenderDataPu& out)
{
}

GameStateKind Lobby::getID() const
{
    return ID;
}

void Select::onEnter(StateContext& ctx)
{
    //buttons.clear();
    //buttons.emplace_back(selectLeftButton, ButtonKind::selectLeftButton);
    //buttons.emplace_back(selectMiddleButton, ButtonKind::selectMiddleButton);
    //buttons.emplace_back(selectRightButton, ButtonKind::selectRightButton);
}

void Select::onExit(StateContext& ctx)
{
    //ctx.renderData.isSelect = false;
}

void Select::onSuspend(StateContext& ctx)
{
}

void Select::onResume(StateContext& ctx)
{
}

void Select::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx))return;

    //轮询判断
    for (const auto& btn : ctx.buttonList) {
        if (!btn.isClicked) continue;

        switch (btn.status.kind) {
            case ButtonKind::selectRightButton:
                //进入PU
                ctx.intend.replacePuGame = true;
                break;
            default: break;
        }
    }
}

void Select::submitRender(RenderDataPu& out)
{
}

GameStateKind Select::getID() const
{
    return ID;
}

void Pause::onEnter(StateContext& ctx)
{
    //buttons.clear();
    //buttons.emplace_back(pauseLeftButton, ButtonKind::pauseLeftButton);
    //buttons.emplace_back(pauseMiddleButton, ButtonKind::pauseMiddleButton);
    //buttons.emplace_back(pauseRightButton, ButtonKind::pauseRightButton);
}

void Pause::onExit(StateContext& ctx)
{
}

void Pause::onSuspend(StateContext& ctx)
{
}

void Pause::onResume(StateContext& ctx)
{
}

void Pause::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx))return;

    //轮询判断
    for (const auto& btn : ctx.buttonList) {
        if (!btn.isClicked) continue;

        switch (btn.status.kind) {
            case ButtonKind::pauseLeftButton:
                //继续游戏
                ctx.intend.popSta = true;
                break;

            case ButtonKind::pauseMiddleButton:
                //重新开始
                ctx.intend.restartPuGame = true;
                break;

            case ButtonKind::pauseRightButton:
                //返回大厅
                ctx.intend.backLobby = true;
                break;  
            default: break;
        }
    }
}

void Pause::submitRender(RenderDataPu& out)
{
}

GameStateKind Pause::getID() const
{
    return ID;
}
