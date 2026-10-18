#pragma once  // 防止重复包含

//#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
//#include <SFML/Graphics.hpp>

#include "type_render_data.h"

class Renderer{
private:
    sf::RenderWindow window_;//窗口事件
    RenderData renderData_;

    void renderTEST();

public:
    Renderer(unsigned int logicW = 1920, unsigned int logicH = 1080);//构造函数，创建窗口

    void render(const RenderData& renderData);
    void onEnter();
    void onExit();

    //控制器
    bool isOpen() const { return window_.isOpen(); }
    void close()      { window_.close(); }
    sf::RenderWindow& getWindow() { return window_; }//不可以const
};