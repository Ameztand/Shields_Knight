# Shields_Knight 约定

> 本文件管「版本号」「五份文档的分工」「提交信息」与「自测」，不描述代码。
> 规则自 **V0.1.1** 起生效。

## 1 版本号

`大.小.批次.批次内序号`，例 `V0.1.0.6`。

- **批次**：引入「改变后续代码形状」的改动——接口签名、数据归属、依赖方向。不是"改了很多代码"。
- **批次内序号**：同一批次里的迭代，含试错与修补。
- 批次内序号如果为 `.0`，则省略。例 `V0.1.1` 就是 `V0.1.1.0`。
- 批次起点（`.0`）默认非稳定；批次内修完才是候选稳定。
- 一个版本 = 一到多个 commit，**以 git tag 为准**。工作区代表"上一版之后"，不占版本号。
- 生效起点：**V0.1.1**。此前编号不规则（3 段与 4 段混用），以 commit 信息为准，**不追改历史**。
- **版本号只出现在**：git tag、commit subject、`VERSION.md`、`DEVLOG.md` 的版本手记标题；声明稳定版时 README 可有一行。**`PLAN.md` 不写版本号。**

## 2 稳定与 tag

- 判据：回退锚点。实测跑通主流程，且**相对上一个稳定版有净改善**（bug 更少 / 框架更顺）。不要求零缺陷。
- **默认不声明稳定版。** 声明时：README 写一行「稳定版：`Vx`」，另打 `stable/<版本>` tag。README 没有这一行 = 当前没有稳定版。
- **每版打 tag**（如 `V0.1.1`）；tag 记"当时这么声明过"，**不删**。
- 可撤销：日后发现新问题，改 README 那一行指回更早的稳定版即可，tag 不删。
- V0.1.1 之前 tag 不齐全；**不追认、不补打**。那段历史的回退锚点用 commit，见 `VERSION.md`。
- 发版用 **annotated tag**；message 写 2~3 行 release note（这个版本的关键变化）。

## 3 五份文档分工

**每份一条时间轴，互不重叠。**

| 文件 | 时间轴 | 职责 | 频率 |
|---|---|---|---|
| `README.md` | 慢变 | 是什么 / 怎么搭 / 去哪看 / 一段现状 | 极低 |
| `PLAN.md` | **现在 + 将来** | **只放待办**（触发条件 + `已到` / `未触发`） | 每次任务 |
| `VERSION.md` | **过去（流水）** | 版本 / 日期 / commit / tag / 一句话 | **每版一行** |
| `DEVLOG.md` | **过去（why）** | 现行方法论 / 踩坑记录 / 版本手记 | 有新判断时追加 |
| `CONVENTION.md` | **规则** | 版本号 + 本表 + 提交格式 + 自测 | 极低 |


### 一条事实只能有一个家

| 职责 | 唯一归宿 |
|---|---|
| 版本流水、时间线 | `VERSION.md` |
| 还没做什么、触发条件 | `PLAN.md` |
| 现在坏了什么（可行动的部分） | `PLAN.md`（折成待办） |
| 为什么这么改、否决了什么 | `DEVLOG.md` |
| 卡过哪些坑、怎么排查 | `DEVLOG.md` |
| 跨版本有效的判据 | `DEVLOG.md §现行方法论` |
| 规则 | `CONVENTION.md` |
| 是什么、怎么搭、怎么跑 | `README.md` |

- **每版必动的是 `VERSION.md`**：加一行。`PLAN.md` 按完成情况增删；`DEVLOG.md` 有新判断才加；README 只在构建 / 用法 / 现状变化时改。
- 第二处出现同一事实 → **写指针，不复制**。
- **`PLAN.md` 不写版本号、不写状态矩阵、不写缺陷总表**：缺陷折成待办，死代码只以「清理候选」清单出现，否决理由进 `DEVLOG.md`。
- `DIRECTION.md` 已删除（V0.1.1），内容分到 `PLAN.md` / `DEVLOG.md` / `VERSION.md`；**不再重建**——需要判断时写进 `DEVLOG.md` 的版本手记。
- **完成的改动不进 `PLAN.md`**；只有「改完又生出的待办」才进 `PLAN.md`。§7 第 2 问据此检查。

## 4 DEVLOG 三条硬规则

1. **版本手记 append-only。** 新条目加在最上面；旧条目不改。计划与实际不符 → **新增一段**说明差异。
2. **方法论只增不删。** 某条失效 → 原地标 `已失效（自 Vx）` + 一句原因，**不删**。
3. **"现在"不进 DEVLOG。** 缺陷、状态、待办都在 `PLAN.md`。

> V0.1.1 做过一次文档迁移（旧条目重排、`DIRECTION.md` 删除）；此后按上述规则执行。

**写入前自问：这段话一年后还有用吗？** 没用就别写——`git log` 和 `VERSION.md` 记着。

## 5 提交信息

```text
V0.1.1 refactor(logic): 一句话主改动

```

- subject：`<版本号> <类型>(<范围>): <一句话主改动>`；类型取 `feat` / `refactor` / `fix` / `wip`。
- body：一眼看不出「为什么 / 边界」时才写；写细节与已知问题，一条一行，不重复 subject。

## 6 每次改动后的自测

```text
□ 1. 启动 → Lobby：大厅背景，1 个「开始」按钮
□ 2. 点开始 → Select：白背景，3 个绿方块，只有最右的有反应
□ 3. 点右键 → PuGame：红方块在正中，WASD 能动
□ 4. 悬停/点击按钮：绿方块无变化（临时渲染，暂不校验）
□ 5. Esc → Pause：3 个绿方块，红方块仍在画面上
□ 6. Esc → 回 PuGame：方块位置不变；cnt 在 onSuspend 减半、onResume 加一（看控制台）
□ 7. 反复进出 PuGame 两次：只有一个红方块
```

## 7 发版五问

1. 版本号 ↔ commit ↔ tag 对得上吗？`CMakeLists.txt` 的 `project VERSION` 对齐了吗？
2. `VERSION.md` 是否加了本版一行？`PLAN.md` 是否删了完成项、加了新产生的待办？
3. 本次有没有"未落地却写成现状"的句子？
4. 否决 / 踩坑进 `DEVLOG.md` 了吗？过期条目从 `PLAN.md` 删了吗？
5. 提交前 `git diff --cached --name-status` 对过了吗？build / docs / 资源有没有漏进 body？

## 8 目录约定

> 规则自 **V0.1.2.9** 起生效；判据与踩坑见 `DEVLOG.md` 同名手记。

### 三条不变量

1. **`include/` 是零依赖区。** 其中任何头只允许依赖：标准库、`SFML::System`、以及 `include/` 内部。禁止出现 `#include "logic/…"`、`"input/…"`、`"render/…"`、`"platform/…"`。
2. **头文件必须自足。** 每个 `.h` 单独编得过（自己 include 自己用到的每一个类型）。`.cpp` 可以靠传递。
3. **跨目录 include 一律从 `src` 起算**（`-I ${CMAKE_SOURCE_DIR}/src` 已就位）；同目录/子目录可用短名。不要再写 `../`。

### 目录与允许的依赖方向

| 目录 | 干什么 | 允许 include |
|---|---|---|
| `src/include/` | 零依赖区：基础类型、闭集词表、编译期配置、视图换算 | 标准库、SFML::System、`include/` 内部 |
| `src/input/` | SFML 事件 → `MsgData`（唯一产出者） | `include/` |
| `src/platform/` | 窗口与 OS：建窗、resize、IME | `include/`、SFML::Graphics/Window |
| `src/logic/` | 世界状态、状态机、意图结算 | `include/`、`input/msgData.h` |
| `src/render/` | `RenderData` → 屏幕 | `include/`、`logic/contract/` |
| `src/main.cpp` | 组装与主循环 | 全部 |

**已知例外（仅此一条）**：投影化之前，`render.cpp` 允许 include `logic/button/button.h`——因为 `RenderData` 里装的是 `Button` 本体而非投影。见 `PLAN.md`「快照投影化」；那天这条例外一并删除。

### 加文件前的三问

1. **谁写它？** 只有一个写者 → 跟写者走；一个写者都没有（闭集词表/常量）→ `include/`。
2. **它是不是"只由这一帧的物理输入决定"？** 是 → `input/msgData.h`。需要知道"现在是哪个状态"才能算出来 → 那是**意图**，归 `logic/`。
3. **它依赖外部系统吗？** 依赖 SFML::Graphics / OS 的，永远不进 `include/`。

### 命名

- 名字是**名词**，不是角色：`ButtonStore` / `EntityStore` / `StateStack` / `Cache` 好；`Manager` / `Controller` / `Handler` 不写。
- **同后缀必须同语义**：两个 `Store` 若一个是"唯一真相"、一个是"可重建的镜像"，就得有一个改名。
- **文件名必须等于内容**：`camera.h` 装 `Window`、`renderDataPu.h` 装 `RenderDataEntity`，都算违例。由于历史原因，暂时豁免这两个例子。