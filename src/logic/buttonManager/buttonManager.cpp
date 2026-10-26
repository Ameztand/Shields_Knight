#include "buttonManager.h"

void ButtonManager::registerButtons(const IGameState::GameSta nowSta){
    size_t sta = (size_t)nowSta;
    auto& vec = buttons[sta];
    vec.clear();

    vec.reserve(kButtonTable[sta].size());
    for (const auto& btn : kButtonTable[sta]){
        vec.emplace_back(btn);
    }  
}

void ButtonManager::deleteButtons(const IGameState::GameSta nowSta)
{
    buttons[(size_t)nowSta].clear();
}

const std::array<std::vector<Button>, kStateCount> &ButtonManager::commitButtons()
{
    return buttons;
}

void ButtonManager::tick(Data& data, const IGameState::GameSta nowSta){
    //轮询判断(只检查栈顶状态)
    size_t sta = (size_t)nowSta;
    auto& vec = buttons[sta];
    for (auto& btn : vec) {
        //检查悬停
        bool tempHoveredSta = btn.status.area.contains(data.getMsgData().MousePos);
        if(tempHoveredSta){
            if(btn.isHovered){
                //悬停中
                if(data.getNow() - btn.hoverStartTime > btn.hoverThreshold){//悬停超过指定延迟时间
                    btn.isLongHover = true;
                }   
            }
            else{
                //悬停开始
                btn.hoverStartTime = data.getNow();
            }
        }else{
            //悬停结束
            btn.isLongHover = false;
        }
        btn.isHovered = tempHoveredSta;

        //检查按下
        btn.isClicked = btn.isHovered && (data.getMsgData().MouseL == IInputLayer::KeySta::Falling);
    }
}

void ButtonManager::initButtonManager()
{
    registerButtons(IGameState::GameSta::Lobby);
}
