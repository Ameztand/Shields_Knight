#pragma once  // 防止重复包含

#include <optional>

#include <SFML/System/Vector2.hpp>

#include "../include/common.h"
#include "../include/screen.h"      // Screen

namespace sf { class Event; }      // 只当参数类型用，前置声明足够

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

        KeySta MouseL = KeySta::Null;//鼠标左键
        Position MousePos = Epos;
    };

    virtual ~IInputLayer() = default;

    virtual void HandleEvent(const Screen& screen, std::optional<sf::Vector2i> mousePos) = 0;

    virtual void onEvent(const sf::Event& event) = 0;

    virtual const IInputLayer::MsgData& getMsgData() const = 0;
};