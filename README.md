# Shields_Knight

一个基于 **SFML 3.1 / C++17** 的桌面游戏外壳，用来验证一套最小引擎骨架：

> **输入 → 黑板 → 状态机 → 渲染快照 → 渲染**

目标不是通用引擎，而是一个**能扛住自己项目**的最小引擎子集。

---

## 现状

能开窗、能画背景和按钮、能按 Esc 走栈、按钮能点。
窗口可自由缩放，画面等比不变形并自动留黑边。

能在 PuGame 里用 WASD 移动一个红色方块（测试实体）。

---

## 构建

### 前置

| 依赖 | 版本 | 本机路径 |
|---|---|---|
| MinGW g++ | 14.2.0 | `D:\vsCodeTools\mingw64\bin` |
| SFML | 3.1.0 | `D:\vsCodeTools\SFML-3.1.0`（MSVC 用 `D:\vsCodeTools\SFML-3.1.0-MSVC\SFML-3.1.0`） |
| CMake | ≥ 3.25 | `D:\vsCodeTools\cmake-4.4.4-windows-x86_64` |

路径不同时只改 `CMakePresets.json`，`CMakeLists.txt` 不用动。

### 命令行
```
cmake --preset mingw
cmake --build --preset mingw
build\mingw\bin\Shields_Knight.exe
```

### VS 2022

直接打开文件夹，选 `msvc` 预设（排在第一位，打开即默认）。Release 但带 `/Od /Z7 /DEBUG`，所以**能断点调试**。

### VS Code

装 **CMake Tools** 扩展，`Ctrl+,` 把 `cmake.cmakePath` 指向 `cmake.exe`。打开项目后状态栏显示 `mingw`，**F7** 构建，`F5` 调试。

### 产物

`build/<preset>/bin/` 是运行目录。`pic/` 与 SFML 的 dll 由构建自动拷入，**整个文件夹可直接拷走运行**——资源用相对路径 `./pic/...`，所以工作目录必须在这个 `bin/` 下（不是项目根）。

---

## 目录
```
src/
├─ main.cpp               主循环：poll → Exit 判定 → tick → 提交快照 → render
├─ input/                 输入层：SFML 事件 → 五态 KeySta 快照；鼠标并入同一套判定
├─ platform/              Window：建窗口、收 resize、供鼠标像素坐标；Win32 侧禁输入法
├─ render/                渲染层：View 与 letterbox、背景、按钮、实体框
├─ logic/                 逻辑层
│  ├─ dataStore.h         黑板：MsgData / 时间戳与 dt / 按钮快照 / 意图
│  ├─ gameData.h          PuGameData：单个游戏模式的游戏数据
│  ├─ logic.h|cpp         调度：输入 → 按钮 → 状态机 → 意图结算 → 提交快照
│  ├─ buttonManager/      按钮适配（表驱动，按状态查 kButtonTable）
│  ├─ entityManager/      实体表（Entity 为 POD）+ 槽位复用；由状态持有
│  └─ stateMachine/
│     ├─ gameStateMachine.h|cpp  栈式状态机：push / pop / replace
│     └─ gameState/             Lobby / Select / Pause / PuGame
└─ include/               配置与共享类型
   ├─ common.h              坐标和矩形区域的声明
   ├─ config_design_view.h  世界分辨率
   ├─ config_rect.h         按钮区域
   ├─ config_button_table.h 状态 → 按钮表
   ├─ screen.h              像素 ↔ 设计坐标、缩放与黑边
   ├─ fit.h                 缩放策略（Cover / Contain / Stretch）
   ├─ type_render_data.h    渲染快照
   ├─ type_render_entity.h  提交给渲染的实体数据
   └─ context/stateContext.h 帧中段上下文
```

---

## 工具链约定

| 场景 | 预设 | 说明 |
|---|---|---|
| VS 2022 | `msvc` | Ninja + MSVC x64。x64 环境由 VS 自动注入；**普通 PowerShell 里直接跑会捡到 32 位 cl.exe**，表现为 `SFML_DIR-NOTFOUND` |
| VS Code / 任何终端 | `mingw` | MinGW g++，编译器与 make 都写死绝对路径，不依赖环境变量 |

- 两个预设都显式指定 `SFML_DIR`，不靠 `CMAKE_PREFIX_PATH` 搜索（搜索会同时命中 MinGW 包和 vc17 包，选哪个靠运气）
- `CMakeLists.txt` 里有 x64 自检：32 位 MSVC 会在 configure 阶段直接给中文报错，不再是 `SFML_DIR-NOTFOUND`
- 源文件用 `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)` 自动收集，**新增 `.cpp` 不用改本文件**。代价是 IDE 看不到显式文件列表，VS 会弹「建议修改 CMake 脚本」——一律取消

## 源码编码约定

- 所有源文件必须是 **UTF-8（无 BOM）**。`/source-charset:utf-8` 已开启，GBK 文件会报 C4819 / C2001
- VS 新建文件默认用系统 ANSI（GBK），保存时需选「编码保存 → UTF-8 无签名」
- 控制台输出**只用 ASCII**：执行字符集是 GBK(936)，与 UTF-8 终端不一致时中文（尤其全角标点）会乱码
- 调试输出统一用 `std::cout << ... << std::endl`，不混用 `printf`（`\n` 不保证 flush，`std::endl` 会）

---

## 文档

| 文件 | 内容 |
|---|---|
| `docs/PLAN.md` | 待办（只放还没做的事） |
| `docs/VERSION.md` | 版本时间线（版本 / 日期 / commit / tag） |
| `docs/DEVLOG.md` | 为什么这么改、否决方案、踩坑 |
| `docs/CONVENTION.md` | 版本号、五份文档分工、提交格式、自测 |
