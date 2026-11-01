#pragma once

#include <cstddef>   // ← 新增，size_t

#include "../../include/context/stateContext.h"

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
    // 入场/退场只碰世界状态：拿不到输入，也发不出意图
    // world退化为世界状态，不再承载输入输出数据流
    // 签名即权限：
    //   onEnter/onExit(Data& world)  切栈时刻 —— 按钮未注册、输入已被旧状态消费、
    //                                意图本帧末尾就清空。只该碰世界状态。
    //   tick(StateContext& ctx)      帧中段   —— 输入新鲜、按钮已注册、意图可写。
    //
    // R0 阶段这条只是约定；R3（Intend 移出 Data）和 R5（Data 变成 GameData）
    // 之后由类型强制。
    virtual void onEnter(Data& world) = 0;
    virtual void onExit(Data& world) = 0;
    virtual void tick(StateContext& ctx) = 0;
    virtual IGameState::GameSta getID() const = 0;
};

inline constexpr size_t kStateCount = static_cast<size_t>(IGameState::GameSta::COUNT);