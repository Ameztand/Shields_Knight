#pragma once

#include <cstdint>//为了uint32_t
#include <cstddef>//为了size_t
#include <array>//为了kAssetTable数组

#include "include/buttonKind.h"
#include "include/view/fit.h"    // FitPolicy考虑分离？
#include "include/config/colorTable.h"

// 一件美术资源的全部说明。path == nullptr 表示"没有美术，画纯色"。
struct AssetSpec {
    const char* path   = nullptr; //美术路径
    FitPolicy   policy = FitPolicy::Contain;
    uint32_t    rgb    = colorOf(ColorKey::Green);  // 回退色，沿用 sf::Color::Green
    float       extraScale = 1.f;   // 视觉放大：只影响绘制，不影响命中
};

inline constexpr std::array<AssetSpec, kButtonKindCount> kAssetTable {{
    // Null, NormalUIButton, Role, Skill, BUFF → 全部留空走默认
    {}, {}, {}, {}, {},
    // lobbystartButton
    { "./pic/LobbyStartButton.png", FitPolicy::Contain, colorOf(ColorKey::Green), 1.3f },//1.3仅图像放大
    // 其余未配置 → 默认空（path=nullptr，画纯色）
    {}, {}, {}, {}, {}, {}, {},
}};

inline const AssetSpec& assetOf(ButtonKind kind) {
    return kAssetTable[static_cast<size_t>(kind)];
}

// 背景不属于按钮，单独两条（后续背景也要统一进配置表）
inline constexpr AssetSpec kBackgroundLobby{ "./pic/background.png",      FitPolicy::Contain, colorOf(ColorKey::White) };
inline constexpr AssetSpec kBackgroundNull { "./pic/background_null.png", FitPolicy::Contain, colorOf(ColorKey::White) };
