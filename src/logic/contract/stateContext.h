#pragma once

#include <vector>

#include "logic/intendEvent.h"

struct Intend;
struct MsgData;
struct Button;

struct StateContext {
    Intend& intend;
    float dt;//说了多少遍了不要加const！！！
    const MsgData& msgData;
    const std::vector<Button>& buttonList;
};
