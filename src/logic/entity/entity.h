#pragma once

#include <cstdint>//为了uint32_t

#include "include/common.h"
#include "logic/entity/entityKind.h"

struct EntityHandle {                       // 是哪一个：池分配的号
    uint32_t id = 0;                        // 0 = 无效
    explicit operator bool() const { return id != 0; }
};

struct Entity {
    EntityHandle handle;                    // spawn 时分配，之后永不改
    EntityKind kind = EntityKind::Null;

    Vec2  pos;                      // 中心 = 碰撞箱中心 = 逻辑位置
    Vec2  vel;                      // 像素/秒
    float w = 100.f, h = 100.f;     // 碰撞箱尺寸
    float speed = 300.f;            // 像素/秒

    bool   isHovered = false;   // 派生：每帧由 updateHover 全量重算
    bool   isLongHover = false; // 派生
    double hoverTime = 0.0;     // 悬停计时用
    //挪成配置常量 kLongHoveeSec？
    double hoverThreshold = 1.0;//悬停阈值(单位不是毫秒！！！)


    // 碰撞箱是【派生的】，不存储
    // 变形？采用记录四个顶点？
    float left()    const { return pos.x - w * 0.5f; }
    float top()     const { return pos.y - h * 0.5f; }
    float right()   const { return pos.x + w * 0.5f; }
    float bottom()  const { return pos.y + h * 0.5f; }

    void updateMove(const float dt) {
        pos = pos + vel * dt;            // ← 只动一个东西
    }
};

inline constexpr Entity Eentity = {};
