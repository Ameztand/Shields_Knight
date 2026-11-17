#pragma once  // 防止重复包含

#include "include/common.h"

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

    KeySta W = KeySta::Null;      // ← 加 4 个
    KeySta A = KeySta::Null;
    KeySta S = KeySta::Null;
    KeySta D = KeySta::Null;

    KeySta MouseL = KeySta::Null;//鼠标左键
    Position MousePos = Epos;

    static constexpr bool held(KeySta s) {
    return s != KeySta::Null && s != KeySta::Rising;
}
};

