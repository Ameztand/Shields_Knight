#include <windows.h>
#include <iostream>

#include "./input/input.h"
#include "./render/render.h"
#include "./include/utils/time_clock.h"
#include "./logic/logic.h"
#include "./platform/camera.h"


int main() {
    //加载数据

    //创建对象
    Window window;//创建窗口

    Renderer render;
    Input input;
    TimeClock clock;
    Logic logic;

    render.onEnter();

    bool close = false;//难道sysCommand是对的？
    while (window.isOpen()) {
        //分发输入事件
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) close = true;// 点关闭按钮
            else if (auto* r = event->getIf<sf::Event::Resized>()) window.onResize(r->size);  // 拖大拖小
            else input.onEvent(*event);// 其余全丢给输入层
        }
        //处理事件，提炼数据
        input.HandleEvent(window.screen(), window.getMousePixel());
        const auto& msgData = input.getMsgData();

        if (close || logic.Exit()) {
            std::cout << "退出进程" << std::endl;
            window.close();
            break;
        }
        //if (logic.resetTime())clock.reset();

        logic.tick(msgData, clock.now());
        render.render(window.target(), window.screen(), logic.commitRenderData());
    }

    std::cout << "程序正常退出。" << std::endl;

    Sleep(1000);

    return 0;
}