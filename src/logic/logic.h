#pragma once


//#include "common.h"

#include "input/msgData.h"
#include "logic/contract/renderData.h"
#include "logic/dataStore.h"
#include "logic/stateMachine/gameStateMachine.h"
#include "logic/button/ButtonStore.h"



class Logic {
private:
	Data data;
	ButtonStore buttons;
	GameStateMachine stateStack;
	

	RenderData renderData;

	void onEnter();
	void translateMsg();

	void delCurButtons();
	void regCurButtons();
	//void delCurEntitys();
	//void regCurEntitys();
	void replaceSta(std::unique_ptr<IGameState> sta);
	void pushSta(std::unique_ptr<IGameState> sta);
	void popSta();
	void changeSta();
	
	void clearIntend();
public:
	Logic() { onEnter(); }//包含入场onEnter

	void tick(const MsgData& out, const double now);

	void updateRenderData();
	RenderData commitRenderData();//不要加const，为什么？

	bool Exit();

	bool resetTime();

	//析构调用状态机的onExit//要结合拷贝构造？
	//~Logic() {
    //	while (!gameSta.empty()) gameSta.popSta(data);   // 保证每个状态都走一次 onExit
	//}
};
