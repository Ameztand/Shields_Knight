#include "logic.h"

#include <memory>

#include "./stateMachine/gameState/gameState.h"
#include "./stateMachine/gameState/puGameState.h"

#include "logic/contract/stateContext.h"
#include "logic/contract/renderDataPu.h"
#include "logic/intendEvent.h"
#include "logic/button/button.h"
#include "input/msgData.h"

struct StateContext;


void Logic::onEnter()
{
	buttons.initButtonStore();
	StateContext ctx{ intend, 0.0f, cacheMsgData, buttons.commitButtons()[static_cast<size_t>(stateStack.getStaID())]  };  // 有界上下文
	stateStack.initGameSta(ctx);
}

void Logic::translateMsg()
{
	//data.gameIntend.isEsc = (out.Esc == KeySta::falling);
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

void Logic::replaceSta(StateContext& ctx, std::unique_ptr<IGameState> sta) {
	if(stateStack.getStaID() == GameStateKind::Pause)popSta(ctx);//先退出暂停状态

    delCurButtons();
	//delCurEntitys();
    stateStack.replaceSta(ctx, std::move(sta));
    regCurButtons();
	//regCurEntitys();
}

void Logic::pushSta(StateContext& ctx,std::unique_ptr<IGameState> sta) {
    stateStack.pushSta(ctx, std::move(sta));
    regCurButtons();
	//regCurEntitys();
}

void Logic::popSta(StateContext& ctx) {
    delCurButtons();
	//delCurEntitys();
    stateStack.popSta(ctx);
}

void Logic::changeSta(StateContext& ctx)
{
	if (intend.popSta){
		popSta(ctx);
	}
	else if (intend.replaceSelect) {
		replaceSta(ctx, std::make_unique<Select>());
	}
	else if (intend.replacePuGame) {
		replaceSta(ctx, std::make_unique<PuGame>());
	}
	else if (intend.pushPause) {
		pushSta(ctx, std::make_unique<Pause>());
	}
	else if (intend.restartPuGame) {
		replaceSta(ctx, std::make_unique<PuGame>());
	}
	else if (intend.backLobby) {
		while (!(stateStack.empty())) {
			popSta(ctx);
		}
		pushSta(ctx, std::make_unique<Lobby>());
	}
}

void Logic::tick(const MsgData& out, const float dt)
{
	//获取数据写入黑板（还没写入时间）
	cacheMsgData = out;
	translateMsg();//多余？

	buttons.tick(stateStack.getStaID(), cacheMsgData, dt);//按钮输入

	StateContext ctx{ intend, dt , cacheMsgData , buttons.commitButtons()[static_cast<size_t>(stateStack.getStaID())]  };  // 有界上下文
	if (auto* s = stateStack.getState()) {   // 空栈保护
		
		s->tick(ctx);
		//换栈前提交数据，或者换栈拖到下一帧？
		updateRenderData();
	}

	//换栈
	changeSta(ctx);

	//数据清理
	clearIntend(intend);
}

void Logic::updateRenderData()
{
	renderData.buttons_ = buttons.commitButtons();
	renderData.gameSta_ = stateStack.getStaStack();
	renderData.gameStaDepth_ = stateStack.getStaDepth();

	//renderData.debugPanel_ = data.getDebug();

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
	return intend.resetTime;
}
