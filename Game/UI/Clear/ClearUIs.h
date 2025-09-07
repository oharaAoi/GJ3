#pragma once
#include <memory>
#include <array>

#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/Json/IJsonConverter.h"

#include "Game/UI/Button/IButtonUI.h"

class ClearUIs :
	public AttributeGui
{
public:

	struct Parameter : public IJsonConverter {
		Vector2 selectButtonPos;
		Vector2 nextStageButtonPos;

		Parameter() { SetName("ClearUIs"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("selectButtonPos", selectButtonPos)
				.Add("nextStageButtonPos", nextStageButtonPos)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "selectButtonPos", selectButtonPos);
			fromJson(jsonData, "nextStageButtonPos", nextStageButtonPos);
		}
	};

public:

	ClearUIs() = default;
	~ClearUIs() = default;

	void Init();

	void Update();

	void Debug_Gui() override;


	ButtonType GetTypeIndex(int index)const { return buttonUIs_[index]->GetButtonType(); }
	void BlinkingIndex(int index) { buttonUIs_[index]->Blinking(); }
	void ResetIndex(int index) { buttonUIs_[index]->Reset(); }

private:

	// ボタン
	std::array<std::unique_ptr<IButtonUI>, 2> buttonUIs_;


	Parameter param_;

};

