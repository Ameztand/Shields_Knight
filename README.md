# Shields_Knight

一个基于 **SFML 3.1 / C++17** 的桌面游戏外壳，用来验证一套最小引擎骨架：

> **输入 → 黑板 → 状态机 → 渲染快照 → 渲染**

目标不是通用引擎，而是一个**能扛住自己项目**的最小引擎子集。

---

## 现状

能开窗、能画背景和按钮、能按 Esc 走栈、按钮能点。
窗口可自由缩放，画面等比不变形并自动留黑边。

能在 PuGame 里用 WASD 移动一个红色方块（测试实体）。

按下 `alt` 可以注册两个测试实体，如果可能会删掉黄色的已选择实体。

实体可以被鼠标拖动，悬停和长时间悬停均有视觉体现

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

`pic/` **不入库**（见 `.gitignore`）：新克隆需自备该目录；缺失时渲染层回退为纯色方块，程序照常启动。

---

## 目录
```
src/
├─ main.cpp                 主循环：poll → Exit 判定 → tick → 提交快照 → render
├─ include/                 零依赖区：只依赖标准库与 SFML::System
│  ├─ common.h              Vec2 / Position / Rect / Epos / Erect
│  ├─ buttonKind.h          按钮闭集 ID（配置表的键）
│  ├─ gameStateKind.h       状态闭集 ID + kStateCount
│  ├─ config/               编译期配置数据
│  │  ├─ buttonTable.h      ButtonConfig + kButtonTable（状态 → 按钮表）
│  │  ├─ buttonTableRect.h  各按钮的区域
│  │  └─ designView.h       世界分辨率 1920×1080
│  ├─ view/                 像素 ↔ 设计坐标
│  │  ├─ screen.h           Screen：缩放、黑边、toDesign
│  │  └─ fit.h              缩放策略 Cover / Contain / Stretch
│  └─ utils/
│     └─ time_clock.h       steady_clock 毫秒时间源
├─ input/                   输入层
│  ├─ msgData.h             KeySta / MsgData / held()：纯数据，不含 SFML
│  └─ input.h|cpp           Input：五态判定 + 鼠标坐标映射
├─ platform/                窗口层
│  └─ camera.h|cpp          class Window（文件名待改 window.*；Win32 侧禁输入法）
├─ render/                  渲染层：只读 RenderData
│  └─ render.h|cpp          View/letterbox、背景、按钮、实体框
└─ logic/                   逻辑层
   ├─ logic.h|cpp           调度：输入 → 按钮 → 状态机 → 意图结算 → 提交快照
   ├─ dataStore.h           黑板：时间戳与 dt / 消息 / 按钮快照 / 意图
   ├─ gameData.h            PuGameData：单个游戏模式的游戏数据
   ├─ button/               Button（运行期态）+ ButtonStore（表驱动派生）
   ├─ entity/               Entity（POD）+ EntityStore（槽位复用，由状态持有）
   ├─ stateMachine/         GameStateMachine（push/pop/replace）+ IGameState
   │  └─ gameState/         Lobby / Select / Pause / PuGame
   └─ contract/             快照与帧上下文——render 唯一可见的 logic 目录
      ├─ renderData.h       RenderData（一帧渲染快照）
      ├─ renderDataPu.h     RenderDataEntity（实体快照）
      └─ stateContext.h     StateContext（帧中段上下文）
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
