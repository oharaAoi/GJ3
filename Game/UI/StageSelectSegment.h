#pragma once
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

/// <summary>
/// StageSelectで描画する背景やステージの1区間
/// </summary>
class StageSelectSegment :
	public AttributeGui {
public:

	StageSelectSegment() = default;
	~StageSelectSegment() = default;

	void Init(Canvas2d* _canvas2d, uint32_t index);

	void Update();

	void Debug_Gui() override;

private:

	Sprite* backGround_;
	Sprite* pictureFrame_;
	Sprite* stageContent_;

	Vector2 centerPos_ = {640.f, 360.f};

};

