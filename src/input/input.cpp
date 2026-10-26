#include "input.h"

//#include <SFML/Graphics/RenderWindow.hpp>//???
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

//构造链接
//Input::Input(){}

void Input::translate(const Screen& screen, std::optional<sf::Vector2i> mousePos)
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
    //鼠标坐标映射
    //const sf::Vector2f world = screen.toDesign(window_);// 每帧取真实像素+映射
    //mousePos_.x = static_cast<int>(world.x);
    //mousePos_.y = static_cast<int>(world.y);
    // 鼠标坐标：拆盒 + 换算
    if (mousePos) {
        rawMousePos_ = *mousePos;
        const sf::Vector2f world = screen.toDesign(rawMousePos_);
        mousePos_.x = static_cast<int>(world.x);
        mousePos_.y = static_cast<int>(world.y);
    } else {
        mousePos_ = Epos;        // 没有有效位置 → 谁都不悬停
    }
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

    msgData.MouseL  = keySta_[COUNT - 1];
    msgData.MousePos = mousePos_;
}

void Input::HandleEvent(const Screen& screen, std::optional<sf::Vector2i> mousePos)
{
    translate(screen, mousePos);
    updateMsgData();
}

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
    //else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())//鼠标移动
    //{
        //rawMousePos_ = mouseMoved->position;//保持原始数据
        //rawMousePos_ = sf::Mouse::getPosition(window_);        // 每帧取真实像素
        //查看dsh
    //}
    else if (event.is<sf::Event::FocusLost>())//失焦
    {
        rawKeys_.fill(false);
        //rawKeys_[COUNT - 1] = false;
    }
}

const IInputLayer::MsgData &Input::getMsgData() const
{
    return msgData;
}
