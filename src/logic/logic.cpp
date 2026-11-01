#include "logic.h"

#include <memory>

#include "./stateMachine/gameState.h"


void Logic::onEnter()
{
	data.initData();
	buttonManager.initButtonManager();
	gameSta.initGameSta(data);
}

void Logic::translateMsg()
{
	//const IInputLayer::MsgData& out = data.getMsgData();
	using KeySta = IInputLayer::KeySta;
	
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

void Logic::replaceSta(std::unique_ptr<IGameState> sta) {
	if(gameSta.getStaID() == IGameState::GameSta::Pause)popSta();//先退出暂停状态

    delCurButtons();
    gameSta.replaceSta(data, std::move(sta));
    regCurButtons();
}

void Logic::pushSta(std::unique_ptr<IGameState> sta) {
    gameSta.pushSta(data, std::move(sta));
    regCurButtons();
}

void Logic::popSta() {
    delCurButtons();
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

void Logic::tick(const IInputLayer::MsgData& out, const double now)
{
	//获取数据写入黑板（还没写入时间）
	data.pushMsgData(out);
	data.updataNow(now);
	translateMsg();

	buttonManager.tick(data, gameSta.getStaID());//按钮输入
	data.updataButtonSta(buttonManager.commitButtons());//临时按键输入写入黑板,方便状态机获取

	if (auto* s = gameSta.getState()) {   // 空栈保护
		StateContext ctx{ data };  // 有界上下文
		s->tick(ctx);
	}

	//换栈
	changeSta();

	//数据清理
	if (data.gameIntend.initGameData)data.initGameData();
	if (data.gameIntend.initALLData)data.initData();
	clearIntend();
}

RenderData Logic::commitRenderData()
{
	RenderData renderData;

	renderData.buttons_ = buttonManager.commitButtons();
	renderData.gameSta_ = gameSta.getStaStack();
	renderData.gameStaDepth_ = gameSta.getStaDepth();

	renderData.debugPanel_ = data.getDebug();

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
