#pragma once  // 防止重复包含

//#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
//#include <SFML/Graphics.hpp>


#include "include/view/screen.h"
#include "logic/contract/renderData.h"
#include "render/assetCache.h"


class Renderer{
private:
    //渲染
    //RenderData renderData_;//删掉，传参限制生命周期

    void renderSetView(sf::RenderWindow& window, const Screen& screen);
    void renderBackground(sf::RenderWindow& window, const Screen& screen,const RenderData& renderData);
    void renderButton(sf::RenderWindow& window, const RenderData& renderData);
    void drawAsset(sf::RenderTarget&, const AssetSpec&, const sf::Texture*, const Rect&);

    void onEnter();
    void onExit();

    AssetCache assets_;      // ← 新增

public:
    Renderer() { onEnter(); }//构造函数，创建窗口

    //渲染
    void render(sf::RenderWindow& window, const Screen& screen, const RenderData& renderData);
    
};