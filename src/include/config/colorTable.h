#pragma once

#include <cstdint>//为了uint32_t
#include <cstddef>//为了size_t
#include <array>//为了kColorTable数组

//颜色配置
enum class ColorKey {
    White,   // 数组下标 0 → 对应 0xFFFFFF
    Black,   // 下标 1 → 0x000000
    Red,     // 下标 2 → 0xFF0000
    Yellow,  // 下标 3 → 0xFFFF00
    Green,   // 下标 4 → 0x1FD18B
    Blue,    // 下标 5 → 0x0000FF
    COUNT    // 哨兵，数组大小靠它
};

inline constexpr size_t kColorKeyCount = static_cast<size_t>(ColorKey::COUNT);

inline constexpr std::array<uint32_t, kColorKeyCount> kPredefinedColors {{
    0xFFFFFF,  // White
    0x000000,  // Black
    0xFF0000,  // Red
    0xFFFF00,  // Yellow
    0x1FD18B,  // Green
    0x0000FF,  // Blue
}};

inline constexpr uint32_t colorOf(ColorKey key) {
    return kPredefinedColors[static_cast<size_t>(key)];
}