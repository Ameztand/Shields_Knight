#pragma once


//#include "common.h"

#include "input/msgData.h"
#include "logic/contract/renderData.h"
#include "logic/stateMachine/gameStateMachine.h"
#include "logic/button/buttonStore.h"
#include "logic/intendEvent.h"



class Logic {
private:
	Intend intend;
	ButtonStore buttons;
	GameStateMachine stateStack;
	
	MsgData cacheMsgData;//在这里处理alt/F1等数据？

	RenderData renderData;

	void onEnter();
	void translateMsg();

	void delCurButtons();
	void regCurButtons();
	//void delCurEntitys();
	//void regCurEntitys();
	void replaceSta(StateContext& ctx, std::unique_ptr<IGameState> sta);
	void pushSta(StateContext& ctx, std::unique_ptr<IGameState> sta);
	void popSta(StateContext& ctx);
	void changeSta(StateContext& ctx);
public:
	Logic() { onEnter(); }//包含入场onEnter

	void tick(const MsgData& out, const float dt);

	void updateRenderData();
	RenderData commitRenderData();//不要加const，为什么？

	bool Exit();

	bool resetTime();

	//析构调用状态机的onExit//要结合拷贝构造？
	//~Logic() {
    //	while (!gameSta.empty()) gameSta.popSta(data);   // 保证每个状态都走一次 onExit
	//}
};
