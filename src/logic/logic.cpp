#include "logic.h"

#include <memory>

#include "./stateMachine/gameState/gameState.h"
#include "./stateMachine/gameState/puGameState.h"


void Logic::onEnter()
{
	data.initData();
	buttonManager.initButtonManager();
	//entityManager.init
	gameSta.initGameSta(data);
}

void Logic::translateMsg()
{
	//data.gameIntend.isEsc = (out.Esc == KeySta::falling);
	data.updateDebug(data.getMsgData().F1);
	data.updateAlt(data.getMsgData().Alt);
	//printf("%d\n", data.getBout());
}

void Logic::delCurButtons() {
    buttonManager.deleteButtons(gameSta.getStaID());
}

void Logic::regCurButtons() {
    buttonManager.registerButtons(gameSta.getStaID());
}

//void Logic::delCurEntitys() {
//	entityManager.deleteEntitys(gameSta.getStaID(), EntityID::testEntity);
//}
//
//void Logic::regCurEntitys() {
//	const auto sta = gameSta.getStaID();
//	//临时调用
//	if (sta == IGameState::GameSta::PuGame) entityManager.registerEntitys(sta, EntityID::testEntity);
//}

void Logic::replaceSta(std::unique_ptr<IGameState> sta) {
	if(gameSta.getStaID() == GameStateKind::Pause)popSta();//先退出暂停状态

    delCurButtons();
	//delCurEntitys();
    gameSta.replaceSta(data, std::move(sta));
    regCurButtons();
	//regCurEntitys();
}

void Logic::pushSta(std::unique_ptr<IGameState> sta) {
    gameSta.pushSta(data, std::move(sta));
    regCurButtons();
	//regCurEntitys();
}

void Logic::popSta() {
    delCurButtons();
	//delCurEntitys();
    gameSta.popSta(data);
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
		while (!(gameSta.empty())) {
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

	buttonManager.tick(data, gameSta.getStaID());//按钮输入
	//entityManager.tick(data, gameSta.getStaID(), dt);//临时实体控制
	data.updataButtonSta(buttonManager.commitButtons());//临时按键输入写入黑板,方便状态机获取

	if (auto* s = gameSta.getState()) {   // 空栈保护
		StateContext ctx{ data , data.getMsgData() , buttonManager.commitButtons()[static_cast<size_t>(s->getID())]  };  // 有界上下文
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
	renderData.buttons_ = buttonManager.commitButtons();
	renderData.gameSta_ = gameSta.getStaStack();
	renderData.gameStaDepth_ = gameSta.getStaDepth();

	renderData.debugPanel_ = data.getDebug();

	//renderData.entitys_ = entityManager.commitEntitys();
	gameSta.getState()->submitRender(renderData.entitys_[static_cast<size_t>(gameSta.getStaID())]);//应该遍历全部状态？
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
	return gameSta.empty();
}

bool Logic::resetTime()
{
	return data.gameIntend.resetTime;
}
