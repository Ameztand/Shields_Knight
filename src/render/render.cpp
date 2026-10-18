#include "render.h"

#include <iostream>

//#include <SFML/Graphics.hpp>
//图片加载
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

void Renderer::renderTEST()
{
    //加载图片测试
    //std::cout << "CWD = " << std::filesystem::current_path() << std::endl;

    // 纹理只加载一次更好，但先这样测试
    static sf::Texture texture;
    static bool loaded = false;
    if (!loaded) {
        if (!texture.loadFromFile("./pic/test.png")) {
            std::cerr << "load ./pic/test.png failed\n";
        } else {
            loaded = true;
        }
    }

    sf::Sprite sprite(texture);
    sprite.setPosition({10.f, 10.f});
    window_.draw(sprite);

    // 绿色圆
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({300.f, 200.f});
    window_.draw(shape);
}

Renderer::Renderer(unsigned int logicW, unsigned int logicH)
    : window_(sf::VideoMode({logicW, logicH}), "Genshen??",
              sf::Style::Titlebar | sf::Style::Close)
{
    window_.setFramerateLimit(60);
}

void Renderer::render(const RenderData& renderData)
{
    renderData_ = renderData;//复制快照

    window_.clear();//清空上一帧
    renderTEST();
    window_.display();//限制帧率？？
}

void Renderer::onEnter()
{
    std::cout << "SFML 窗口已启动, 按下Eac退出。" << std::endl;
}

void Renderer::onExit()
{
    std::cout << "执行关闭窗口操作。" << std::endl;
    window_.close();
    std::cout << "SFML 窗口已关闭。" << std::endl;
}