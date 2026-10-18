#pragma once  // 防止重复包含

//逻辑像素常量（看不到）
constexpr float kLogicWidth  = 1920.f;
constexpr float kLogicHeight = 1080.f;

// ========== 通用常量 ==========
inline constexpr int LOW_PRESS_TIME    = 200;   // 常规按下时间阈值（毫秒）
inline constexpr int PIECE_START_X     = 100;   // 棋盘左上角 x
inline constexpr int PIECE_START_Y     = 100;   // 棋盘左上角 y
inline constexpr int PIECE_CELL_SIZE   = 80;    // 棋盘格子大小