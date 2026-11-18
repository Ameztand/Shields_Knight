# Shields_Knight 开发手记

> 本文件记录 **过去**：为什么这么改、否决了什么、踩在哪里、试过什么错的。
> **「改了什么」由 git 与 `VERSION.md` 记录；「现在坏了什么 / 还没做什么」在 `PLAN.md`；「规则」在 `CONVENTION.md`。**
>
> **三条硬规则**
> 1. **版本手记 append-only**：新条目加在最上面，旧条目不改；计划与实际不符 → **新增一段**差异说明
> 2. **方法论只增不删**：失效就原地标 `已失效（自 Vx）` + 原因
> 3. **「现在」不进本文件**：缺陷、状态、待办都在 `PLAN.md`
>
> 写入前自问：**「这段话一年后还有用吗？」**
> V0.1.1 做过一次文档迁移（旧条目重排、`DIRECTION.md` 删除），此后按上述规则执行。

---

## 现行方法论

> 只增不删。跨版本有效，与具体代码形状无关。

### A 打包与拆分

**1. 生命周期一致 + 写者唯一 → 才该打包成一个 struct。** 有一条"否"就拆。

**2. 一个实例抽不出共性。** 等第二个实例出现再抽象。现在抽出来的"共性"，多半第一个实例的特例。

**3. 派生数据 vs 权威数据 → 决定要不要 Manager。**
- 派生（从输入 / 配置算出来，没有作者）→ 需要中心去算 → Manager 合理（`ButtonStore`）
- 权威（游戏逻辑写进去）→ 作者自己持有（状态持有的 `EntityStore`）
- 给权威数据安排代理作者，就是"别扭"的来源

### B 数据流与边界

**4. 分层 tick 顺序。** 状态机先 tick → 再结算意图 → 最后提交渲染。三个小项目才摸出来，最贵的一条。

**5. 数据与意图分离。** 黑板存"世界是什么"，意图存"我想让世界变成什么"。发意图的人不依赖执行的人，**谁读谁负责**。

**6. 快照装描述，不装对象（投影 vs 镜像）。** 判据：快照的头文件认识几个 domain 类型。认识越多，离"渲染层开始判断游戏规则"越近。
例外：`Entity` 现在很瘦，`vector<Entity>` 镜像暂时无害——但它有到期日。

**7. 栈是控制流，不是渲染清单。** "遍历栈 = 渲染层"只在"每个状态的可见物恰好等于它自己"时成立。
绘制顺序属于层（layer），不属于栈。

**8. 差异必须来自数据表，不能来自代码里的魔法数。** `1.3f` 是坏的（代码里拍脑袋），帧表里的 `offset` 是好的。两张矩形可以不等，但不等的原因必须在表里。

**9. `logic` 不该认识渲染类型。** 渲染只读描述。目前 `logic/` 只依赖 SFML System，不依赖 Graphics——这是最干净的那条线，别破。

### C 时序与增量

**10. dt 由帧循环算一次，对象只消费。**
"每个对象自己存 lastTime"只在每个对象每帧都被 tick 时等价；一旦跳过（暂停、眩晕、被覆盖），对象里的 last 会过期，恢复时一次把欠的时间全还回来。
差别不在减法，在**谁的记录不会过期**。

**11. 跨帧的量用 `+= dt` 累积，不用 `now - start`。**
累积只在被 tick 的那几帧增长 → 暂停、跳过、慢放全部自动正确。绝对差会把所有"没被 tick 的时间"算进去。

**12. 会动的东西用 float 坐标。**
`int` 每帧截断，扔掉的小数永远回不来：慢的会冻住（30px/s @60fps → 0.5px/帧 → 0），快的系统性偏慢，速度还随帧率变。

**13. 每帧全量重建是终态，不是妥协。**
实现上持有一个实例 `clear()` 后复用（`clear()` 不释放 capacity）。第二帧起零分配。

### D 工程节奏

**14. 触发条件驱动，不按"结构债最重"排。**
每写一个待办，必须能说出"什么条件到了才做它"。答不出就是"想做"，不是"该做"。

**15. 一次只动一条所有权边。**
搬家比新增贵，因为要重排所有人的邻居。搬之前写"行为清单"，搬完逐条对；允许丑的中间态。

**16. 无调用者的接口不写。** 答不出"调用者是谁、什么时候调"就不是接口，是负债。

### 不能删的资产

| 资产 | 为什么 |
|---|---|
| `Screen` + `sf::View` letterbox | 正确且完整，能扛任意窗口尺寸 |
| 表驱动的可点注册表（`kButtonTable`） | 加一个按钮 = 加一条数据，不改流程 |
| 栈式状态机 + 四事件 | push / pop / replace 语义干净，覆盖层免费受益 |
| 双工具链 + 字符集处理 | 一份源码 MinGW / MSVC 都能编，VS 可断点，中文不乱码 |
| `logic/` 不依赖 SFML Graphics | 唯一的干净边界 |

---

## 踩坑记录

> 只追加。每条记"症状 → 根因 → 排查手段"。

### 前置声明的 class-key 必须和定义一致（LNK2019 的真凶）

`struct Button` 的定义，前置声明必须也写 `struct Button;`。

MSVC 把 class-key 编进 mangled name（`U`=struct，`V`=class），g++ 的 Itanium ABI 不区分。
结果：**g++ 链接通过，MSVC 报「找不到明明定义了的函数」**。

排查：对定义方和引用方的 `.obj` 跑 `dumpbin /SYMBOLS`，看括号里是 `class` 还是 `struct`。
这招对所有 LNK2019 通用，比重读代码快得多。

### 源码文件编码必须是 UTF-8 无 BOM

`CMakeLists.txt` 开了 `/source-charset:utf-8`，GBK 文件会报 C4819 / C2001。
VS 新建文件默认用系统 ANSI（GBK），要另存为 UTF-8。项目根 `.editorconfig` 已固定 `charset = utf-8`。

### 调试输出一律用 ASCII

执行字符集是 GBK(936)，VS Code 终端默认 UTF-8(65001)。中文要过「源码 → 执行字符集 → 控制台代码页」三关，全角标点尤其容易乱码；ASCII 零关。

### `printf` 与 `std::cout` 不要混用

`printf` 的 `\n` 只在连控制台时刷新，被重定向时是全缓冲；`std::cout` 的 `std::endl` 强制 flush。
混用会出现「cout 有输出、printf 没有」。

### 32 位 MSVC + 64 位 SFML = `SFML_DIR-NOTFOUND`

普通 PowerShell 里跑 `cmake --preset msvc`，会捡到 VS 的默认 `Hostx86` 工具链。SFML 包是 64 位，被 config 的架构检查拒绝 → `REQUIRED` 失败 → 没有 `build.ninja` → `cmake --build` 报「找不到 build.ninja」。

排查：看 `build/msvc/CMakeCache.txt` 里的 `CMAKE_CXX_COMPILER` 是 `Hostx64` 还是 `Hostx86`。
**`CMAKE_CXX_COMPILER` 一旦写进缓存就不会重新检测**，配错了必须删整个 `build/msvc`（VS 开着时删不掉，先关 VS）。

### 构建产物不一致会伪造出假的 LNK2019

`.obj` 是 11 天前的、源码是今天的，照样能链上去，然后报一个"符号不存在"。
判据：同一目录下所有 `.obj` 的时间戳应该在同一分钟内。不是就 clean 重建。

### `cd /d` 是 cmd 的，不是 PowerShell 的

PowerShell 的 `cd` 是 `Set-Location` 别名，没有 `/d` 参数 → `/d` 被当位置参数 → 报错。
PowerShell 里不需要 `/d`（本来就跨盘符）；路径有空格必须加引号。

---

## 版本手记

> 倒序，最新在上。只记"为什么"；"改了什么"见 `VERSION.md` 与 git。

### V0.1.2.1–.9 — 结构重构：目录按「谁写它」重整

> 改了什么见 `VERSION.md` 与 git；这里只记为什么。

#### 为什么按「谁写它」分，而不是按层或按文件类型

**否决一：按调用方（层）分。** `common.h` 被 12 处引用、分属 4 个目录；按层放会让另外三层反向依赖它。`#include` 是文本包含，不是依赖承诺——**读者可以很多，作者通常只有一个**。

**否决二：按文件类型分（声明 / 配置）。** `ButtonConfig` 同时是声明和配置，`Button` 同时是声明和运行期态，按类型分必然逼同一个概念横跨两处。`include/` 原来的病就是这么来的：`config_button_table.h` 为了一个 `size_t` 常量，去 include 一个满是虚函数的接口。

**现行判据：先问「谁能改它」。** 唯一写者 → 跟写者走；无写者（闭集词表/常量）→ `include/`；需要知道"现在是哪个状态"才能算出来的 → 那是意图，归状态。

#### 准入测试：`include/` 的资格是编出来的，不是聊出来的

只给 `-I src/include` + SFML 路径，对每个头跑 `g++ -fsyntax-only -x c++`，编得过才有资格进 `include/`。这把"该不该放这"从判断题变成执行题——`renderData.h` 就是被这条挡在门外的（它认识 `Button` / `Entity`）。

#### 踩坑：`IInputLayer` 不是接口，是命名空间

- 全仓库 **0 处** `IInputLayer&/*`（虚表一次没被用过），却有 **40 处** `IInputLayer::KeySta` 这类限定名。
- 它是 logic 层唯一的 SFML 泄漏源：`logic.h → dataStore.h → IInputLayer.h → screen.h → SFML`。拆出 `input/msgData.h` 后，`logic.cpp` 只给 `-Isrc` 就能编过。
- 结论：**数据契约要独立成头**，别寄居在一个用不上的接口里。接口留不留，等有第二个来源再定。

#### 踩坑：`<array>` 不带 `uint32_t`，`<vector>` 不带 `size_t`

V0.1.2 引入 `EntityHandle{ uint32_t id; }` 后，MinGW 有 5 个 TU 编不过，MSVC 却编得过——因为 `<memory>` / `<iostream>` / `SFML/Graphics/RenderWindow.hpp` 恰好会带出 `uint32_t`，谁先 include 谁就活。
判据：**头文件必须自足**；`.cpp` 靠传递可以接受。

#### 踩坑：`class State` 装的是栈

类名叫 `State`，内容是 `vector<unique_ptr<IGameState>>`，而"状态"的基类也叫 `IGameState`——文件和类、类与基类三方撞词。改名 `GameStateMachine` 后，文件、类、职责三者对齐。

#### 改名对照（本文历史条目保持原名，不追改）

| 旧 | 新 |
|---|---|
| `IInputLayer::MsgData` | `MsgData`（`input/msgData.h`） |
| `IGameState::GameSta` + `kStateCount` | `GameStateKind` + `kStateCount`（`include/gameStateKind.h`） |
| `ButtonID` | `ButtonKind`（`include/buttonKind.h`） |
| `ButtonManager` | `ButtonStore`（`logic/button/`） |
| `EntityManager` | `EntityStore`（`logic/entity/`） |
| `class State` | `GameStateMachine` |
| `include/type_render_data.h` | `logic/contract/renderData.h` |
| `include/type_render_entity.h` | `logic/contract/renderDataPu.h` |
| `include/context/stateContext.h` | `logic/contract/stateContext.h` |
| `include/config_design_view.h` | `include/config/designView.h` |
| `include/config_rect.h` | `include/config/buttonTableRect.h` |

### V0.1.2 — 种类 / 实例 ID 分离

> 改了什么见 `VERSION.md` 与 commit；这里只记为什么。

#### 为什么把 `EntityID` 拆成 `EntityKind` + `EntityHandle`

原 `EntityID` 的实际语义是"这是什么"（种类），却被 `setoff` 当成"这是哪一个"（实例）用：
`std::unordered_map<EntityID, int>` 一张表只有一格，等于默认了「每种类最多一个实例」。
注册第二个同种实体时 `setoff[id] = i` 直接覆盖，前一个槽既不会被 `deleteEntitys` 回收、`kind` 也还是活的——
**泄漏槽继续被渲染、被命中，但表里查不到它**。往卡牌推进时这一步必然要拆（同名牌多张）。

结论：**标量与序列走 `PuGameData`（选中 / 悬停 / 拖拽锚点），实例本体留在 `EntityStore` 的槽里，两边用 handle 当桥。**

#### 踩坑：`Entity temp = manager.getEntity(id)` 是拷贝

`getEntity` 返回 `const Entity&`，但声明成 `Entity temp` 就是拷贝初始化，改 `temp.pos` 改的是栈上的副本，
池子一个字节没动。症状是「拖动完全没反应」，和「没写这段代码」长得一样；编译器只给一个「设了但没读」的警告。
修法：拷贝出来 → 改 → `setEntity` 写回。`getEntity` / `setEntity` 必须成对。

#### 踩坑：单位差 1000 倍

`Data::getDeltaSec()` 是**秒**，而 `Entity::hoverThreshold` 当初按毫秒写 `200.0` → 长悬停要连续悬停 200 秒才亮。
判据：**跨层传时间量时把单位写进变量名**（`dtSec` / `nowMs`），别靠注释——注释会过期。

#### 踩坑：`assert` 在本工程里是空的

`CMakeLists.txt` 强制 `Release`，`CMAKE_CXX_FLAGS_RELEASE` 自带 `-DNDEBUG` / `/DNDEBUG`（见 `build/*/CMakeCache.txt`），
`assert` 整句被编掉、不报错。自检一律手写 `if (!cond) { std::cout << ... }`。

### V0.1.1 — 实体归状态持有

> 改了什么见 `VERSION.md` 与 commit；这里只记为什么。

#### 为什么放弃「实体进 GameData」的原计划

V0.1.0.6 的设想是：实体搬进 `PuGameData`，渲染走 `collectView` + `View`。
实际做成的是：实体留在 `EntityStore` 里，只把**持有者**从 `Logic` 换成状态自身。

判据见方法论 3：`EntityStore` 现在有**真实职责**（槽位复用 `freeList_` + ID→下标 `setoff`），
而原计划的三条理由（状态够不着 / 需维护注册三件套 / 生命周期不跟状态）在持有者改变后**全部消失**。

**给未来的自己**：想拆它之前，先看它现在有没有职责。

#### 为什么接缝叫 `submitRender` 而不是 `collectView`

原设想是"`Logic` 遍历整个栈，每个状态 `collectView` 往 `View` 里 push"。
实际是"`Logic` 只调**栈顶**的 `submitRender`，各状态往自己那一格写"。

差别在**载荷**：现在是 `vector<Entity>` **镜像**，不是中立的投影。
镜像暂时无害（`Entity` 只有 `ID / pos / vel / w / h / speed`），
但 `Entity` 长出 `hp / alive / animTime` 那天，渲染层就会拿到逻辑专属字段——那时必须改投影。
**红线**：哪天想在 `render.cpp` 里写 `if (hp <= 0)`，就是改投影的信号。

#### 为什么 `renderData` 留在 `Logic` 成员上（不改成每帧局部）

`Logic::updateRenderData()` 只填栈顶那一格，而 `renderData` 是 `Logic` 的**成员**（不是每帧新建）。
所以暂停时 `entitys_[Pause]` 被填空，`entitys_[PuGame]` **保留上一帧的值**；渲染遍历整个栈时照样画出 PuGame 的实体。

效果是对的（世界冻结仍可见），但**它是"旧值没被清"这个副作用**。
哪天为了"单帧唯一真相"把 `renderData` 改成局部变量，**世界会在暂停时消失**——而且很难查。

#### 计划与实际的差异：`Entity::alive` 没有落地

- V0.1.0.6 里写的"临时裁决者 `Entity::alive`"**没有落地**；`Entity` 上没有 `alive` 字段（身份字段只有 `ID`）。
- `deleteEntitys` 只把 `ID` 置 `Null`，实体仍在 `vector` 里，`submitRender` 整份交出 → 死槽会被画出来。
  这条差异的现状与待办见 `PLAN.md`。

#### 为什么重排文档：PLAN 只留待办

V0.0.0–V0.1.0 只有一份 `README_PLAN.md`（已完成 / 版本记录 / 已知问题 / 重构目标）。
V0.1.0.1 拆成 README + PLAN + DEVLOG 时，PLAN 把版本记录和已知问题一起带走了；
后来状态矩阵、缺陷表、死代码表、迁移执行单继续往 PLAN 里堆，PLAN 变成了五合一。

V0.1.1 起按时间轴分家：

- `VERSION.md`：版本流水（版本 / 日期 / commit / tag）
- `PLAN.md`：只放待办
- `DEVLOG.md`：为什么、否决、踩坑
- `CONVENTION.md`：规则

#### 为什么删掉 `DIRECTION.md`

它原本是"一次性的 AI 指南"，V0.1.0.6 才加，V0.1.1 就删。内容分三处：
次序 / 触发条件 → `PLAN.md`；判据 / 否决 → 本文件；版本流水 → `VERSION.md`。
留着它只会多一个会过期的"第二真相"。

---

### V0.1.0.7 — dt 与 float 坐标

#### 为什么 dt 中央算一次

"每个对象自己存 `lastTime`"只在**每个对象每帧都被 tick** 时等价。
一旦跳过（暂停、眩晕、状态被覆盖），对象里的 `last` 会过期，恢复时一次把欠的时间全还回去。
`Data` 里那个 `last` 每帧都被刷新——差别不在减法，在**谁的记录不会过期**。

#### 为什么坐标必须 float

`int` 每帧截断，扔掉的小数永远回不来：
慢的会冻住（30px/s @60fps → 0.5px/帧 → 0），快的系统性偏慢，而且速度随帧率变。

`300px/s ÷ 60fps = 正好 5.0` 恰好掩盖了这个问题——换成 250 或 144fps 立刻暴露。
**验收 dt 生效的唯一方法**：锁 30 帧和锁 144 帧，走一样快。

#### 输入法会吃掉 WASD

中文输入法处于中文态时，字母键被 IME 拿去组词，游戏收不到。
修法：`ImmAssociateContext(hwnd, nullptr)` 建窗后调一次（`imm32` 要进 CMake 的链接列表，MinGW 的 `#pragma comment(lib)` 无效）。

---

### V0.1.0.6 — 状态生命周期四事件

> 当时的设计意图是 `collectView(View&)` 成为接缝；**实际最终改为 `submitRender`（只提交栈顶），见 V0.1.1 手记**。

#### 为什么加 `onSuspend` / `onResume`

「进入暂停就 `cnt /= 2`」这种规则：不该写进 `Pause`（UI 不该懂游戏规则），也不该在每个触发暂停的调用点手写（规则会散落）。

**缺的不是接口，是状态机的「被覆盖」事件。** 补上之后规则只有一份，任何覆盖层（背包、对话框、商店）压上来都自动生效。

生命周期调用序列：

| 动作 | 序列 |
|---|---|
| 启动（栈空） | `Lobby.onEnter` |
| replace（栈 1） | `旧.onExit` → `新.onEnter` |
| push（栈 1） | `栈顶.onSuspend` → `新.onEnter` |
| pop（栈 2） | `栈顶.onExit` → `新栈顶.onResume` |
| pop（栈 1） | `旧.onExit`（栈空，无 resume） |
| 暂停中重开（栈 2） | `Pause.onExit` → `PuGame.onResume` → `PuGame.onExit` → `新.onEnter` |

**最后一行是坑**：`replaceSta` = `popSta` + `pushSta`，所以「恢复」紧接「销毁」。
`onResume` 里不要放必须持久、必须成对的逻辑。

各回调的职责判据：**对象还活着吗？**

| 情形 | 回调 |
|---|---|
| 新建 / 销毁 | `onEnter` / `onExit` |
| 活着但被盖住 / 重新露出 | `onSuspend` / `onResume` |
| 每帧 | `tick` |

#### 为什么 `GameData` 不放进 `Data`

判据是方法论 1：生命周期是否一致？写者是否唯一？

`now_` / 输入快照 / 意图是「每帧被覆盖」，`cnt` 是「跨帧累积」——不一致，拆。
`Data` 的身份是**本帧通信板**，不是世界状态。它叫 `Data` 是因为最初「什么数据都往这放」，但内容其实已经很专一。

---

### V0.1.0 — 逻辑层 + 按钮为什么重做

#### 这版做了什么

- 新增 `src/logic/`，把主干接通：`Input → Logic::tick → RenderData → Renderer::render`
- 状态机：栈式 `Lobby / Select / Pause / PuGame`，`onEnter/onExit` 成对，`push/pop/replace` 时按钮随之挂载/卸载
- 按钮表驱动：`ButtonConfig`（区域 + ID，静态配置）与 `Button`（运行时态）分离
- 状态机 `tick(Data&)` 只读黑板快照，不再直接吃输入结构体

#### 按钮版 v1（错的）

`class Button` 持有 `isHovered / isPressed / hoverStartTime`，
由状态的 `onEnter` 注册、`onExit` 销毁，配一个 `ButtonStore`。

**结果：写不下去。** 三个症状：

- 悬停/按下本是**输入派生**，却被存进控件 → 必须有注册 / 同步 / 销毁三件套
- 渲染快照在注册那一刻生成、之后不再更新 → 点击无视觉反馈
- 状态自己维护按钮列表，和 `kButtonTable` 表重复硬编码

**根因：** 把"每帧都变的派生数据"存进了"跨帧存活的对象"。
生命周期不一致 → 必然需要一个协调者（`ButtonStore` 就是那个被迫诞生的角色）。

#### 按钮版 v2（现行判断）

**按钮不是控件，是"表 + 每帧由输入算出的当帧快照"。**

- 静态：`ButtonConfig{ area, ID }` 放 `kButtonTable`（表驱动，状态不持有）
- 动态：`Button{ 悬停/点击/长按/长悬停 }` 每帧由输入统一算
- 语义：按钮只产 `ButtonKind`，**翻译成什么意图由状态决定**，按钮不知道游戏语义

**判断一条"该不该打包成 struct"的规则** → 升级为方法论 1。

#### 其他这版的决定

- **鼠标并入五态**：输入层 `translate()` 里鼠标左键与键盘共用同一套 `Falling/DownS/DownL/Rising` 判定（`COUNT = KeyCount + 1`）。鼠标不该单独写一套状态机。
- **退出交给状态机**：`main` 不再直接判 Esc 退出，改为 `logic.Exit()`（状态栈空）。
- **主循环四段式**：`poll → Exit 判定 → tick → commitRenderData → render`。

#### 黑板：局部反推全局

起点是局部黑板（一个模块内的共享状态），几个项目后反推成全局黑板。关键的区分：

- **逻辑上的全局**（好）：本帧本游戏的全部知识，所有决策都从它读。
- **物理上的全局**（坏）：单例 / `static` / 谁都能拿到的 `GetData()`。

**规矩**：黑板是"逻辑上的唯一知识源"，但必须是"**物理上的私有成员 + 显式传参**"。
一旦某个模块能绕开参数拿到黑板，它就变成了隐式依赖。

---

### V0.0.x

初始化 / 输入重构（`IInputLayer` + 五态 `KeySta`）/ 时间组块（`TimeClock`）。无决策记录。

---

## 唠嗑

上面的信息多数是 ai 提炼总结。因为这是第一次写开发文档，让 ai 反推文档经历过。

其实非常痛苦，因为我直接把上一个小项目的架构照搬过来，体量不匹配，光是抽象出一个按钮适配就走了不少弯路。试过状态自己持有、logic 持有状态注册，最后走适配器。

基本上，就是打通了数据流，勉强能编译运行。不管了，先交 git 睡觉，有啥待办 bug 什么的明天再说。

2026.10.19 01:15
