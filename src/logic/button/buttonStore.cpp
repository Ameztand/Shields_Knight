#include "buttonStore.h"

void ButtonStore::registerButtons(const GameStateKind nowSta){
    size_t sta = (size_t)nowSta;
    auto& vec = buttonList[sta];
    //vec.clear();

    vec.reserve(kButtonTable[sta].size());
    for (const auto& btn : kButtonTable[sta]){
        vec.emplace_back(btn);
    }  
}

void ButtonStore::deleteButtons(const GameStateKind nowSta)
{
    buttonList[(size_t)nowSta].clear();
}

const std::array<std::vector<Button>, kStateCount> &ButtonStore::commitButtons()
{
    return buttonList;
}

void ButtonStore::tick(const GameStateKind nowSta, const MsgData& msgData, const float dt){
    //轮询判断(只检查栈顶状态)
    size_t sta = (size_t)nowSta;
    auto& vec = buttonList[sta];
    for (auto& btn : vec) {
        //检查悬停
        bool tempHoveredSta = btn.status.area.contains(msgData.MousePos);
        btn.isHovered = tempHoveredSta;
        if(tempHoveredSta){
            btn.hoverTime += dt;
            btn.isLongHover = (btn.hoverTime > btn.hoverThreshold);
        }else{
            btn.isLongHover = false;
            btn.hoverTime = 0;
        }

        //检查按下
        btn.isClicked = btn.isHovered && (msgData.MouseL == KeySta::Falling);
    }
}

void ButtonStore::initButtonStore()
{
    registerButtons(GameStateKind::Lobby);
}
