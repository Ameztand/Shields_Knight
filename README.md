# Shields_Knight

一个基于 **SFML 3.1 / C++17** 的桌面游戏外壳，用来验证一套最小引擎骨架：

> **输入 → 黑板 → 状态机 → 渲染快照 → 渲染**

目标不是通用引擎，而是一个**能扛住自己项目**的最小引擎子集。

当前版本 **V0.1.0.6**（引入 `StateContext`，状态 `tick` 改吃每帧上下文）。

---

## 现状

能开窗、能画背景和按钮、能按 Esc 走栈、按钮能点也有视觉反馈。
窗口可自由缩放，画面等比不变形并自动留黑边。

**下一步是收窄渲染契约**：让渲染层只认识"怎么画"，不认识"画的是什么品种"。方向见 `docs/DIRECTION.md`。

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
├─ main.cpp               主循环：poll → Exit 判定 → tick → commitRenderData → render
├─ input/                 输入层：SFML 事件 → 五态 KeySta 快照；鼠标并入同一套判定
├─ platform/              Window：建窗口、收 resize、供鼠标像素坐标
├─ render/                渲染层：View 与 letterbox、背景、按钮
├─ logic/                 逻辑层
│  ├─ dataStore.h         黑板：MsgData / 时间戳 / 按钮快照 / 意图
│  ├─ logic.h|cpp         调度：输入 → 按钮 → 状态机 → 意图结算 → 提交快照
│  ├─ buttonManager/      按钮适配（表驱动，按状态查 kButtonTable）
│  └─ stateMachine/       栈式状态机：Lobby / Select / Pause / PuGame
└─ include/               配置与共享类型
   ├─ config_design_view.h  世界分辨率
   ├─ config_rect.h         按钮区域
   ├─ config_button_table.h 状态 → 按钮表
   ├─ screen.h              像素 ↔ 设计坐标、缩放与黑边
   ├─ fit.h                 缩放策略（Cover / Contain / Stretch）
   ├─ type_render_data.h    渲染快照
   └─ context/stateContext.h 帧中段上下文
```

---

## 状态矩阵

「代码」= 静态通读判断；「运行」= 作者本机实测（本轮未复跑，标 ✓ 的沿用原记录）。

| 模块 | 代码 | 运行 | 说明 |
|---|---|---|---|
| 输入层 | ✅ | ✅ | 五态 `KeySta`；鼠标左键并入同一套判定（`COUNT = KeyCount + 1`）；坐标已映射到设计坐标 |
| 状态机 | ✅ | ✅ | 栈式 push / pop / replace；Esc 走栈 |
| 按钮层 | ✅ | ⚠️ | 命中用设计坐标；快照已进 `RenderData`；但覆盖层退出后不重注册，Select 左右键无响应 |
| 渲染层 | ✅ | ✅ | `Screen` + `sf::View` letterbox 正确；背景与按钮能画，但绘制区比命中区大 30%，且每帧重读贴图 |
| 长悬停 | ✅ | ⚠️ | 时间戳已打通（`updataNow` 已调用），逻辑成立，未单独验证 |
| 长按 | ❌ | ❌ | `isLongPress` 声明后从不赋值 |
| 时间 | ⚠️ | ⚠️ | 只有绝对时间，没有 dt |

---

## 已知缺陷

按危害排序。结构性问题的判断与取舍见 `docs/DIRECTION.md`。

| 现象 | 定位 |
|---|---|
| 覆盖层退出后下层按钮全部失效 | `logic.cpp:47-50` `popSta()` 只删不注册，`Pause` 关掉后栈顶无按钮 |
| 每帧从磁盘重读按钮贴图（2.8MB PNG） | `render.cpp:124-125` 在循环内声明局部 `sf::Texture` 并 `loadFromFile` |
| 画出来的比能点的大 30% | `render.cpp:128-129` `fitApply(res, 1.3f)`，命中区 200×80 而绘制约 260×104 |
| 没有 dt | `time_clock.h` 只给绝对毫秒；业务逻辑需要帧间差值 |
| Select 页左右键点了没反应 | `gameState.cpp:82-88` `switch` 只处理 `selectRightButton` |
| 背景按"栈底是不是 Lobby"猜 | `render.cpp:95` 用 `front() == Lobby` 选图，目前靠巧合成立 |
| 设计分辨率硬编码三处 | `fit.h:15`（`{960,540}`）、`common.h:71`（`1920×1080`）、`config_design_view.h` |
| `isLongPress` 从不赋值 | `button.h:20` |
| 长按阈值与业务耦合 | `buttonManager.cpp:49` 点击判定只认 `Falling`，`DownL` 无消费者 |

### 死代码

| 项 | 位置 |
|---|---|
| `Intend::replaceLobby` / `pushLobby` | `dataStore.h:38,42` 定义后无调用者 |
| `Button::isLongPress` | `button.h:20` 从不赋值 |
| `Renderer::renderTEST` | `render.cpp:35` 已不被 `render()` 调用，保留作参考 |
| `buttonVisual.h` | 0 字节空文件 |

---

## 路线

- [ ] **收窄渲染契约**：`Config` 运行时结构 + 适配器 + 统一 `drawLayer` + 纹理缓存 ← 当前目标
- [ ] 数据离开代码：JSON 读取 + 校验 + 回退 + 手动重载
- [ ] 补 dt
- [ ] Select / Pause 中间按钮接入语义
- [ ] 覆盖层的按钮注册（依赖第一项里的 `syncButtonsWithStack`）
- [ ] 死代码清理

取舍与理由见 `docs/DIRECTION.md`；版本记录见 `docs/PLAN.md`。

---

## 文档

| 文件 | 内容 |
|---|---|
| `docs/DIRECTION.md` | 当前判断、接下来三步、AI 的建议、悬而未决 |
| `docs/DEVLOG.md` | 设计取舍与踩坑：为什么按钮做成「表 + 当帧快照」 |
| `docs/PLAN.md` | 版本记录与长期待办 |

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