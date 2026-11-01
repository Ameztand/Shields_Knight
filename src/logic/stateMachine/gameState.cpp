#include "gameState.h"

#include "../dataStore.h"
#include "../buttonManager/button.h"
#include "../buttonManager/buttonID.h"


namespace HandleInput{
    bool EscPop(Data& world)
    {
        using KeySta = IInputLayer::KeySta;
        bool temp = (world.getMsgData().Esc == KeySta::Falling);
        world.gameIntend.popSta = temp;
        return temp;
    }

    bool EscPuase(Data& world)
    {
        using KeySta = IInputLayer::KeySta;
        bool temp = (world.getMsgData().Esc == KeySta::Falling);
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

void Lobby::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx.world))return;

    //轮询判断
    for (const auto& btn : ctx.world.getButtonOf(getID())) {
        if (!btn.isClicked) continue;

        switch (btn.status.ID) {
            case ButtonID::lobbystartButton:
                //选择难度
                ctx.world.gameIntend.replaceSelect = true;
                break;
            default: break;
        }
    }
}

IGameState::GameSta Lobby::getID() const
{
    return ID;
}

void Select::onEnter(Data& data)
{
    //buttons.clear();
    //buttons.emplace_back(selectLeftButton, ButtonID::selectLeftButton);
    //buttons.emplace_back(selectMiddleButton, ButtonID::selectMiddleButton);
    //buttons.emplace_back(selectRightButton, ButtonID::selectRightButton);
}

void Select::onExit(Data& data)
{
    //ctx.renderData.isSelect = false;
}

void Select::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx.world))return;

    //轮询判断
    for (const auto& btn : ctx.world.getButtonOf(getID())) {
        if (!btn.isClicked) continue;

        switch (btn.status.ID) {
            case ButtonID::selectRightButton:
                //进入PU
                ctx.world.gameIntend.replacePuGame = true;
                break;
            default: break;
        }
    }
}

IGameState::GameSta Select::getID() const
{
    return ID;
}

void PuGame::onEnter(Data& data)
{
    //data.gameIntend.initGameData = true;
}

void PuGame::onExit(Data& data)
{
}

void PuGame::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPuase(ctx.world))return;

    //游戏逻辑
}

IGameState::GameSta PuGame::getID() const
{
    return ID;
}

void Pause::onEnter(Data& data)
{
    //buttons.clear();
    //buttons.emplace_back(pauseLeftButton, ButtonID::pauseLeftButton);
    //buttons.emplace_back(pauseMiddleButton, ButtonID::pauseMiddleButton);
    //buttons.emplace_back(pauseRightButton, ButtonID::pauseRightButton);
}

void Pause::onExit(Data& data)
{
}

void Pause::tick(StateContext& ctx)
{
    //退出
    if (HandleInput::EscPop(ctx.world))return;

    //轮询判断
    for (const auto& btn : ctx.world.getButtonOf(getID())) {
        if (!btn.isClicked) continue;

        switch (btn.status.ID) {
            case ButtonID::pauseLeftButton:
                //继续游戏
                ctx.world.gameIntend.popSta = true;
                break;

            case ButtonID::pauseMiddleButton:
                //重新开始
                ctx.world.gameIntend.restartPuGame = true;
                break;

            case ButtonID::pauseRightButton:
                //返回大厅
                ctx.world.gameIntend.backLobby = true;
                break;  
            default: break;
        }
    }
}

IGameState::GameSta Pause::getID() const
{
    return ID;
}
