#pragma once
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Game/Camera/Camera2d.h"

class StageSelectCamera :
	public AttributeGui {
public:

	StageSelectCamera() = default;
	~StageSelectCamera() = default;

	void Init(Camera2d* _camera2d);

	void Update();

	void Debug_Gui() override;

private:

	Camera2d* pCamera2d_;

};

