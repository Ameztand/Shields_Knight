#pragma once

#include <cstddef>   // size_t

enum class GameStateKind {
    Null,
    Lobby,
    Select,
    Pause,
    PuGame,
    COUNT//哨兵
};

inline constexpr size_t kStateCount = static_cast<size_t>(GameStateKind::COUNT);