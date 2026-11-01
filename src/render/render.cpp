#include "render.h"

#include <iostream>
//图片加载
//#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/Sprite.hpp>

//#include <SFML/Graphics/CircleShape.hpp>
//#include <SFML/Graphics/Color.hpp>

#include <SFML/Graphics.hpp>

#include "../include/fit.h"

#include "../logic/buttonManager/button.h"
#include "../logic/stateMachine/IGameState.h"

void Renderer::renderSetView(sf::RenderWindow &window, const Screen &screen)
{
    // ① 按本帧的屏幕事实建相机
    const float W = static_cast<float>(screen.window.x);
    const float H = static_cast<float>(screen.window.y);
    const float k = screen.scale();
    const sf::Vector2f off = screen.offset();

    sf::View view;
    view.setSize(screen.design);                              // 看多大一块画布
    view.setCenter(screen.design * 0.5f);                     // ★ 中心必须是画布中心，漏了会整体偏一半
    view.setViewport(sf::FloatRect({off.x / W, off.y / H},      // 内容左上角（归一化）
                                   {screen.design.x * k / W,   // 内容占宽
                                    screen.design.y * k / H})); // 内容占高
    window.setView(view);                                      // ② 交给窗口，之后所有 draw 都按它算
}

void Renderer::renderTEST(sf::RenderWindow &window, const RenderData &renderData)
{
    //加载图片测试
    //std::cout << "CWD = " << std::filesystem::current_path() << std::endl;

    // 纹理只加载一次更好，但先这样测试
    static sf::Texture texture;//static有生命周期问题
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
    window.draw(sprite);

    // 绿色圆
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({300.f, 200.f});
    window.draw(shape);


    std::cout << "栈深 = " << renderData.gameStaDepth_ ;
    if(!renderData.gameSta_.empty())std::cout << "栈ID = " << static_cast<size_t>(renderData.gameSta_.back());
    std::cout << std::endl;
}

void Renderer::renderBackground(sf::RenderWindow &window, const Screen& screen,const RenderData &renderData)
{
    //临时加区分状态渲染背景
    static sf::Texture texture;//static有生命周期问题
    static sf::Texture texture_null;
    static bool loaded = false;
    static bool loaded_null = false;
    if (!loaded) {
        if (!texture.loadFromFile("./pic/background.png")) {
            std::cerr << "load ./pic/background.png failed\n";
            
        } else {
            loaded = true;
        }
    }
    if (!loaded_null) {
        if (!texture_null.loadFromFile("./pic/background_null.png")) {
            std::cerr << "load ./pic/background_null.png failed\n";
            
        } else {
            loaded_null = true;
        }
    }
    //if (!loaded ) return;                       // ★ 加载失败就别画，空纹理什么都画不出来

    sf::Sprite sprite(texture);
    sf::Sprite sprite_null(texture_null);
    if (!renderData.gameSta_.empty() && renderData.gameSta_.front() == IGameState::GameSta::Lobby) {
        FitResult backgroundFitResult = fitInto(sprite.getLocalBounds().size);
        FitTransform res = fitApply(backgroundFitResult);
        sprite.setScale(res.scale);
        sprite.setPosition(res.pos);

        window.draw(sprite);
        return;
    }
    else {
        FitResult backgroundFitResult = fitInto(sprite_null.getLocalBounds().size);
        FitTransform res = fitApply(backgroundFitResult);
        sprite.setScale(res.scale);
        sprite.setPosition(res.pos);

        window.draw(sprite_null);
        return;
    }
}

void Renderer::renderButton(sf::RenderWindow& window, const RenderData& renderData)
{
    for(const auto& sta : renderData.gameSta_)
    {
        const size_t staID = static_cast<size_t>(sta);
        for(const auto& btn : renderData.buttons_[staID])
        {
            if (btn.status.ID == ButtonID::lobbystartButton) 
            {
                sf::Texture texture_button;//static有生命周期问题
                bool F = texture_button.loadFromFile("./pic/LobbyStartButton.png");
                sf::Sprite sprite_button(texture_button);

                FitResult buttonFitResult = fitInto(sprite_button.getLocalBounds().size, btn.status.area, FitPolicy::Contain);
                FitTransform res = fitApply(buttonFitResult, 1.3f);
                sprite_button.setScale(res.scale);
                sprite_button.setPosition(res.pos);

                window.draw(sprite_button);
                continue;
            }
            const sf::Vector2f button_area_ = 
                { static_cast<float>(btn.status.area.width), static_cast<float>(btn.status.area.height) };//int->float
            sf::RectangleShape shape(button_area_);
            const sf::Vector2f button_pos_ = 
                { static_cast<float>(btn.status.area.pos.x), static_cast<float>(btn.status.area.pos.y) };
            shape.setPosition(button_pos_);
            //颜色
            shape.setFillColor(sf::Color::Green);
            //看不懂的，还会报错
            //shape.setOutlineThickness(thickness);
            //shape.setOutlineColor(outline);
            window.draw(shape);
        }
    }
    //临时渲染按钮，后续汇总到配置表
    
}

void Renderer::render(sf::RenderWindow& window, const Screen& screen, const RenderData& renderData)
{
    const RenderData renderData_ = renderData;//单帧唯一真相

    renderSetView(window, screen);

    window.clear();//清空上一帧
    renderBackground(window, screen, renderData_);
    //renderTEST(window, renderData_);
    renderButton(window, renderData_);
    window.display();//限制帧率？？
}

void Renderer::onEnter()
{
    std::cout << "SFML 窗口已启动, 按下Eac退出。" << std::endl;
}

void Renderer::onExit()
{
    //std::cout << "执行关闭窗口操作。" << std::endl;
    //window_.close();
    //std::cout << "SFML 窗口已关闭。" << std::endl;
}