#pragma once

#include "../../include/common.h"
//#include "../stateMachine/IGameState.h"
#include "entityID.h"

struct EntityConfig {
    Position pos;
    Rect collisionBox;
    //Rect artArea;
    EntityID ID;
};

struct Entity {
    EntityConfig status;

    int Vx = 0;
    int Vy = 0;

    void updateMove() {
        Position temp = status.pos;
        temp.x += Vx;
        temp.y += Vy;
        status.pos = temp;

        Position tempBox = status.collisionBox.pos;
        tempBox.x += Vx;
        tempBox.y += Vy;
        status.collisionBox.pos = tempBox;
    }

    // 保留默认构造（以防别的地方要空构造）
    Entity() = default;

    // 接收配置的构造函数
    explicit Entity(const EntityConfig& config) : status(config) {}
};
