#include "render.h"

#include <iostream>
//图片加载
//#include <SFML/Graphics/Texture.hpp>
//#include <SFML/Graphics/Sprite.hpp>

//#include <SFML/Graphics/CircleShape.hpp>
//#include <SFML/Graphics/Color.hpp>

#include <SFML/Graphics.hpp>

#include "include/view/fit.h"
#include "include/gameStateKind.h"
#include "logic/button/button.h"


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
    if (!renderData.gameSta_.empty() && renderData.gameSta_.front() == GameStateKind::Lobby) {
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
        if (!renderData.buttons_[staID].empty()) {
            for (const auto& btn : renderData.buttons_[staID])
            {
                if (btn.status.kind == ButtonKind::lobbystartButton)
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
        

        //临时借用渲染实体
        if (!renderData.entitys_[staID].entitys_.empty()) {
            for (const auto& ety : renderData.entitys_[staID].entitys_)
            {
                if (ety.kind == EntityKind::Null) continue;//跳过墓碑

                sf::RectangleShape shape({ ety.w, ety.h });
                shape.setPosition({ ety.left(), ety.top() });   // 中心 → 左上角
                sf::RectangleShape shapeBig({ (float)(ety.w * 1.1), (float)(ety.h * 1.1) });//硬凑，不用fit.h?
                shapeBig.setPosition({ (float)(ety.left() - ety.w * 0.05), (float)(ety.top() - ety.h * 0.05) });   // 中心 → 左上角

                //长悬停背景
                if (ety.isLongHover)
                {
                    sf::RectangleShape shape2({ (float)(ety.w * 1.25), (float)(ety.h * 1.25) });//硬凑，不用fit.h?
                    shape2.setPosition({ (float)(ety.left() - ety.w * 0.125), (float)(ety.top() - ety.h * 0.125) });   // 中心 → 左上角
                    shape2.setFillColor(sf::Color::Red);
                    window.draw(shape2);
                }

                //是否选择
                if (ety.handle.id == renderData.entitys_[staID].selectedID_)
                {
                    shape.setFillColor(sf::Color::Yellow);//选择
                    shapeBig.setFillColor(sf::Color::Yellow);//选择
                }
                else {
                    shape.setFillColor(sf::Color::Red);
                    shapeBig.setFillColor(sf::Color::Red);
                }

                //渲染
                if (ety.isHovered && !ety.isLongHover)
                {
                    window.draw(shapeBig);
                }
                else {
                    window.draw(shape);
                }
            }
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
