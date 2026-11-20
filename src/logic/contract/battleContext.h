#pragma once

struct PuGameData;
class EntityStore;


struct BattleContext {
    PuGameData& gameData;
    EntityStore& entityPool;//允许修改？还是const引用？
};