#pragma once

#include <SFML/System/Vector2.hpp>

#include "config_design_view.h"   // kDesignW / kDesignH，别再硬编码第三次

struct Screen {
    sf::Vector2f design{kDesignW, kDesignH};
    sf::Vector2u window{static_cast<unsigned>(kDesignW), static_cast<unsigned>(kDesignH)};

    // ① 缩放倍数：把整块画布塞进窗口的那个统一倍率（取小的，保证装得下）
    float scale() const
    {
        const float sx = static_cast<float>(window.x) / design.x;
        const float sy = static_cast<float>(window.y) / design.y;
        return (sx < sy) ? sx : sy;
    }

    // ② 黑边：画布居中后左上角的留白（像素）
    sf::Vector2f offset() const
    {
        const float k = scale();
        return { (static_cast<float>(window.x) - design.x * k) * 0.5f,
                 (static_cast<float>(window.y) - design.y * k) * 0.5f };
    }

    // ③ 像素 → 设计坐标：先减黑边，再除倍数
    sf::Vector2f toDesign(sf::Vector2i pixel) const
    {
        const float k = scale();
        const sf::Vector2f o = offset();
        //保持float
        return { (static_cast<float>(pixel.x) - o.x) / k,
                 (static_cast<float>(pixel.y) - o.y) / k };
    }
};