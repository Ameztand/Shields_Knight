#pragma once

#include <SFML/System/Vector2.hpp>

#include "common.h"        // Rect

// ===== 缩放策略 =====
enum class FitPolicy {
    Cover,      // 等比铺满：填满目标，超出部分溢出
    Contain,    // 等比装入：整张塞进去，不足处留白居中
    Stretch,    // 直接拉伸到目标尺寸（不保比例，仅当比例本就一致时才等价）
};

struct FitResult {
    sf::Vector2f center = { 960, 540 };        // 中心坐标(硬编码世界大小/2)
    sf::Vector2f scale = { 1.0f, 1.0f };       // 基础缩放（不含 extraScale）
    sf::Vector2f size = { 0.0f,0.0f };         // 缩放后实际尺寸（exOrigin 靠它算，必须存）
    sf::Vector2f pos = { 0.0f,0.0f };          // 基础左上角（世界坐标系，z=1 时直接用）
    //sf::Vector2f exOrigin = { 0.0f,0.0f };     // 添加额外缩放后左上交坐标
};

// ---------- 重载：Rect 目标（按钮，默认 Contain 留黑边） ----------
// src = 美术原始尺寸，dst = 目标区域尺寸
inline FitResult fitInto(const sf::Vector2f img_size, const Rect& target = BackgroundRect, const FitPolicy policy = FitPolicy::Contain)
{
    FitResult r;
    if (img_size.x <= 0.f || img_size.y <= 0.f || target.height <= 0 || target.width <= 0) {
        return r;                     // 尺寸非法：原样返回，别产生 inf/nan
    }

    const float kx = static_cast<float>(target.width) / img_size.x;    // 按宽塞进去要几倍
    const float ky = static_cast<float>(target.height) / img_size.y;   // 按高塞进去要几倍

    switch (policy) {
        case FitPolicy::Cover:   { const float k = (kx > ky) ? kx : ky; r.scale = {k, k}; break; }
        case FitPolicy::Contain: { const float k = (kx < ky) ? kx : ky; r.scale = {k, k}; break; }
        case FitPolicy::Stretch:   r.scale = {kx, ky}; break;
    }                                 // ← 不写 default：将来加策略时编译器会提醒你漏了分支

    r.size = { img_size.x * r.scale.x, img_size.y * r.scale.y }; //基础缩放
    r.center = { static_cast<float>(target.pos.x) + target.width * 0.5f,
                 static_cast<float>(target.pos.y) + target.height * 0.5f };
    r.pos = { r.center.x - r.size.x * 0.5f,    //靠size计算
              r.center.y - r.size.y * 0.5f };
    //r.exOrigin = r.origin;
    return r;
}

struct FitTransform {
    sf::Vector2f scale;
    sf::Vector2f pos;
};

// ---------- 获取变换信息（支持额外缩放） ----------
inline FitTransform fitApply(const FitResult& f, float extraScale = 1.f)
{
    if (extraScale == 1.f)
        return { f.scale, f.pos };
    const sf::Vector2f s{ f.size.x * extraScale, f.size.y * extraScale };
    return { { f.scale.x * extraScale, f.scale.y * extraScale },
             { f.center.x - s.x * 0.5f, f.center.y - s.y * 0.5f } };
}

