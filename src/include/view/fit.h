#pragma once

#include <SFML/System/Vector2.hpp>

#include "include/common.h"        // Rect

// ===== 缩放策略 =====
enum class FitPolicy {
    Cover,      // 等比铺满：填满目标，超出部分溢出
    Contain,    // 等比装入：整张塞进去，不足处留白居中
    Stretch,    // 直接拉伸到目标尺寸（不保比例，仅当比例本就一致时才等价）
};


struct FitResult {
    sf::Vector2f scale = { 1.0f, 1.0f };
    sf::Vector2f pos   = { 0.0f, 0.0f };
};

inline FitResult computeFit(const sf::Vector2f img_size, const Rect& targetRect, const FitPolicy policy, float extraScale = 1.f)
{
    FitResult res;
    if (img_size.x <= 0.f || img_size.y <= 0.f || targetRect.height <= 0 || targetRect.width <= 0) {
        return res;                     // 尺寸非法：原样返回，别产生 inf/nan
    }

    const float kx = static_cast<float>(targetRect.width) / img_size.x;    // 按宽塞进去要几倍
    const float ky = static_cast<float>(targetRect.height) / img_size.y;   // 按高塞进去要几倍

    switch (policy) {
        case FitPolicy::Cover:   { const float k = (kx > ky) ? kx : ky; res.scale = {k, k}; break; }
        case FitPolicy::Contain: { const float k = (kx < ky) ? kx : ky; res.scale = {k, k}; break; }
        case FitPolicy::Stretch:   res.scale = {kx, ky}; break;
    }                                 // ← 不写 default：将来加策略时编译器会提醒你漏了分支

    const sf::Vector2f size  { img_size.x * res.scale.x, img_size.y * res.scale.y };//基础缩放
    const sf::Vector2f center = { static_cast<float>(targetRect.pos.x) + targetRect.width * 0.5f,
                                  static_cast<float>(targetRect.pos.y) + targetRect.height * 0.5f };
    res.pos = { center.x - size.x * 0.5f,    //靠size计算
                center.y - size.y * 0.5f };

    if (extraScale == 1.f) return { res.scale, res.pos };

    const sf::Vector2f s{ size.x * extraScale, size.y * extraScale };
    return { { res.scale.x * extraScale, res.scale.y * extraScale },
             { center.x - s.x * 0.5f, center.y - s.y * 0.5f } };
}