#pragma once  // 防止重复包含

//#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
//#include <SFML/Graphics.hpp>

#include "../include/type_render_data.h"
#include "../include/screen.h"


class Renderer{
private:
    //渲染
    //RenderData renderData_;//删掉，传参限制生命周期

    void renderSetView(sf::RenderWindow& window, const Screen& screen);
    void renderTEST(sf::RenderWindow& window, const RenderData& renderData);
    void renderBackground(sf::RenderWindow& window, const Screen& screen,const RenderData& renderData);
    void renderButton(sf::RenderWindow& window, const RenderData& renderData);


public:
    //Renderer();//构造函数，创建窗口

    //渲染
    void render(sf::RenderWindow& window, const Screen& screen, const RenderData& renderData);
    void onEnter();
    void onExit();
};