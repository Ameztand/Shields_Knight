#pragma once


//#include "common.h"

#include "./dataStore.h"
#include "./stateMachine/gameStateMachine.h"
#include "./buttonManager/buttonManager.h"
#include "../include/type_render_data.h"
#include"../input/IInputLayer.h"


class Logic {
private:
	Data data;
	ButtonManager buttonManager;
	State gameSta;
	

	//RenderData renderData;

	void onEnter();
	void translateMsg();

	void delCurButtons();
	void regCurButtons();
	void replaceSta(std::unique_ptr<IGameState> sta);
	void pushSta(std::unique_ptr<IGameState> sta);
	void popSta();
	void changeSta();
	
	void clearIntend();
public:
	Logic() { onEnter(); }//包含入场onEnter

	void tick(const IInputLayer::MsgData& out, const double now);

	RenderData commitRenderData();//不要加const，为什么？

	bool Exit();

	bool resetTime();

	//析构调用状态机的onExit//要结合拷贝构造？
	//~Logic() {
    //	while (!gameSta.empty()) gameSta.popSta(data);   // 保证每个状态都走一次 onExit
	//}
};
