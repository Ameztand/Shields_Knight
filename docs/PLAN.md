# PLAN 待办

> 只放还没做的事；完成即删。
> 为什么 / 否决 / 踩坑 → `DEVLOG.md`；版本号 / 时间线 → `VERSION.md`；规则 → `CONVENTION.md`。
> 状态：`已到` = 现在能做；`未触发` = 等条件。线：`主` = 推进游戏；`支` = 旁支。
> 新行模板：`| 已到 | 支 | 事项 | 触发条件 / 判据 |`

## 待办

| 状态 | 线 | 事项 | 触发条件 / 判据 |
|---|---|---|---|
| 已到 | 主 | `Battle` 阶段机：`Phase` 枚举 + 单点 `switch`；发牌 / 打出 / 结算先用 println 闭环 | 按 S0–S2 顺序推进；跑通前不做动画、不碰渲染契约 |
| 未触发 | 主 | 碰撞箱梯形变形：选中态收窄上沿，命中与绘制共用同一份 `corners()` | 卡牌要画成透视 |
| 未触发 | 主 | 收窄渲染契约：`Config` 运行时结构 + 适配器 + `drawLayer` + 纹理缓存 + 实体帧表 | 要给实体贴图，或要播动画 |
| 未触发 | 主 | 让数据离开代码：JSON 读取 + 校验 + 失败回退 + 手动重载 | 有 3~4 样东西的位置要反复调 |
| 已到 | 支 | dt 单位进名字：`StateContext::dt` → `dtSec`，`Battle` / `EntityStore` 收 dt 的形参统一写 `dtSec` | 刚踩过"秒当毫秒"（见 DEVLOG V0.1.4）；纯改名，一次成型 |
| 已到 | 支 | 悬停结算重构：先倒序扫出 `hoveredID`，再一次性全量刷新；去掉 `!held(MouseL)` 门禁；处理鼠标出界残留；`updateHover` / `clearHover` 合一；删无调用者的 `clearAllHover` | 现状：按住鼠标、或鼠标移出窗口时悬停状态冻结 |
| 已到 | 支 | 悬停参数进配置：`Entity::hoverThreshold`（1.0 秒）每张牌存一份、`Button::hoverThreshold`（200 毫秒）另一份；`render.cpp` 的 1.1 / 1.25 / 0.05 / 0.125 是魔数 | 调一次手感要改三处以上 |
| 已到 | 支 | `HitTest` 从 `battle.cpp` 搬到头文件 | 别的状态 / 渲染也要用命中判定时 |
| 已到 | 支 | `registerEntitys` 一次 spawn 2 个 → Alt 净增 1；拆出 `spawnOne(kind, pos)` | 按 5 次 Alt 得到 7 个方块 |
| 已到 | 支 | 不变量断言 `checkInvariants()`：`setoff.size()` == 活实体数、下标不指向墓碑、`handle` 唯一 | 手写 `if` + 打印，**不用 `assert`**（Release 带 `NDEBUG`） |
| 已到 | 支 | [缺陷] 死槽不渲染：`submitRender` 出口过滤 + `Entity::isAlive()` | 现状：死掉的单位仍会被画出来 |
| 已到 | 支 | [缺陷] 绘制区比命中区大 30%（仅大厅开始按钮） | `fitApply(res, 1.3f)`：命中 200×80，绘制约 260×104 |
| 已到 | 支 | [缺陷] 背景按「栈底是不是 Lobby」猜 | `Renderer::renderBackground` 的 `front() == Lobby` |
| 已到 | 支 | [缺陷] 设计分辨率硬编码两处 | `include/common.h` 的 `BackgroundRect`、`include/config/designView.h`（`FitResult::center` 已在 V0.1.3 消失） |
| 已到 | 支 | [缺陷] 长按阈值与业务耦合：点击只认 `Falling`，`DownL` 无消费者 | `ButtonStore::tick` |
| 已到 | 支 | [验证] 覆盖层（Pause）进出后按钮仍可用 | 跑一遍 Pause 进出；旧判断“已被分格覆盖”待实跑复核 |
| 已到 | 支 | 清理死代码（见下「清理候选」） | — |
| 已到 | 支 | `HandleInput` / `HandleInput2` 去重（4 个函数里 `HandleInput::EscPuase`、`HandleInput2::EscPop` 已无调用者） | — |
| 已到 | 支 | [清理] `platform/camera.*` 里是 `class Window`；`include/utils/` 只有一个头 | 各 1 处 include，随时可做 |
| 未触发 | 支 | `Button` 的长悬停没有任何视觉反馈（`isLongHover` 算出来了但没人用） | 按钮要有第二态贴图 / 描边时 |
| 未触发 | 支 | 确定性回放：`MsgData` 序列 + 固定 dt 喂 `Logic::tick`，期望值写成断言 | 出现「时有时无 / 只在特定顺序下复现」的 bug |
| 未触发 | 支 | [清理] `contract/renderDataPu.h` 名字带 Pu，内容是通用的 `RenderDataEntity` | 出现第二个带实体的状态时 |
| 未触发 | 支 | Select 左/中键接入语义（难度 / 返回） | 语义定下来；Pause 三键已接线，Select 右键已通、左/中未接 |
| 未触发 | 支 | 能力门控表 + 仲裁管线 | 出现「同一对象在不同系统里被区别对待」 |
| 未触发 | 支 | 按游戏模式拆 `GameData` + `src/logic/gameData/` | 第二个游戏模式出现 |
| 未触发 | 支 | `submitRender` 参数由镜像改投影（只带 `pos` / `w` / `h` 一类） | `Entity` 长出 `hp` / `alive` / `animTime`；`render.cpp` 里 `#include "logic/button/button.h"` 那行消失；或需要旋转 / 四角命中时 |
| 未触发 | 支 | `registerEntitys(id)` → `spawn(EntityDef, Vec2 pos)` | 出现第二种单位 |
| 未触发 | 支 | 设计分辨率收敛到单一来源 | 要改设计分辨率之前 |
| 未触发 | 支 | 窗口缩放后鼠标命中的回归验证 | 有人报命中错位 |


## 清理候选（死代码）

- [ ] `Rect::intersects` — `common.h`，无调用者（引入旋转后，AABB 相交会误导）
- [ ] `Intend::replaceLobby` / `pushLobby` / `initGameData` — `intendEvent.h`，定义后无读者
- [ ] `Button::isLongPress` — `button.h`，声明后从不赋值
- [ ] `Logic::translateMsg()` / `PuGame::translateMsg()` — 无调用者（`F1_`/`Alt_` 只写不读）