#pragma once

#include <chrono>

class TimeClock {
private:
    std::chrono::steady_clock::time_point start_;
    double lastNow_ = 0;      // ← 私有记忆，不出这个类

    double now() const {//返回毫秒
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(now - start_).count();
    }

    void onEnter() {
        start_ = std::chrono::steady_clock::now();
        lastNow_ = now();
    }

public:
    TimeClock() { onEnter(); }//包含入场onEnter

    //更新时间,返回dt.(单位:s)
    float tick() {
        const double t = now();
        float dt_ = (float)(t - lastNow_) * 0.001f;
        lastNow_ = t;

        if (dt_ < 0.0)  dt_ = 0.0;// 时钟回退保护
        if (dt_ > 0.1)  dt_ = 0.1;// 钳制
        return dt_;
    }
};