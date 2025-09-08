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

	void SetCreateGhost(bool flag) { isCreateGhost_ = flag; }
	void SetGhostCount(uint32_t count) { ghostCount_ = count; }

	void TutorialReset();

private:
	// 猫がしゃべってボタンを押すScene(0,3,6兼用)
	void SpeechCatScene(
		const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis,
		float& time,
		TutorialType nextType
	);

	// 猫が出てくる処理
	void SpeechCat_SceneIn();
	// 猫が出て行く処理
	void SpeechCat_SceneOut();

	// 吹き出しを出す処理
	void SpeechBubble_SceneIn();
	// 吹き出しが出て行く処理
	void SpeechBubble_SceneOut();

	// しゃべっているか判定する
	bool IsSpeechCat(const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis, float& time);
	// しゃべった後にボタンを押したか判定
	void PushButton();
	// しゃべった文字を消す処理
	void DeleteSpeechUI(
		const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis, 
		float& time,
		TutorialType nextType
	);

	void SetColorUIs(const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis, const Vector4& color);

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
	float drawGhostBlockTimer_ = 0.0f;		// ゴーストブロックのスプライトを出す
	float createGhostTimer_ = 0.0f;
	bool isSpeechFinish_ = false;
	bool isCreateGhost_ = false;
	// ゴーストが何故できるかの説明
	float explanaTimer_ = 0.0f;
	// ゴーストを取らせる
	float collisionTimer_ = 0.0f;
	uint32_t ghostCount_ = 0;
	// ゴールさせる
	float goalTimer_ = 0.0f;
	// リセット処理
	float skipTimer_ = 1.0f;
	bool isSkip_ = false;
};

