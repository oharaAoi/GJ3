#pragma once
#include "Engine/Core/GraphicsContext.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/2d/Sprite.h"
#include "Engine/Module/Components/2d/Canvas2d.h"

/// <summary>
/// StageSelectで描画する背景やステージの1単体
/// </summary>
class StageSelectSegment :
	public AttributeGui {
public:

	StageSelectSegment() = default;
	~StageSelectSegment() = default;

	void Init(Canvas2d* _canvas2d,int32_t index);

	void Update();

	void Debug_Gui() override;

public:

	void SetCenterPosX(float _posX);
	const Vector2& GetCenterPos() const { return centerPos_; }

	int32_t GetStageIndex() const{ return stageIndex_; }
	void SetStageIndex(int32_t index){ stageIndex_ = index; }

	RenderTargetType GetRenderTargetType() const { return rtType_; }

private:

	GraphicsContext* ctx_;

	Sprite* backGround_;
	Sprite* pictureFrame_;
	Sprite* stageContent_;

	Vector2 centerPos_ = {640.f, 360.f};

	int32_t stageIndex_;

	RenderTargetType rtType_;
public:
	int32_t getStageIndex() const{ return stageIndex_; }
};

