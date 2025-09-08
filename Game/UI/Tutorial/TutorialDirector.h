#pragma once
#include <memory>

#include "Engine/Module/Components/2d/Sprite.h"

#include "Game/Information/TutorialType.h"
#include "Game/UI/Tutorial/TutorialUIs.h"
#include "Game/UI/Tutorial/Text/ITutorialTextUI.h"

class TutorialDirector
{
public:

	TutorialDirector() = default;
	~TutorialDirector() = default;

	void Init();

	void Update();

	const bool GetIsMoveEnable()const { return isMoveEnable_; }

private:

	// 猫が出てくる処理
	void SpeechCat_SceneIn();
	// 吹き出しを出す処理
	void SpeechBubble_SceneIn();
	// しゃべっているか判定する
	bool IsSpeechCat(const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis, float& time);


private:

	// 動けるようにするか
	bool isMoveEnable_ = false;

	// チュートリアルのUIを管理
	std::unique_ptr<TutorialUIs> uis_ = nullptr;
	// コントローラーON/OFF
	bool isControllerConnected_ = false;

	// 今何を説明しているかのType
	TutorialType type_ = TutorialType::Self;
	// ボタンを押して説明を聞いたか
	bool isPush_ = false;

	// チュートリアル猫
	float catComes_ = 0.0f;		// 出てくる

	// チュートリアルの吹き出し
	float spComes_ = 0.0f;		// 出てくる

	// 自己紹介しろ
	float selfTimer_ = 0.0f;
	// 移動を教える
	float moveTimer_ = 0.0f;
	int moveChecker_ = 0;
	// ゴーストを作らせる
	float createGhostTimer_ = 0.0f;
	// ゴーストが何故できるかの説明

	// ゴーストを取らせる

	// ゴールさせる

};

