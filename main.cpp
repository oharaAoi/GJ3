#include <Windows.h>
#include "Game/RunTime/GameCore.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	D3DResourceLeakChecker debugChecker;

	std::unique_ptr<AoiFramework> game = std::make_unique<GameCore>();
	game->Run();

	return 0;
}