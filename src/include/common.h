#pragma once
#include <array>

//逻辑坐标（没看懂）
struct Vec2 {
    float x = 0.f;
    float y = 0.f;
    Vec2 operator+(Vec2 o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(Vec2 o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(float s) const { return {x * s, y * s}; }
};

// ========== 坐标 ==========
struct Position {
    int x = 0;
    int y = 0;

    // 成员函数形式重载 ==
    constexpr bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    constexpr bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

//单位坐标(-1,-1)
inline constexpr Position Epos{-1, -1};

// ========== 区域 ==========
//传参(左上角坐标，宽，高)
struct Rect {
    //再也不敢const了哭哭哭
    Position pos = Epos;//左上角坐标
    int width = 0;
    int height = 0;                      

    // 唯一构造入口，必须传参(左上角坐标，宽，高)
    constexpr Rect(Position pos_, int width_, int height_)
        : pos{pos_}, width{width_}, height{height_} {}

    // 边界
    constexpr int left()   const { return pos.x; }
    constexpr int top()    const { return pos.y; }
    constexpr int right()  const { return left() + width;  }
    constexpr int bottom() const { return top()  + height; }

    // 点是否在区域内（左上闭、右下开，避免相邻矩形重复命中）
    constexpr bool contains(const Position& pos) const {
        return pos.x >= left() && pos.x < right()
            && pos.y >= top()  && pos.y < bottom();
    }

    // 两个矩形是否相交（AABB 碰撞检测）
    constexpr bool intersects(const Rect& o) const {
        return !(right() <= o.left() || o.right() <= left()
              || bottom() <= o.top()  || o.bottom() <= top());
    }

    constexpr bool operator==(const Rect& o) const {
        return left() == o.left() && top() == o.top()
            && width == o.width && height == o.height;
    }
    constexpr bool operator!=(const Rect& o) const { return !(*this == o); }
};

//单位区域(-1,-1)
inline constexpr Rect Erect{Epos, 0, 0};

//背景区域
inline constexpr Rect BackgroundRect{ {0,0}, 1920, 1080 };//要不要引入config_design_view??