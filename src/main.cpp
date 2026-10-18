#include <windows.h>
#include <iostream>

#include "config.h"

#include "./input/input.h"
#include "./render/render.h"
#include "./include/utils/time_clock.h"
#include "./logic/logic.h"


int main() {
    //加载数据

    //创建对象
    Renderer render(kLogicWidth, kLogicHeight);//创建窗口，逻辑像素
    Input input(render.getWindow());
    TimeClock clock;//每进入对局状态一次重置？
    Logic logic;

    render.onEnter();

    while (render.isOpen()) {
        input.poll();
        const auto& msgData = input.getMsgData();

        if (logic.Exit()) {
            std::cout << "执行logic.Exit退出。" << std::endl;
            render.onExit();
            break;
        }

        logic.tick(msgData, clock.now());
        render.render(logic.commitRenderData());
    }

    std::cout << "程序正常退出。" << std::endl;

    Sleep(1000);

    return 0;
}