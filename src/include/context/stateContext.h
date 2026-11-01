#pragma once

class Data;   // 前置声明即可：这里只持有引用

struct StateContext {
    Data& world;   // 暂时就是原来的 Data；R5 改名 GameData 并移进 State 持有
};
