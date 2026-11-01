#include "gameState.h"

#include "../dataStore.h"
#include "../buttonManager/button.h"
#include "../buttonManager/buttonID.h"


namespace HandleInput{
    bool EscPop(Data& data)
    {
        using KeySta = IInputLayer::KeySta;
        bool temp = (data.getMsgData().Esc == KeySta::Falling);
        data.gameIntend.popSta = temp;
        return temp;
    }

    bool EscPuase(Data& data)
    {
        using KeySta = IInputLayer::KeySta;
        bool temp = (data.getMsgData().Esc == KeySta::Falling);
        data.gameIntend.pushPause = temp;
        return temp;
    }
}

void Lobby::onEnter(Data& data)
{
    data.gameIntend.resetTime = true;
}

void Lobby::onExit(Data& data)
{
    
}

void Lobby::tick(Data& data)
{
    //退出
    if (HandleInput::EscPop(data))return;

    //轮询判断
    for (const auto& btn : data.getButtonOf(getID())) {
        if (!btn.isClicked) continue;

        switch (btn.status.ID) {
            case ButtonID::lobbystartButton:
                //选择难度
                data.gameIntend.replaceSelect = true;
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

void Select::tick(Data& data)
{
    //退出
    if (HandleInput::EscPop(data))return;

    //轮询判断
    for (const auto& btn : data.getButtonOf(getID())) {
        if (!btn.isClicked) continue;

        switch (btn.status.ID) {
            case ButtonID::selectRightButton:
                //进入PU
                data.gameIntend.replacePuGame = true;
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

void PuGame::tick(Data& data)
{
    //退出
    if (HandleInput::EscPuase(data))return;

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

void Pause::tick(Data& data)
{
    //退出
    if (HandleInput::EscPop(data))return;

    //轮询判断
    for (const auto& btn : data.getButtonOf(getID())) {
        if (!btn.isClicked) continue;

        switch (btn.status.ID) {
            case ButtonID::pauseLeftButton:
                //继续游戏
                data.gameIntend.popSta = true;
                break;

            case ButtonID::pauseMiddleButton:
                //重新开始
                data.gameIntend.restartPuGame = true;
                break;

            case ButtonID::pauseRightButton:
                //返回大厅
                data.gameIntend.backLobby = true;
                break;  
            default: break;
        }
    }
}

IGameState::GameSta Pause::getID() const
{
    return ID;
}
