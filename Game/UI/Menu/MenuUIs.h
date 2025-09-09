#pragma once
#include <memory>
#include <array>

#include "Engine.h"
#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/Json/IJsonConverter.h"
#include "Engine/Module/Components/2d/BaseEntity2d.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

#include "Game/UI/Button/IButtonUI.h"
#include "Game/UI/Menu/OperationUI.h"

class MenuUIs : 
	public AttributeGui
{
public:

	struct Parameter : public IJsonConverter {
		Vector2 centerPos;
		float interval;
		Vector2 backButtonPos;
		Vector4 bgColor;

		Parameter() { SetName("MenuUIs"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("centerPos", centerPos)
				.Add("interval", interval)
				.Add("backButtonPos", backButtonPos)
				.Add("bgColor", bgColor)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "centerPos", centerPos);
			fromJson(jsonData, "interval", interval);
			fromJson(jsonData, "backButtonPos", backButtonPos);
			fromJson(jsonData, "bgColor", bgColor);
		}
	};

public:

	MenuUIs() = default;
	~MenuUIs() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

	void FadeIn() { isFade_ = true; endFade_ = false; fadeFrame_ = 0.0f; }
	void FadeOut() { isFade_ = false; endFade_ = false; fadeFrame_ = 1.0f; }

	bool GetEndFade()const { return endFade_; }
	bool GetIsFade()const { return isFade_; }

	Sprite* GetMenuBG()const { return menu_; }
	bool GetOpEndFade()const { return operationUI_->FadeEnd(); }

	ButtonType GetTypeIndex(int index)const { return buttonUIs_[index]->GetButtonType(); }
	void BlinkingIndex(int index) { buttonUIs_[index]->Blinking(); }
	void ResetIndex(int index) { buttonUIs_[index]->Reset(); }
	void OperationUpdate(bool openOperation) { operationUI_->Update(openOperation); }
	void SetColor(int index) { buttonUIs_[index]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.4f }); }

	void SetColors(int index) {
		for (size_t i = 0; i < buttonUIs_.size(); ++i) {
			if (index == static_cast<int>(i)) { continue; }
			buttonUIs_[i]->GetSprite()->SetColor(Vector4{1.0f,1.0f,1.0f,0.4f});
		}
	}

	// ステージリセット時
	void ResetUIs();

private:

	// メニュー背景
	Sprite* menu_ = nullptr;

	// 操作方法
	std::unique_ptr<OperationUI> operationUI_ = nullptr;

	// ボタン
	std::array<std::unique_ptr<IButtonUI>, 4> buttonUIs_;

	Parameter param_;

	// trueがin、falseがout
	bool isFade_ = false;
	bool endFade_ = false;
	float fadeTime_ = 1.0f;
	float fadeFrame_ = 0.0f;

};

