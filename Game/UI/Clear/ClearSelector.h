#pragma once
#include <memory>

#include "Game/UI/Button/ButtonType.h"
#include "Game/UI/Clear/ClearUIs.h"

class ClearSelector
{
public:

	ClearSelector() = default;
	~ClearSelector() = default;

	void Init();

	void Update();

	bool GetChangeScene() const { return changeScene_; }
	bool GetChangeStage() const { return changeStage_; }
	bool GetIsExecute() const{ return isExecute_; }

private:

	// クリアシーンのUI
	std::unique_ptr<ClearUIs> clearUIs_ = nullptr;
	// シーン切り替えを選択したら
	bool changeScene_ = false;
	bool changeStage_ = false;
	bool isExecute_ = false;
	// 切り替え時間
	float changeTimer_ = 0.0f;


	// 今の選択したカーソルIndex 0 = Select,1 = nextScene
	int cursorIndex_ = 1;

};

