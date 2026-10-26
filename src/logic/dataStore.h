#pragma once

#include "../include/common.h"
#include "../include/config_button_table.h"
#include "../input/IInputLayer.h"
#include "./stateMachine/gameStateMachine.h"

#include <array>
#include <vector>

//虚拟按钮双缓存
struct DoubleBool {
	using KeySta = IInputLayer::KeySta;

	bool res = false;
	bool prve = false;

	void update(const IInputLayer::KeySta out) {
		bool purr = false;
		if (out == KeySta::Falling) {
			prve = !prve;
			//printf("开\n");
		}
		if (out == KeySta::DownL) {
			prve = false;
			//printf("关\n");
		}
		if (out != KeySta::Null) {
			purr = true;
		}
		res = (prve || purr);
	}

	const bool get() { return res; }
};

struct Intend {
	bool replaceLobby = false;
	bool replaceSelect = false;
	bool replacePuGame = false;

	bool pushLobby = false;
	bool pushPause = false;
	bool restartPuGame = false;
	
	bool backLobby = false;

	bool popSta = false;

	bool initGameData = false;
	bool initALLData = false;
};


class Data {
private:
	using MsgData = IInputLayer::MsgData;
	//using GameSta = IGameState::GameSta;

	double now_;

	MsgData cacheMsgData_ = {};
	std::array<std::vector<Button>, kStateCount> buttons_;//全部按钮

	//F1Debug
	DoubleBool F1_ = {};
	//Alt调试
	DoubleBool Alt_ = {};

public:
	//意图
	Intend gameIntend;

	//时间戳
	void updataNow(const double now) { now_ = now; }
	const double getNow() const { return now_; }

	//获取输入
	void pushMsgData(const MsgData& out)
	{
		cacheMsgData_ = out;
		//时间戳
	}
	const MsgData& getMsgData()const{ return cacheMsgData_; }

	//按钮
	void updataButtonSta(const std::array<std::vector<Button>, kStateCount>& buttons) { buttons_ = buttons; }
	const std::vector<Button>& getButtonOf(const IGameState::GameSta sta) const { return buttons_[static_cast<size_t>(sta)]; }
	//const std::array<std::vector<Button>, kStateCount>& getButtonAll() const { return buttons_; }

	//Debug
	void updateDebug(const IInputLayer::KeySta out){ F1_.update(out); }
	const bool getDebug(){ return F1_.get(); }

	//ALt
	void updateAlt(const IInputLayer::KeySta out){ Alt_.update(out); }
	const bool getAlt(){ return Alt_.get(); }

	//清理意图
	void clearIntend(){ gameIntend = {}; }

	//清理全部数据
	void initData()
	{
		initGameData();
		clearIntend();
	}

	//清理游戏数据
	void initGameData()
	{
		
	}
};