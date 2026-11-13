# Shields_Knight 版本记录与待办

## 版本记录

| 版本 | 内容 | 关键决定 |
|---|---|---|
| V0.0.0 | 初始化项目与基础架构 | — |
| V0.0.1 | 输入重构 | `IInputLayer` 抽象 + 五态 `KeySta` + 事件转快照 |
| V0.0.2 | 时间组块 | `TimeClock` 保留接口；渲染像素由 `main` 传入 |
| V0.1.0 | 逻辑层 | 状态机 + 按钮表驱动 + 数据黑板；主循环接上 tick / render |
| V0.1.0.1 | 文档与构建校准 | 接入 CMake；README / PLAN / DEVLOG 调整结构 |
| V0.1.0.2 | 分离 `Window` | 加入 `sf::View` 与 letterbox；MinGW / MSVC 双工具链 |
| V0.1.0.3 | `fit` 重写 | `zoom.h` → `fit.h`，`FitResult` 改为承载 center / size / pos |
| V0.1.0.4 | 背景与贴图占位 | 背景按状态切换；按钮贴图占位；时间戳打通 |
| V0.1.0.5 | 引入 `StateContext` | 状态 `tick` 改吃每帧上下文，为后续拆 `Data` 铺路 |
| V0.1.0.6 | 生命周期四事件 + 实体子系统骨架 | 状态机加 onSuspend/onResume；拆 PuGame；实体走按状态注册（更新未接）；CMakePresets 改 SFML_DIR |
| V0.1.0.7 | 第一个可操作循环 | 实体 WASD 移动；dt 由 Logic 中央计算；坐标 int→float；禁用输入法 |

## 待办

### 主线

1. **收窄渲染契约**：`Config` 运行时结构 + 适配器 + 统一 `drawLayer` + 纹理缓存
2. **让数据离开代码**：JSON 读取 + 校验 + 失败回退 + 手动重载
3. **业务地基**：dt → 实体 → 第一段动画

顺序不可换：1 不做完，2 没有结构可填；2 不做完，3 的实体表无处可放。

### 长期

- Select / Pause 中间按钮接入语义
- 覆盖层的按钮注册（依赖第 1 项里的 `syncButtonsWithStack`）
- 窗口缩放后鼠标命中的回归验证
- 死代码清理
- 设计分辨率收敛到单一来源（目前三处硬编码）
- `Data` 继续拆分：`Intend` 是否移出黑板

## 现状与缺陷

见 `README.md` 的「状态矩阵」「已知缺陷」。

## 判断与取舍

见 `docs/DIRECTION.md`，决策理由记入 `docs/DEVLOG.md`。

## 待办（第二批）

- [ ] 项目根加 `.editorconfig`：`charset = utf-8`（防止新建文件被存成 GBK）
- [ ] `StateContext` 字段改名：`world` → `frame`
      理由：`Data` 装的是「本帧输入 + 时间 + 意图」，不是世界状态；世界状态已独立为 `GameData`
- [ ] `Intend` 移出 `Data`（R3）
- [ ] 能力门控表 + 仲裁管线
      触发条件：出现「同一对象在不同系统里被区别对待」的规则
      挂载点：表由 `Logic` 持有；`collectView` 与输入处理是消费方
      临时的裁决者：`Entity::alive`（领域状态，不是渲染决策）
      参考形态：UE GameplayTags / Godot process_mode
- [ ] 按游戏模式拆分 `GameData`（`PuGameData` / `AtGameData`…）+ `src/logic/gameData/` 目录
      触发条件：第二个游戏模式出现。拆之前先看两者的真实共性，别提前抽基类
- [ ] 死代码清理（见 README「死代码」表）


- { 主任务 } 拆 `EntityManager`：实体搬进 `PuGameData`，渲染走 `collectView` + `ViewRect`
      为什么是它：回合逻辑要写在 `PuGame::tick`，但实体住在 `EntityManager`（Logic 持有），
      状态够不着自己的单位 —— **它挡在业务前面**
      状态：触发条件已到（第一个实体实例存在），不再是条件性待办

## 编码约束（与构建相关，勿删）

- 源文件 UTF-8 无 BOM
- 控制台输出只用 ASCII
- 调试输出统一 `std::cout` + `std::endl`