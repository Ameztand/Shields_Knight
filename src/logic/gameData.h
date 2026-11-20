#pragma once

#include <cstdint>
#include "include/common.h"
#include "input/msgData.h"//为了KeySta

//虚拟按钮双缓存
struct DoubleBool {

	bool res = false;
	bool prve = false;

	void update(const KeySta out) {
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

class PuGameData {
private:
	int cnt_ = 0;//测试用例
	uint32_t selectedID_ = 0;

	Position centre = Epos;//拖动开始的锚点
	uint32_t selectedDownLID_ = 0;//拖动开始的选中

	//uint32_t selectedHoverID_ = 0;//悬停对象
public:

	//临时存放
	//F1Debug
	DoubleBool F1_ = {};
	//Alt调试
	DoubleBool Alt_ = {};

	void setCnt(int i) { cnt_ = i; }
	int getCnt() { return cnt_; }

	void setID(uint32_t id) { selectedID_ = id; }
	uint32_t getID() { return selectedID_; }

	void setCentre(const Position& pos) { centre = pos; }
	const Position& getCentre() const { return centre; }

	void setDownLID(uint32_t id) { selectedDownLID_ = id; }
	uint32_t getDownLID() { return selectedDownLID_; }

	//void setHoverID(uint32_t id) { selectedHoverID_ = id; }
	//int getHoverID() { return selectedHoverID_; }

	void initGameData() {}
};
