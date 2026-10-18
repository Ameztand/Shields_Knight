# Shields_Knight

**当前版本：V0.1.0 添加逻辑层**

> 代号：Shields_Knight（原 Chess_Ref_with_SFML 更名为本代号，仓库地址暂未迁移）

## 已完成 (V0.1.0)

- **逻辑层骨架**：`logic` 内部调度 `StateMachine` / `ButtonManager` / `DataStore`
  三件，分别对应**状态机**、**按钮适配器**、**顶级数据黑板**。
- **数据流贯通**：`Input -> Logic::tick -> RenderData -> Renderer::render`。
  `main` 单循环：`poll -> Exit 判定 -> tick -> render`。
- **状态机**：栈式 `Lobby / Select / Pause / PuGame`，`onEnter/onExit` 成对，
  `push/pop/replace` 时按钮随之挂载卸载。
- **按钮表驱动**：`ButtonConfig`（区域 + ID，静态配置）与 `Button`（悬停/点击
  运行时态）分离；按状态查 `kButtonTable`，状态类不再持有按钮列表。
- **黑板**：状态机 `tick(Data&)` **只读黑板快照**，不直接吃输入结构体。

## 版本记录

| 版本 | 内容 |
|---|---|
| V0.0.1 | 输入重构：`IInputLayer` 抽象 + 五态 `KeySta` + 事件转快照 |
| V0.0.2 | 时间组块（`TimeClock`）；渲染像素改由 `main` 传入预留接口 |
| V0.1.0 | 逻辑层：状态机 + 按钮适配 + 数据黑板；主循环接上 tick/render |

## 已知问题（V0.1.0 收尾清单）

- [ ] `Logic::commitRenderData()` 的 `RenderData` 尚未填充，快照为空 -> 按钮不显示
- [ ] `MousePos` 为窗口像素，未映射到逻辑坐标 -> 窗口缩放后命中错位
- [ ] `Button::isClicked` 是电平而非下降沿 -> 按住会连续触发
- [ ] `Data::updataNow()` 未调用，`now_` 恒为 0 -> 长悬停失效
- [ ] 下层状态按钮表未注册 -> Pause 无法叠加渲染下层
- [ ] `config_rect.h` 中 Select/Pause 的左键与中键坐标完全重合 -> 中间按钮点不到
- [ ] 本次上传git会把测试图片上传

## 重构目标 (待办)

- [ ] 渲染由 SFML 重写（尤其是贴合可调整窗口大小），UI 渲染完全重写
- [ ] 后续考虑 `config` 初始化读取 txt 文本