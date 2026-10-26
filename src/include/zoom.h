#pragma once

#include <SFML/System/Vector2.hpp>

#include "common.h"        // Rect

// ===== 缩放策略 =====
enum class FitPolicy {
    Cover,      // 等比铺满：填满目标，超出部分溢出（背景类）
    Contain,    // 等比装入：整张塞进去，不足处留白居中（按钮/立绘类）
    Stretch,    // 直接拉伸到目标尺寸（不保比例，仅当比例本就一致时才等价）
};

struct FitResult {
    sf::Vector2f scale{1.f, 1.f};    // → sprite.setScale(...)
    sf::Vector2f origin{0.f, 0.f};   // 相对目标区域左上角的偏移 → 定位用
    sf::Vector2f size{0.f, 0.f};     // 缩放后的实际尺寸（调试/居中用）
};

// src = 美术原始尺寸，dst = 目标区域尺寸
inline FitResult fitInto(sf::Vector2f src, sf::Vector2f dst, FitPolicy policy = FitPolicy::Cover)
{
    FitResult r;
    if (src.x <= 0.f || src.y <= 0.f || dst.x <= 0.f || dst.y <= 0.f) {
        return r;                     // 尺寸非法：原样返回，别产生 inf/nan
    }

    const float kx = dst.x / src.x;   // 按宽塞进去要几倍
    const float ky = dst.y / src.y;   // 按高塞进去要几倍

    switch (policy) {
        case FitPolicy::Cover:   { const float k = (kx > ky) ? kx : ky; r.scale = {k, k}; break; }
        case FitPolicy::Contain: { const float k = (kx < ky) ? kx : ky; r.scale = {k, k}; break; }
        case FitPolicy::Stretch:   r.scale = {kx, ky}; break;
    }                                 // ← 不写 default：将来加策略时编译器会提醒你漏了分支

    r.size   = { src.x * r.scale.x, src.y * r.scale.y };
    r.origin = { (dst.x - r.size.x) * 0.5f,        // 统一居中：
                 (dst.y - r.size.y) * 0.5f };      // Cover 时是负数(溢出)，Contain 时是正数(留白)
    return r;
}

// 直接吃你的 Rect（判定区域）
inline FitResult fitInto(sf::Vector2f src, const Rect& dst, FitPolicy policy = FitPolicy::Cover)
{
    return fitInto(src, { static_cast<float>(dst.width), static_cast<float>(dst.height) }, policy);
}

// 把"相对偏移"落成世界坐标
inline sf::Vector2f fitPosition(const Rect& dst, const FitResult& f)
{
    return { static_cast<float>(dst.pos.x) + f.origin.x,
             static_cast<float>(dst.pos.y) + f.origin.y };
}