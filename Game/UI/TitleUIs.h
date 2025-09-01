#pragma once
#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

/// <summary>
/// TitleのUIをまとめたクラス
/// </summary>
class TitleUIs :
	public AttributeGui {
public:

	TitleUIs() = default;
	~TitleUIs() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

private:

	Sprite* title_;

};

