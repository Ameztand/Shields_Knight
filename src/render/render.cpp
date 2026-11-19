#include "render.h"

#include <iostream>
//#include <memory>//为了unique_ptr(图片路径)

#include <SFML/Graphics.hpp>

#include "include/view/fit.h"
#include "include/gameStateKind.h"
#include "logic/button/button.h"

namespace {
    // 将 uint32_t 转换为 sf::Color
    sf::Color toColor(uint32_t rgb) {
        return sf::Color((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
    }
}


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

void Renderer::renderBackground(sf::RenderWindow &window, const Screen& screen,const RenderData &renderData)
{
    const bool isLobby = !renderData.gameSta_.empty() && renderData.gameSta_.front() == GameStateKind::Lobby;
    const AssetSpec& spec = isLobby ? kBackgroundLobby : kBackgroundNull;
    const sf::Texture* tex = isLobby ? assets_.textureBgLobby() : assets_.textureBgNull();
    drawAsset(window, spec, tex, BackgroundRect);
}

void Renderer::renderButton(sf::RenderWindow& window, const RenderData& renderData)
{
    for(const auto& sta : renderData.gameSta_)
    {
        const size_t staID = static_cast<size_t>(sta);
        for (const auto& btn : renderData.buttons_[staID])
        {
            const AssetSpec& spec = assetOf(btn.status.kind);
            drawAsset(window, spec, assets_.textureOf(btn.status.kind), btn.status.area);
        }
        

        //临时借用渲染实体
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
    //临时渲染按钮，后续汇总到配置表
    
}

void Renderer::drawAsset(sf::RenderTarget& target, const AssetSpec& spec, const sf::Texture* tex, const Rect& area)
{
    if (!tex) {// 无美术 / 加载失败
        sf::RectangleShape shape({ float(area.width), float(area.height) });
        shape.setPosition({ float(area.pos.x), float(area.pos.y) });
        shape.setFillColor(toColor(spec.rgb));
        target.draw(shape);
        return;
    }
    // 有美术 / 加载成功
    sf::Sprite sprite(*tex);
    const FitResult res = computeFit(sprite.getLocalBounds().size, area, spec.policy, spec.extraScale);
    sprite.setScale(res.scale);
    sprite.setPosition(res.pos);
    target.draw(sprite);
}

void Renderer::onEnter()
{
    assets_.loadAll();//加载所有资源，仅在构造函数调用一次
    std::cout << "SFML 窗口已启动, 按下Eac退出。" << std::endl;
}

void Renderer::onExit()
{
    //std::cout << "执行关闭窗口操作。" << std::endl;
    //window_.close();
    //std::cout << "SFML 窗口已关闭。" << std::endl;
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