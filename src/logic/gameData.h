#pragma once

class PuGameData {
private:
	int cnt = 0;//测试用例
public:
	void setCnt(int i) { cnt = i; }
	int getCnt() { return cnt; }

	void initGameData() {}
};