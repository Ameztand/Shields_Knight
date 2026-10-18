#include "input.h"

#include "config.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

//构造链接
Input::Input(sf::RenderWindow& window)
    : window_(window)
{}

void Input::onEvent(const sf::Event &event)
{
    //写入帧快照
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {//按下
        int code = static_cast<int>(keyPressed->code);
        if (code >= 0 && code < KEY_COUNT)rawKeys_[code] = true;
    }
    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())//抬起
    {
        int code = static_cast<int>(keyReleased->code);
        if (code >= 0 && code < KEY_COUNT)rawKeys_[code] = false;
    }
    else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())//鼠标左键按下
    {
        if (mousePressed->button == sf::Mouse::Button::Left)rawKeys_[COUNT - 1] = true;
    }
    else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())//鼠标左键抬起
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)rawKeys_[COUNT - 1] = false;
    }
    else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())//鼠标移动
    {
        rawMousePos_ = mouseMoved->position;//保持原始数据
    }
    else if (event.is<sf::Event::FocusLost>())//失焦
    {
        rawKeys_.fill(false);
        //rawKeys_[COUNT - 1] = false;
    }
}

void Input::translate()
{
    auto now = std::chrono::steady_clock::now();

    //键盘+鼠标
    for (int i = 0; i < COUNT; ++i)
    {
        bool curr = rawKeys_[i];
        bool prev = prevKeys_[i];

        int mode = (curr ? 1 : 0) | (prev ? 2 : 0); // 00，01，11，10

        switch (mode)
        {
        case 0b01: //刚按下
            keySta_[i] = IInputLayer::KeySta::Falling;
            pressTime_[i] = now;
            break;

        case 0b11: //持续按住
        {//限制作用域
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - pressTime_[i]).count();
            keySta_[i] = (duration >= LOW_PRESS_TIME) ? IInputLayer::KeySta::DownL : IInputLayer::KeySta::DownS;
            break;
        }
            
        case 0b10: //刚释放
            keySta_[i] = IInputLayer::KeySta::Rising;
            //时间要归零吗？
            break;

        case 0b00: //空闲
        default:
            keySta_[i] = IInputLayer::KeySta::Null;
            break;
        }

        prevKeys_[i] = curr; //保存当前状态?
    }

    //鼠标坐标
    //mousePos_.x = rawMousePos_.x;
    //mousePos_.y = rawMousePos_.y;
}

void Input::updateMsgData()
{
    auto getSta = [this](sf::Keyboard::Key k) {
        int idx = static_cast<int>(k);
        if (idx < 0 || idx >= KEY_COUNT) return IInputLayer::KeySta::Null;
        return keySta_[idx];
    };

    msgData.Esc = getSta(sf::Keyboard::Key::Escape);
    msgData.F1  = getSta(sf::Keyboard::Key::F1);
    msgData.Alt = getSta(sf::Keyboard::Key::LAlt);//单左alt

    msgData.isMouseF = rawKeys_[COUNT - 1];//input.mouseLogicPos = {0.f, 0.f};还没有映射成逻辑坐标
    //msgData.MousePos = mousePos_;
    msgData.MousePos = {rawMousePos_.x, rawMousePos_.y};
}

void Input::poll()
{
    while (const auto event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) 
        { 
            window_.close(); 
            continue; 
        }
        onEvent(*event);
    }
    translate();
    updateMsgData();
}

const IInputLayer::MsgData &Input::getMsgData() const
{
    return msgData;
}
