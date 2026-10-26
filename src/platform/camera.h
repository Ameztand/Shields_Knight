#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>

#include "../include/screen.h"


class Window {
private:
    Screen screen_;//窗口要用的到这个数据就要先声明
    sf::RenderWindow window_;//窗口事件

public:
    Window();// 建窗口 + setMinimumSize + 初始 Screen
    bool isOpen() const;
    void close();

    std::optional<sf::Event> pollEvent();//需要被while
    void onResize(sf::Vector2u size);// 唯一写 screen_ 的地方
    const Screen& screen() const { return screen_; }  // 把屏幕事实给别人看
    std::optional<sf::Vector2i> getMousePixel() const;//提供鼠标坐标
    sf::RenderWindow& target() { return window_; }    // 把窗口借给 Renderer 画

};