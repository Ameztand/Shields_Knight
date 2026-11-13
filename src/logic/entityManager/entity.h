#pragma once

#include "../../include/common.h"
//#include "../stateMachine/IGameState.h"
#include "entityID.h"

struct Entity {
    EntityID ID = EntityID::Null;

    Vec2  pos;                      // 中心 = 碰撞箱中心 = 逻辑位置
    Vec2  vel;                      // 像素/秒
    float w = 100.f, h = 100.f;     // 碰撞箱尺寸
    float speed = 300.f;            // 像素/秒

    // 碰撞箱是【派生的】，不存储
    float left()    const { return pos.x - w * 0.5f; }
    float top()     const { return pos.y - h * 0.5f; }
    float right()   const { return pos.x + w * 0.5f; }
    float bottom()  const { return pos.y + h * 0.5f; }

    void updateMove(float dt) {
        pos = pos + vel * dt;            // ← 只动一个东西
    }
};
