#include "logic.h"

#include <memory>

#include "./stateMachine/gameState/gameState.h"
#include "./stateMachine/gameState/puGameState.h"


void Logic::onEnter()
{
	data.initData();
	buttons.initButtonStore();
	//entityStore.init
	stateStack.initGameSta(data);
}

void Logic::translateMsg()
{
	//data.gameIntend.isEsc = (out.Esc == KeySta::falling);
	data.updateDebug(data.getMsgData().F1);
	data.updateAlt(data.getMsgData().Alt);
	//printf("%d\n", data.getBout());
}

void Logic::delCurButtons() {
    buttons.deleteButtons(stateStack.getStaID());
}

void Logic::regCurButtons() {
    buttons.registerButtons(stateStack.getStaID());
}

//void Logic::delCurEntitys() {
//	entityStore.deleteEntitys(stateStack.getStaID(), EntityID::testEntity);
//}
//
//void Logic::regCurEntitys() {
//	const auto sta = gameSta.getStaID();
//	//临时调用
//	if (sta == IGameState::GameSta::PuGame) entityStore.registerEntitys(sta, EntityID::testEntity);
//}

void Logic::replaceSta(std::unique_ptr<IGameState> sta) {
	if(stateStack.getStaID() == GameStateKind::Pause)popSta();//先退出暂停状态

    delCurButtons();
	//delCurEntitys();
    stateStack.replaceSta(data, std::move(sta));
    regCurButtons();
	//regCurEntitys();
}

void Logic::pushSta(std::unique_ptr<IGameState> sta) {
    stateStack.pushSta(data, std::move(sta));
    regCurButtons();
	//regCurEntitys();
}

void Logic::popSta() {
    delCurButtons();
	//delCurEntitys();
    stateStack.popSta(data);
}

void Logic::changeSta()
{
	if (data.gameIntend.popSta){
		popSta();
	}
	else if (data.gameIntend.replaceSelect) {
		replaceSta(std::make_unique<Select>());
	}
	else if (data.gameIntend.replacePuGame) {
		replaceSta(std::make_unique<PuGame>());
	}
	else if (data.gameIntend.pushPause) {
		pushSta(std::make_unique<Pause>());
	}
	else if (data.gameIntend.restartPuGame) {
		replaceSta(std::make_unique<PuGame>());
	}
	else if (data.gameIntend.backLobby) {
		while (!(stateStack.empty())) {
			popSta();
		}
		pushSta(std::make_unique<Lobby>());
	}
}

void Logic::clearIntend()
{
	data.gameIntend = {};
}

void Logic::tick(const MsgData& out, const double now)
{
	//获取数据写入黑板（还没写入时间）
	data.pushMsgData(out);
	data.updataNow(now);
	const float dt = static_cast<float>(data.getDeltaSec());
	translateMsg();//多余？

	buttons.tick(data, stateStack.getStaID());//按钮输入
	//entityStore.tick(data, stateStack.getStaID(), dt);//临时实体控制
	data.updataButtonSta(buttons.commitButtons());//临时按键输入写入黑板,方便状态机获取

	if (auto* s = stateStack.getState()) {   // 空栈保护
		StateContext ctx{ data , data.getMsgData() , buttons.commitButtons()[static_cast<size_t>(s->getID())]  };  // 有界上下文
		s->tick(ctx);
		//换栈前提交数据，或者换栈拖到下一帧？
		updateRenderData();
	}

	//换栈
	changeSta();

	//数据清理
	if (data.gameIntend.initGameData)data.initGameData();
	if (data.gameIntend.initALLData)data.initData();
	clearIntend();
}

void Logic::updateRenderData()
{
	renderData.buttons_ = buttons.commitButtons();
	renderData.gameSta_ = stateStack.getStaStack();
	renderData.gameStaDepth_ = stateStack.getStaDepth();

	renderData.debugPanel_ = data.getDebug();

	//renderData.entitys_ = entityStore.commitEntitys();
	stateStack.getState()->submitRender(renderData.entitys_[static_cast<size_t>(stateStack.getStaID())]);//应该遍历全部状态？
}

// 数据流三条规则：
// 1. 状态产出：只能 push_back 自己负责的条目，不读别人的
// 2. Logic 汇合：把多个生产者合成一帧 + 排序，不加工任何条目
// 3. Render 消费：只按字段拆包，从不回头问"这是谁"
RenderData Logic::commitRenderData()
{
    return renderData;
}

bool Logic::Exit()
{
	return stateStack.empty();
}

bool Logic::resetTime()
{
	return data.gameIntend.resetTime;
}
