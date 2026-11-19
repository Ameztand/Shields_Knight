#pragma once

#include <cstddef>   // size_t

enum class ButtonKind {
    Null,
    NormalUIButton,
    Role,//角色(实际上，以后这里直接枚举所有角色id和技能id。。。吗？)
    Skill,
    BUFF,
    lobbystartButton,
    selectLeftButton,
    selectMiddleButton,
    selectRightButton,
    pauseLeftButton,
    pauseMiddleButton,
    pauseRightButton,
    puGameTestButton,
    COUNT //哨兵
};

inline constexpr size_t kButtonKindCount = static_cast<size_t>(ButtonKind::COUNT);