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

private:

	std::unique_ptr<BackGround> backGround_;

};

