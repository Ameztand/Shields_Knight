# Shields_Knight

**当前版本：V0.1.0.1 添加逻辑层**

> 作为V0.1.0的子版本，加入README.md，加入CMake，调整md结构，并把项目转移到Shields_Knight仓库

> 还存在修复md描述与实际不符的问题

现状口径：能开窗、能画测试图、能按 Esc 走栈；按钮不显示

## 构建
- 前置：MinGW g++ 14.2.0 / SFML 3.1.0 / CMake ≥3.21
- 三条命令：configure → build → 运行
- ⚠️ 运行 CWD 必须是项目根（renderTEST 用相对路径 ./pic/test.png）

## 现状矩阵
| 模块 | 代码 | 实测 | 说明 |
|---|---|---|---|
| 输入层 | ✅ | ✅ | 五态 KeySta + 鼠标并入，COUNT=KeyCount+1
| 状态机 | ✅ | ✅ | 栈 push/pop/replace，Esc→pop 实测通过
| 按钮层 | ✅ | ⚠️ | 快照可算，但 RenderData 为空 → 不显示
| 渲染层 | ✅ | ✅ | 仅 renderTEST（test.png + 绿圆）
| 长悬停 | ✅ | ❌ | now_ 恒 0，永不触发
| 长按 | ✅ | ❌ | isLongPress 从不赋值

## 已完成 (V0.1.0)
骨架已通，但渲染快照为空，按钮可以点击但是没渲染
- **逻辑层骨架**：`logic` 内部调度 `State ` / `ButtonManager` / `Data`
  三件，分别对应**状态机**、**按钮适配器**、**顶级数据黑板**。

- **数据流贯通**：`Input -> Logic::tick -> Logic::commitRenderData -> Renderer::render`。
  `main` 单循环：`poll -> Exit 判定 -> tick -> commit -> render`。

- **状态机**：栈式 `Lobby / Select / Pause / PuGame`，`onEnter/onExit` 成对，
  `push/pop/replace` 时按钮随之挂载卸载。

- **按钮表驱动**：`ButtonConfig`（区域 + ID，静态配置）与 `Button`（悬
停/点击
  运行时态）分离；按状态查 `kButtonTable`，状态类不再持有按钮列表。
- **黑板**：状态机 `tick(Data&)` 只读输入结构体，**非只读黑板快照**，
**数据和意图还保留搬运的架构，后续打包为上下文**

## 版本记录

| 版本 | 内容 |
|---|---|
| V0.0.1 | 输入重构：`IInputLayer` 抽象 + 五态 `KeySta` + 事件转快照 |
| V0.0.2 | 时间组块（`TimeClock`）；渲染像素改由 `main` 传入预留接口 |
| V0.1.0 | 逻辑层：状态机 + 按钮适配 + 数据黑板；主循环接上 tick/render |
| V0.1.0.1 | 修复md描述与实际不符的问题 |

## 已知问题（V0.1.0 收尾清单）

- [ ] `Logic::commitRenderData()` 的 `RenderData` 尚未填充，快照为空 -> 按钮不显示
- [ ] `MousePos` 为窗口像素，未映射到逻辑坐标 -> 窗口缩放后命中错位
- [ ] `Button::isClicked` 是电平而非下降沿 -> 按住会连续触发
- [ ] `Data::updataNow()` 未调用，`now_` 恒为 0 -> 长悬停失效
- [ ] 下层状态按钮表未注册 -> Pause 无法叠加渲染下层
- [ ] `config_rect.h` 中 Select/Pause 的左键与中键坐标完全重合 -> 中间按钮点不到
- [ ] 本次上传git会把测试图片上传(一张微信截图做测试用例)

## 重构目标 (待办)

- [ ] 渲染由 SFML 重写（尤其是贴合可调整窗口大小），UI 渲染完全重写
- [ ] 后续考虑 `config` 初始化读取 txt 文本