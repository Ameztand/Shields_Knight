#pragma once

class PuGameData {
private:
	int cnt_ = 0;//测试用例
	uint32_t selectedID_ = 0;

	Position centre = Epos;//拖动开始的锚点
	uint32_t selectedDownLID_ = 0;//拖动开始的选中

	//uint32_t selectedHoverID_ = 0;//悬停对象
public:
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
