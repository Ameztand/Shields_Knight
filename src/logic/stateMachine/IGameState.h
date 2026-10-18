#pragma once

#include <cstddef>   // ← 新增，size_t

class Data;

//========== 状态机 ==========
class IGameState {
public:
    enum class GameSta {
        null,
        Lobby,
        Select,
        Pause,
        PuGame,
        COUNT//哨兵
    };

    virtual ~IGameState() = default;
    virtual void onEnter(Data& data) = 0;
    virtual void onExit(Data& data) = 0;
    virtual void tick(Data& data) = 0;
    virtual IGameState::GameSta getID() const = 0;
};

inline constexpr size_t kStateCount = static_cast<size_t>(IGameState::GameSta::COUNT);