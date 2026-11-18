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

void ButtonStore::tick(Data& data, const GameStateKind nowSta){
    //轮询判断(只检查栈顶状态)
    size_t sta = (size_t)nowSta;
    auto& vec = buttonList[sta];
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
        btn.isClicked = btn.isHovered && (data.getMsgData().MouseL == KeySta::Falling);
    }
}

void ButtonStore::initButtonStore()
{
    registerButtons(GameStateKind::Lobby);
}
