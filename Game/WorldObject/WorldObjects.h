#pragma once
#include <memory>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Game/WorldObject/BackGround.h"

class WorldObjects :
	public AttributeGui {
public:

	WorldObjects() = default;
	~WorldObjects() = default;

	void Init();

	void Update();

	void Debug_Gui() override;

	void SetTexture(const std::string name) { backGround_->SetTexture(name); }

private:

	std::unique_ptr<BackGround> backGround_;

};

