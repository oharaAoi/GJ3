#include "TutorialDirector.h"

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

	const float speechSpeed = 500.0f;

	switch (type_)
	{
	case TutorialType::Self:
	{
		// 猫の出てくる処理
		if (catComes_ < 1.0f) {
			SpeechCat_SceneIn();

			// 猫が出て来たら自己紹介
		} else {
			// 吹き出しを出す
			if (spComes_ < 1.0f) {
				SpeechBubble_SceneIn();
			//吹き出しが出たらしゃべる
			} else {
				// テキストが読み終わっていたら
				if (IsSpeechCat(uis_->GetSelfUIs(),selfTimer_)) {
					// Aボタン、Spaceの表示から押すまで
					if (!isPush_) {
						if (isControllerConnected_) {
							uis_->GetButtonA()->SetEnable(true);
							uis_->GetSpaceKey()->SetEnable(false);
							if (input->IsTriggerButton(XInputButtons::BUTTON_A)) {
								isPush_ = true;
							}
						} else {
							uis_->GetButtonA()->SetEnable(false);
							uis_->GetSpaceKey()->SetEnable(true);
							if (input->IsTriggerKey(DIK_SPACE)) {
								isPush_ = true;
							}
						}
						// 押していたら次のTypeに遷移
					} else {
						if (selfTimer_ > 0) {
							// 文字を消す
							selfTimer_ -= GameTimer::DeltaTime();
							selfTimer_ = std::clamp(selfTimer_, 0.0f, 1.0f);
							Vector4 color = Vector4{ 1.0f,1.0f,1.0f,selfTimer_ };
							for (auto& ui : uis_->GetSelfUIs()) {
								ui->GetSprite()->SetColor(color);
							}
							uis_->GetButtonA()->SetColor(color);
							uis_->GetSpaceKey()->SetColor(color);
							// 消えたら遷移する
						} else {
							uis_->GetButtonA()->SetEnable(false);
							uis_->GetSpaceKey()->SetEnable(false);
							uis_->GetButtonA()->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
							uis_->GetSpaceKey()->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
							type_ = TutorialType::Move;
						}
					}
				}
			}
		}
	}

		break;
	case TutorialType::Move:
	{
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
		// テキストが読み終わっていたら
		if (isFinish) {
			// テキスト読む時間を与え消える
			moveTimer_ -= GameTimer::DeltaTime();
			moveTimer_ = std::clamp(moveTimer_, 0.0f, 3.0f);
			if (moveTimer_ <= 0.5f) {
				for (auto& ui : uis_->GetModeUIs()) {
					ui->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,moveTimer_ * 2.0f });
				}
				// 文字が消えたら吹き出しを戻す
				if (moveTimer_ == 0.0f) {
					spComes_ -= GameTimer::DeltaTime() * 5.0f;
					spComes_ = std::clamp(spComes_, 0.0f, 1.0f);
					uis_->GetSpBubble()->SetColor(Vector4{ 1.0f,1.0f,1.0f,spComes_ });
				}
				// 吹き出しが消えたら猫を戻す
				if (spComes_ == 0.0f) {
					catComes_ -= GameTimer::DeltaTime();
					catComes_ = std::clamp(catComes_, 0.0f, 1.0f);
					float eased = Ease::Out::Back(catComes_);
					float posY = 1000 + (360 - 1000) * eased;
					uis_->GetCat()->SetTranslate(Vector2{ 640.0f,posY });
					if (catComes_ == 0.0f) { moveTimer_ = 1.0f; }
				}
			}
		}

	}
		break;
	case TutorialType::CreateGhost:
	{
		// 猫の出てくる処理
		if (catComes_ < 1.0f) {
			SpeechCat_SceneIn();
		} else {
			// 吹き出しを出す
			if (spComes_ < 1.0f) {
				SpeechBubble_SceneIn();
			//吹き出しが出たらしゃべる
			} else {
				// テキストが読み終わっていたら
				if (IsSpeechCat(uis_->GetCreateUIs(),createGhostTimer_)) {
					// Aボタン、Spaceの表示から押すまで
					if (!isPush_) {
						if (isControllerConnected_) {
							uis_->GetButtonA()->SetEnable(true);
							uis_->GetSpaceKey()->SetEnable(false);
							if (input->IsTriggerButton(XInputButtons::BUTTON_A)) {
								isPush_ = true;
							}
						} else {
							uis_->GetButtonA()->SetEnable(false);
							uis_->GetSpaceKey()->SetEnable(true);
							if (input->IsTriggerKey(DIK_SPACE)) {
								isPush_ = true;
							}
						}
						// 押していたら次のTypeに遷移
					} else {
						if (createGhostTimer_ > 0) {
							// 文字を消す
							selfTimer_ -= GameTimer::DeltaTime();
							selfTimer_ = std::clamp(selfTimer_, 0.0f, 1.0f);
							Vector4 color = Vector4{ 1.0f,1.0f,1.0f,selfTimer_ };
							for (auto& ui : uis_->GetSelfUIs()) {
								ui->GetSprite()->SetColor(color);
							}
							uis_->GetButtonA()->SetColor(color);
							uis_->GetSpaceKey()->SetColor(color);
							// 消えたら遷移する
						} else {
							uis_->GetButtonA()->SetEnable(false);
							uis_->GetSpaceKey()->SetEnable(false);
							uis_->GetButtonA()->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
							uis_->GetSpaceKey()->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
							type_ = TutorialType::Move;
						}
					}
				}
			}
		}
	}
		break;
	case TutorialType::Explanation:
		break;
	case TutorialType::CollisionGhost:
		break;
	case TutorialType::Goal:
		break;
	default:
		break;
	}

	uis_->Update();
}

void TutorialDirector::SpeechCat_SceneIn()
{
	catComes_ += GameTimer::DeltaTime();
	catComes_ = std::clamp(catComes_, 0.0f, 1.0f);
	float eased = Ease::Out::Back(catComes_);
	float posY = 1000 + (360 - 1000) * eased;
	uis_->GetCat()->SetTranslate(Vector2{ 640.0f,posY });
}

void TutorialDirector::SpeechBubble_SceneIn()
{
	spComes_ += GameTimer::DeltaTime() * 5.0f;
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
			if (time > static_cast<float>(i + 1)) {
				time = static_cast<float>(i + 1);
				uis[i]->SetIsFinished(true);
			}
			isFinish = false;
			break;
		}
	}
	return isFinish;
}
