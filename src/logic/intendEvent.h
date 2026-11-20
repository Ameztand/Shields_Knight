#pragma once

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
	bool resetTime = false;
};

inline constexpr void clearIntend(Intend& intend) {
    intend = {};
}