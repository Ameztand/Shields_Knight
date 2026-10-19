# Shields_Knight

一个基于 **SFML 3.1 / C++17** 的桌面游戏外壳，用来验证一套最小引擎骨架：

> **输入 → 黑板 → 状态机 → 渲染快照 → 渲染**

目标不是通用引擎，而是一个**能扛住自己项目**的最小引擎子集。

当前版本 **V0.1.0.1**（文档校准 + 接入 CMake，功能与 V0.1.0 相同）。

---

## 现状

能开窗、能画测试图、能按 Esc 走栈。
**按钮热点能点，但画不出来** —— 渲染快照仍是空的。

---

## 构建

### 前置

| 依赖 | 版本 | 本机路径 |
|---|---|---|
| MinGW g++ | 14.2.0 | `D:\vsCodeTools\mingw64\bin` |
| SFML | 3.1.0 | `D:\vsCodeTools\SFML-3.1.0` |
| CMake | ≥ 3.25 | `D:\vsCodeTools\cmake-4.4.4-windows-x86_64` |

路径不同时只改 `CMakePresets.json` 里的三行，`CMakeLists.txt` 不用动。

### 命令行

```
cmake --preset mingw
cmake --build --preset mingw
build\bin\Shields_Knight.exe
```

### VS Code

装 **CMake Tools** 扩展，`Ctrl+,` 把 `cmake.cmakePath` 指向 `cmake.exe`。
打开项目后状态栏显示 `mingw`，**F7** 构建，`F5` 调试。

### 产物

`build\bin\` 是唯一运行目录。`pic\` 与 SFML 的 dll 由构建自动拷入，**整个文件夹可直接拷走运行**。

---

## 目录

```
src/
├─ main.cpp               主循环：poll → Exit 判定 → tick → commitRenderData → render
├─ input/                 输入层：SFML 事件 → 五态 KeySta 快照
├─ logic/                 逻辑层
│  ├─ dataStore.h         黑板：MsgData / 时间戳 / 按钮快照 / 意图
│  ├─ logic.h|cpp         调度：输入 → 按钮 → 状态机 → 意图结算
│  ├─ buttonManager/      按钮适配（表驱动，按状态查 kButtonTable）
│  └─ stateMachine/       栈式状态机：Lobby / Select / Pause / PuGame
├─ render/                渲染层（当前只有 renderTEST 测试绘制）
└─ include/               配置与共享类型
```

---

## 状态矩阵

| 模块 | 代码 | 实测 | 说明 |
|---|---|---|---|
| 输入层 | ✅ | ✅ | 五态 `KeySta`；鼠标左键并入同一套判定（`COUNT = KeyCount + 1`） |
| 状态机 | ✅ | ✅ | 栈式 push / pop / replace，Esc 走栈实测通过 |
| 按钮层 | ✅ | ⚠️ | 命中判定按窗口像素进行；当前窗口正好 1920×1080 等于逻辑坐标，**1:1 侥幸成立**。快照不进 `RenderData`，故不可见 |
| 渲染层 | ✅ | ✅ | 仅 `renderTEST()`：test.png + 绿圆 |
| 长悬停 | ✅ | ❌ | `now_` 恒为 0，阈值永不触发 |
| 长按 | ✅ | ❌ | `isLongPress` 声明后从未赋值 |

> 「实测」= 作者本机运行验证过。未验证的标 ⚠️，不写成已完成。

---

## 已知缺陷

### 影响功能

| 现象 | 定位 |
|---|---|
| 渲染快照为空，按钮不显示 | `logic.cpp:102` `commitRenderData()` 直接返回从未填充的成员 |
| 按住按钮连续触发 | `buttonManager.cpp:49` 用 `isMouseF` **电平**判定，不是下降沿 |
| 点「重新开始」实际是「继续游戏」 | `config_rect.h:9-10` 左/中键坐标完全重合 → 同帧双命中；`logic.cpp:54` 的 `if/else if` 让 `popSta` 优先 |
| Select 页左/中键点了没反应 | `gameState.cpp:83` `switch` 只处理 `selectRightButton` |
| 长悬停永不触发 | `logic.cpp:82` `tick(now)` 的 `now` 参数未被使用；`Data::updataNow()` 无调用者 |
| 长按永不触发 | `button.h:20` `isLongPress` 从不赋值 |

### 已埋下但当前不触发

| 现象 | 定位 |
|---|---|
| 意图置真后不再复位 | `logic.cpp:77` `clearIntend()` 定义了但**从未被调用**，`replaceSelect` / `replacePuGame` 等一旦置真即永久为真（⚠️ 静态推断，未实测） |
| 状态切换后每帧重建 | 同上 |
| 鼠标坐标未映射到逻辑坐标 | `input.cpp:104` 直接用原始像素；当前窗口不可缩放（`Style::Titlebar \| Close`），故未暴露。**一旦支持缩放立即错位** |
| 下层状态按钮不注册 | `logic.cpp:42` `pushSta` 只注册新栈顶，`Pause` 之下的 `PuGame` 无按钮表 |

### 死代码

`State::getStaDepth()`、`Intend::replaceLobby` / `pushLobby`、`Renderer::renderData_`、`Button::isLongPress` —— 定义后无调用者。

---

## 路线

- [ ] 填充 `commitRenderData()`，让按钮真正显示 ← **当前最高优先级**
- [ ] 窗口缩放接口：逻辑坐标固定 1920×1080 + `sf::View`，鼠标经 `mapPixelToCoords` 换算
- [ ] 意图按帧清理（把 `clearIntend()` 接进主循环）
- [ ] `isClicked` 改为下降沿
- [ ] 渲染层重写，替换 `renderTEST`
- [ ] `config` 支持从 txt 读取

---

## 文档

| 文件 | 内容 |
|---|---|
| `docs/DEVLOG.md` | 设计取舍与踩坑：为什么按钮做成「表 + 当帧快照」 |
| `docs/PLAN.md` | 版本记录与长期待办 |

---

## 仓库

Remote 成功迁移到 `Shields_Knight` 。
