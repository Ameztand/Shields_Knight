#pragma once  // 防止重复包含

#include "../include/common.h"

class IInputLayer{
public:
    enum class KeySta{
        Null,//悬空
        Falling,//按下
        DownS,//短按
        DownL,//长按
        Rising//松开
    };

    struct MsgData {
        KeySta Esc = KeySta::Null;
        KeySta F1 = KeySta::Null;
        KeySta Alt = KeySta::Null;

        bool isMouseF = false;//鼠标左键
        Position MousePos = Epos;
    };

    virtual ~IInputLayer() = default;

    //读取并更新内部状态
    virtual void poll() = 0;

    virtual const IInputLayer::MsgData& getMsgData() const = 0;
};