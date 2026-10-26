#include "camera.h"



Window::Window()
    : screen_{},
      window_(sf::VideoMode(screen_.window), "Shields_Knight",
              sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close)//有标题栏|允许用户拖边框改变大小|有关闭按钮
{
    window_.setFramerateLimit(60);//帧率
}

bool Window::isOpen() const
{
    return window_.isOpen();;
}

void Window::close()
{
    window_.close();
}

std::optional<sf::Event> Window::pollEvent()
{
    return window_.pollEvent();
}

void Window::onResize(sf::Vector2u size)
{
    if (size.x == 0 || size.y == 0) return;   // 最小化的时候可能是 0，别记进去
    screen_.window = size;
}

std::optional<sf::Vector2i> Window::getMousePixel() const
{
    if (window_.getSize().x == 0) return std::nullopt;   // 最小化时没有有效坐标
    return sf::Mouse::getPosition(window_);
}
