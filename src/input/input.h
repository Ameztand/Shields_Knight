#pragma once  // 防止重复包含

#include "../include/screen.h"
#include "IInputLayer.h"

#include <array>
#include <chrono>
#include <optional>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>



namespace sf { class Event; } 


class Input : public IInputLayer{
private:
    static constexpr int KEY_COUNT = static_cast<int>(sf::Keyboard::KeyCount);//101个按键
    static constexpr int COUNT = KEY_COUNT + 1 ;//最后一个是鼠标左键
    static constexpr int LOW_PRESS_TIME = 200;//按下最短时间

    //帧快照-原始数据
    std::array<bool, COUNT> rawKeys_ = {};
    sf::Vector2i rawMousePos_ = {};//保持原始数据

    std::array<bool, COUNT> prevKeys_ = {};//上一帧按下情况
    std::array<std::chrono::steady_clock::time_point, COUNT> pressTime_ = {};// 按下时间戳

    //帧快照-加工数据
    std::array<IInputLayer::KeySta, COUNT> keySta_ = {};
    Position mousePos_ = Epos;//转换坐标类型定义
    
    IInputLayer::MsgData msgData;//输出数据

    void translate(const Screen& screen, std::optional<sf::Vector2i> mousePos);
    void updateMsgData();
    
public:
    explicit Input() = default;

    void HandleEvent(const Screen& screen, std::optional<sf::Vector2i> mousePos) override;

    void onEvent(const sf::Event& event) override;
    
    const IInputLayer::MsgData& getMsgData() const override;
};