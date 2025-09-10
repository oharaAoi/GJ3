#include "TutorialDirector.h"

#include "Engine/System/Audio/AudioPlayer.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Lib/Math/Easing.h"

void TutorialDirector::Init()
{
	uis_ = std::make_unique<TutorialUIs>();
	uis_->Init();
}

void TutorialDirector::Update()
{
	Input* input = Input::GetInstance();

	isControllerConnected_ = input->IsControllerConnected();

	bool speechSkipKey = input->IsTriggerButton(XInputButtons::BUTTON_A) || input->IsTriggerKey(DIK_SPACE);

	const float speechSpeed = 500.0f;

	switch (type_)
	{
	case TutorialType::Self:
	{
		if (isSkip_) { break; }
		SpeechCatScene(uis_->GetSelfUIs(), selfTimer_, TutorialType::Move);
		if (!isSpeechSkip_ && speechSkipKey) {
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			isSpeechSkip_ = true;
		}
	}

	break;
	case TutorialType::Move:
	{
		if (isSkip_) { break; }
		bool key =
			input->IsTriggerKey(DIK_A) || input->IsTriggerKey(DIK_S) ||
			input->IsTriggerKey(DIK_W) || input->IsTriggerKey(DIK_D) ||
			input->IsTriggerKey(DIK_UP) || input->IsTriggerKey(DIK_DOWN) ||
			input->IsTriggerKey(DIK_LEFT) || input->IsTriggerKey(DIK_RIGHT);
		bool pad =
			input->IsTriggerButton(XInputButtons::DPAD_UP) ||
			input->IsTriggerButton(XInputButtons::DPAD_DOWN) ||
			input->IsTriggerButton(XInputButtons::DPAD_LEFT) ||
			input->IsTriggerButton(XInputButtons::DPAD_RIGHT);
		bool stick =
			input->GetLeftJoyStick(0.2f).x > 0.5f ||
			input->GetLeftJoyStick(0.2f).x < -0.5f ||
			input->GetLeftJoyStick(0.2f).y > 0.5f ||
			input->GetLeftJoyStick(0.2f).y < -0.5f;


		if (catComes_ == 0.0f) {
			isMoveEnable_ = true;
			if (moveChecker_ >= 2) {
				if (moveTimer_ > 0.0f) {
					moveTimer_ -= GameTimer::DeltaTime();
				} else {
					type_ = TutorialType::CreateGhost;
					isMoveEnable_ = false;
					isSpeechSkip_ = false;
				}
			}
			if (key || pad || stick) {
				++moveChecker_;
			}
			return;
		}


		// テキストを読む
		bool isFinish = true;
		for (uint32_t i = 0; i < static_cast<uint32_t>(uis_->GetModeUIs().size() - 1); ++i) {
			// パッドかキーボードか
			if (i == 1) {
				if (isControllerConnected_) {
					uis_->GetModeUIs()[i]->GetSprite()->SetEnable(true);
					uis_->GetModeUIs()[i + 1]->GetSprite()->SetEnable(false);
				} else {
					uis_->GetModeUIs()[i]->GetSprite()->SetEnable(false);
					uis_->GetModeUIs()[i + 1]->GetSprite()->SetEnable(true);
				}
			}
			// 読み終わっていなければ
			if (i == 0) {
				const float textureSize = speechSpeed / uis_->GetModeUIs()[i]->GetSprite()->GetTextureSize().x;
				if (!uis_->GetModeUIs()[i]->GetIsFinished()) {
					moveTimer_ += GameTimer::DeltaTime() * textureSize;
					float uv_x = moveTimer_ - static_cast<float>(i);
					uis_->GetModeUIs()[i]->SetUvMaxSize(uv_x);
					uis_->GetModeUIs()[i]->GetSprite()->SetEnable(true);
					if (moveTimer_ > static_cast<float>(i + 1)) {
						moveTimer_ = static_cast<float>(i + 1);
						uis_->GetModeUIs()[i]->SetIsFinished(true);
					}
					isFinish = false;
					break;
				}
			} else {
				uint32_t j = i + 1;
				const float textureSize = speechSpeed / uis_->GetModeUIs()[i]->GetSprite()->GetTextureSize().x;
				if (!uis_->GetModeUIs()[i]->GetIsFinished()) {
					moveTimer_ += GameTimer::DeltaTime() * textureSize;
					float uv_x = moveTimer_ - static_cast<float>(i);
					uis_->GetModeUIs()[i]->SetUvMaxSize(uv_x);
					uis_->GetModeUIs()[j]->SetUvMaxSize(uv_x);
					if (moveTimer_ > static_cast<float>(i + 1)) {
						uis_->GetModeUIs()[i]->SetIsFinished(true);
						uis_->GetModeUIs()[j]->SetIsFinished(true);
						moveTimer_ = 3.0f;
					}
					isFinish = false;
					break;
				}
			}
		}
		if (isSpeechSkip_ && !isFinish) {
			for (size_t i = 0; i < uis_->GetModeUIs().size(); ++i) {
				if (i == 0) {
					uis_->GetModeUIs()[i]->SetUvMaxSize(1.0f);
					uis_->GetModeUIs()[i]->GetSprite()->SetEnable(true);
					uis_->GetModeUIs()[i]->SetIsFinished(true);
				} else {
					uis_->GetModeUIs()[i]->SetUvMaxSize(1.0f);
					uis_->GetModeUIs()[i]->SetIsFinished(true);
				}
			}
			moveTimer_ = 3.0f;
		}

		// テキストが読み終わっていたら
		if (isFinish) {
			// テキスト読む時間を与え消える
			moveTimer_ -= GameTimer::DeltaTime();
			moveTimer_ = std::clamp(moveTimer_, 0.0f, 5.0f);
			if (moveTimer_ <= 0.5f) {
				for (auto& ui : uis_->GetModeUIs()) {
					ui->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,moveTimer_ * 2.0f });
				}
				// 文字が消えたら吹き出しを戻す
				if (moveTimer_ == 0.0f) {
					for (auto& ui : uis_->GetModeUIs()) {
						ui->GetSprite()->SetEnable(false);
					}
					SpeechBubble_SceneOut();
				}
				// 吹き出しが消えたら猫を戻す
				if (spComes_ == 0.0f) {
					SpeechCat_SceneOut();
				}
			}
		}
		if (!isSpeechSkip_ && speechSkipKey) {
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			isSpeechSkip_ = true;
		}

	}
	break;
	case TutorialType::CreateGhost:
	{
		if (isSkip_) { break; }
		// ゴーストを取らせる
		if (catComes_ == 0.0f && isSpeechFinish_) {
			isMoveEnable_ = true;
			if (isCreateGhost_) {
				if (createGhostTimer_ < 0.2f) {
					createGhostTimer_ += GameTimer::DeltaTime();
				} else {
					isSpeechSkip_ = false;
					isMoveEnable_ = false;
					isSpeechFinish_ = false;
					type_ = TutorialType::Explanation;
				}
			}
			return;
		}

		// 猫の出てくる処理
		if (catComes_ < 1.0f && drawGhostBlockTimer_ == 0.0f) {
			SpeechCat_SceneIn();
		} else {
			// 吹き出しを出す
			if (spComes_ < 1.0f && drawGhostBlockTimer_ == 0.0f) {
				SpeechBubble_SceneIn();
				//吹き出しが出たらしゃべる
			} else {
				if (!isSpeechSkip_ && speechSkipKey) {
					AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
					isSpeechSkip_ = true;
					drawGhostBlockTimer_ = 1.0f;
				}
				// ゴーストブロックは描画済みか
				const float textureSize = speechSpeed / uis_->GetGhostBlock()->GetTextureSize().x * 5.0f;
				drawGhostBlockTimer_ += GameTimer::DeltaTime() * textureSize;
				drawGhostBlockTimer_ = std::clamp(drawGhostBlockTimer_, 0.0f, 1.0f);
				uis_->GetGhostBlock()->SetEnable(true);
				uis_->GetGhostBlock()->SetUvMaxSize(Vector2{ drawGhostBlockTimer_ ,1.0f });
				// ブロックの描画が終わっているならテキストを描画する
				if (drawGhostBlockTimer_ == 1.0f) {
					// テキストが読み終わっていたら
					if (IsSpeechCat(uis_->GetCreateUIs(), createGhostTimer_)) {

						createGhostTimer_ -= GameTimer::DeltaTime();
						createGhostTimer_ = std::clamp(createGhostTimer_, 0.0f, 5.0f);
						// テキスト、吹き出し、猫を下げる
						if (createGhostTimer_ <= 0.5f) {
							for (auto& ui : uis_->GetCreateUIs()) {
								ui->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,createGhostTimer_ * 2.0f });
							}
							uis_->GetGhostBlock()->SetColor(Vector4{ 1.0f,1.0f,1.0f,createGhostTimer_ * 2.0f });
							// 文字が消えたら吹き出しを戻す
							if (createGhostTimer_ == 0.0f) {
								for (auto& ui : uis_->GetCreateUIs()) {
									ui->GetSprite()->SetEnable(false);
								}
								SpeechBubble_SceneOut();
							}
							// 吹き出しが消えたら猫を戻す
							if (spComes_ == 0.0f) {
								SpeechCat_SceneOut();
								isSpeechFinish_ = true;
							}
						}
					}
				}
			}
		}
	}
	break;
	case TutorialType::Explanation:
	{
		if (isSkip_) { break; }
		SpeechCatScene(uis_->GetExplanaUIs(), explanaTimer_, TutorialType::CollisionGhost);
		if (!isSpeechSkip_ && speechSkipKey) {
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			isSpeechSkip_ = true;
		}
	}
	break;
	case TutorialType::CollisionGhost:
	{
		if (isSkip_) { break; }
		// ゴーストを取らせる
		if (catComes_ == 0.0f && isSpeechFinish_) {
			isMoveEnable_ = true;
			if (ghostCount_ != 0) {
				if (collisionTimer_ < 0.1f) {
					collisionTimer_ += GameTimer::DeltaTime();
				} else {
					isSpeechSkip_ = false;
					isMoveEnable_ = false;
					isSpeechFinish_ = false;
					type_ = TutorialType::Goal;
				}
			}
			return;
		}

		// テキストが読み終わっていたら
		if (IsSpeechCat(uis_->GetCollisionUIs(), collisionTimer_)) {
			collisionTimer_ -= GameTimer::DeltaTime();
			collisionTimer_ = std::clamp(collisionTimer_, 0.0f, 5.0f);
			// テキスト、吹き出し、猫を下げる
			if (collisionTimer_ <= 0.5f) {
				for (auto& ui : uis_->GetCollisionUIs()) {
					ui->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,collisionTimer_ * 2.0f });
				}
				// 文字が消えたら吹き出しを戻す
				if (collisionTimer_ == 0.0f) {
					for (auto& ui : uis_->GetCollisionUIs()) {
						ui->GetSprite()->SetEnable(false);
					}
					SpeechBubble_SceneOut();
				}
				// 吹き出しが消えたら猫を戻す
				if (spComes_ == 0.0f) {
					SpeechCat_SceneOut();
					isSpeechFinish_ = true;
				}
			}
		}
		if (!isSpeechSkip_ && speechSkipKey) {
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			isSpeechSkip_ = true;
		}
	}
	break;
	case TutorialType::Goal:
	{
		if (isSkip_) { break; }
		// チュートリアルを終了する
		if (catComes_ == 0.0f && isSpeechFinish_) {
			isMoveEnable_ = true;
			isSpeechSkip_ = false;
			type_ = TutorialType::Goal;
			return;
		}

		// 猫の出てくる処理
		if (catComes_ < 1.0f && !isSpeechFinish_) {
			SpeechCat_SceneIn();
			// 猫が出て来たら
		} else {
			// 吹き出しを出す
			if (spComes_ < 1.0f && !isSpeechFinish_) {
				SpeechBubble_SceneIn();
				//吹き出しが出たらしゃべる
			} else {
				// テキストが読み終わっていたら
				if (IsSpeechCat(uis_->GetGoalUIs(), goalTimer_)) {
					// Aボタン、Spaceの表示から押すまで
					if (!isPush_) {
						PushButton();
						// 押していたら次のTypeに遷移
					} else {
						goalTimer_ -= GameTimer::DeltaTime();
						goalTimer_ = std::clamp(goalTimer_, 0.0f, 1.0f);
						if (goalTimer_ <= 0.5f) {
							Vector4 color = Vector4{ 1.0f,1.0f,1.0f,goalTimer_ * 2.0f };
							for (auto& ui : uis_->GetGoalUIs()) {
								ui->GetSprite()->SetColor(color);
							}
							uis_->GetButtonA()->SetColor(color);
							uis_->GetSpaceKey()->SetColor(color);
							// 文字が消えたら吹き出しを戻す
							if (goalTimer_ == 0.0f) {
								for (auto& ui : uis_->GetGoalUIs()) {
									ui->GetSprite()->SetEnable(false);
								}
								isSpeechFinish_ = true;
								SpeechBubble_SceneOut();
							}
							// 吹き出しが消えたら猫を戻す
							if (spComes_ == 0.0f) {
								SpeechCat_SceneOut();
							}
						}
					}
				}
				if (!isSpeechSkip_ && speechSkipKey) {
					AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
					isSpeechSkip_ = true;
				}
			}
		}
	}
	break;
	case TutorialType::Finish:
	{
		isMoveEnable_ = true;
	}
	break;
	default:
		break;
	}

	if (!isMoveEnable_) {
		if ((input->IsTriggerKey(DIK_R) || input->IsTriggerButton(XInputButtons::BUTTON_X)) &&
			!isSkip_) {
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			isSkip_ = true;
		}
		if (isSkip_) {
			skipTimer_ -= GameTimer::DeltaTime();
			skipTimer_ = std::clamp(skipTimer_, 0.0f, 1.0f);
			Vector4 color = { 1.0f,1.0f,1.0f,skipTimer_ };
			SetColorUIs(uis_->GetSelfUIs(), color);
			SetColorUIs(uis_->GetCreateUIs(), color);
			SetColorUIs(uis_->GetExplanaUIs(), color);
			SetColorUIs(uis_->GetCollisionUIs(), color);
			SetColorUIs(uis_->GetGoalUIs(), color);
			uis_->GetButtonA()->SetColor(color);
			uis_->GetSpaceKey()->SetColor(color);
			uis_->GetGhostBlock()->SetColor(color);
			for (auto& ui : uis_->GetModeUIs()) {
				ui->GetSprite()->SetColor(color);
			}
			// 文字が消えたら吹き出しを戻す
			if (skipTimer_ == 0.0f) {
				SpeechBubble_SceneOut();
			}
			// 吹き出しが消えたら猫を戻す
			if (spComes_ == 0.0f) {
				SpeechCat_SceneOut();
			}
			if (catComes_ == 0.0f) {
				type_ = TutorialType::Finish;
			}
		}
	}
	if (isControllerConnected_) {
		uis_->GetReset_R()->SetEnable(false);
		uis_->GetReset_X()->SetEnable(true);
	} else {
		uis_->GetReset_R()->SetEnable(true);
		uis_->GetReset_X()->SetEnable(false);
	}

	uis_->Update();
}

void TutorialDirector::TutorialReset()
{
	uis_->Reset();

	isMoveEnable_ = false;
	type_ = TutorialType::Self;
	isPush_ = false;
	catComes_ = 0.0f;	
	spComes_ = 0.0f;
	selfTimer_ = 0.0f;
	moveTimer_ = 0.0f;
	moveChecker_ = 0;
	drawGhostBlockTimer_ = 0.0f;
	createGhostTimer_ = 0.0f;
	isSpeechFinish_ = false;
	isCreateGhost_ = false;
	explanaTimer_ = 0.0f;
	collisionTimer_ = 0.0f;
	ghostCount_ = 0;
	goalTimer_ = 0.0f;
	skipTimer_ = 1.0f;
	isSkip_ = false;
}

void TutorialDirector::SpeechCatScene(const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis, float& time, TutorialType nextType)
{
	// 猫の出てくる処理
	if (catComes_ < 1.0f) {
		SpeechCat_SceneIn();
	// 猫が出て来たら
	} else {
		// 吹き出しを出す
		if (spComes_ < 1.0f) {
			SpeechBubble_SceneIn();
		//吹き出しが出たらしゃべる
		} else {
			// テキストが読み終わっていたら
			if (IsSpeechCat(uis, time)) {
				// Aボタン、Spaceの表示から押すまで
				if (!isPush_) {
					PushButton();
					// 押していたら次のTypeに遷移
				} else {
					DeleteSpeechUI(uis, time, nextType);
				}
			}
		}
	}
}

void TutorialDirector::SpeechCat_SceneIn()
{
	catComes_ += GameTimer::DeltaTime();
	catComes_ = std::clamp(catComes_, 0.0f, 1.0f);
	float eased = Ease::Out::Back(catComes_);
	float posY = 1000 + (360 - 1000) * eased;
	uis_->GetCat()->SetTranslate(Vector2{ 640.0f,posY });
	uis_->GetReset_R()->SetColor(Vector4{ 1.0f,1.0f,1.0f,catComes_ });
	uis_->GetReset_X()->SetColor(Vector4{ 1.0f,1.0f,1.0f,catComes_ });
}

void TutorialDirector::SpeechCat_SceneOut()
{
	catComes_ -= GameTimer::DeltaTime();
	catComes_ = std::clamp(catComes_, 0.0f, 1.0f);
	float eased = Ease::Out::Back(catComes_);
	float posY = 1000 + (360 - 1000) * eased;
	uis_->GetCat()->SetTranslate(Vector2{ 640.0f,posY });
	uis_->GetReset_R()->SetColor(Vector4{ 1.0f,1.0f,1.0f,catComes_ });
	uis_->GetReset_X()->SetColor(Vector4{ 1.0f,1.0f,1.0f,catComes_ });
	if (catComes_ == 0.0f) { moveTimer_ = 1.0f; }
}

void TutorialDirector::SpeechBubble_SceneIn()
{
	spComes_ += GameTimer::DeltaTime() * 5.0f;
	spComes_ = std::clamp(spComes_, 0.0f, 1.0f);
	uis_->GetSpBubble()->SetColor(Vector4{ 1.0f,1.0f,1.0f,spComes_ });
}

void TutorialDirector::SpeechBubble_SceneOut()
{
	spComes_ -= GameTimer::DeltaTime() * 5.0f;
	spComes_ = std::clamp(spComes_, 0.0f, 1.0f);
	uis_->GetSpBubble()->SetColor(Vector4{ 1.0f,1.0f,1.0f,spComes_ });
}

bool TutorialDirector::IsSpeechCat(const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis, float& time)
{
	const float speechSpeed = 500.0f;

	bool isFinish = true;
	for (uint32_t i = 0; i < static_cast<uint32_t>(uis.size()); ++i) {
		// 読み終わっていなければ
		const float textureSize = speechSpeed / uis[i]->GetSprite()->GetTextureSize().x;
		if (!uis[i]->GetIsFinished()) {
			time += GameTimer::DeltaTime() * textureSize;
			float uv_x = time - static_cast<float>(i);
			uis[i]->SetUvMaxSize(uv_x);
			uis[i]->GetSprite()->SetEnable(true);
			if (time > static_cast<float>(i + 1)) {
				if (i == static_cast<uint32_t>(uis.size() - 1)) {
					time = static_cast<float>(i + 2);
				} else {
					time = static_cast<float>(i + 1);
				}
				uis[i]->SetIsFinished(true);
			}
			isFinish = false;
			break;
		}
	}
	if (isSpeechSkip_ && !isFinish) {
		time = static_cast<float>(uis.size() + 1);
		for (auto& ui : uis) {
			ui->SetUvMaxSize(1.0f);
			ui->GetSprite()->SetEnable(true);
			ui->SetIsFinished(true);
		}
	}
	return isFinish;
}

void TutorialDirector::PushButton()
{
	Input* input = Input::GetInstance();

	if (isControllerConnected_) {
		uis_->GetButtonA()->SetEnable(true);
		uis_->GetSpaceKey()->SetEnable(false);
		if (input->IsTriggerButton(XInputButtons::BUTTON_A)) {
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			isPush_ = true;
		}
	} else {
		uis_->GetButtonA()->SetEnable(false);
		uis_->GetSpaceKey()->SetEnable(true);
		if (input->IsTriggerKey(DIK_SPACE)) {
			AudioPlayer::SinglShotPlay("button.mp3", 0.5f);
			isPush_ = true;
		}
	}
}

void TutorialDirector::DeleteSpeechUI(const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis, float& time, TutorialType nextType)
{
	if (time > 0) {
		// 文字を消す
		time -= GameTimer::DeltaTime();
		time = std::clamp(time, 0.0f, 1.0f);
		Vector4 color = Vector4{ 1.0f,1.0f,1.0f,time };
		for (auto& ui : uis) {
			ui->GetSprite()->SetColor(color);
		}
		uis_->GetButtonA()->SetColor(color);
		uis_->GetSpaceKey()->SetColor(color);
		// 消えたら遷移する
	} else {
		for (auto& ui : uis) {
			ui->GetSprite()->SetEnable(false);
		}
		uis_->GetButtonA()->SetEnable(false);
		uis_->GetSpaceKey()->SetEnable(false);
		uis_->GetButtonA()->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
		uis_->GetSpaceKey()->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
		isPush_ = false;
		type_ = nextType;
		isSpeechSkip_ = false;
	}
}

void TutorialDirector::SetColorUIs(const std::array<std::unique_ptr<ITutorialTextUI>, 2>& uis, const Vector4& color)
{
	for (auto& ui : uis) {
		ui->GetSprite()->SetColor(color);
	}
}
