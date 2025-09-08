#pragma once
#include <vector>
#include <memory>
#include <list>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
// game
#include "Game/UI/StageSelectSegment.h"

/// <summary>
/// Stageの内容をまとめたクラス
/// </summary>
class StageSelectCollection :
public AttributeGui {
public:

	StageSelectCollection() = default;
	~StageSelectCollection() = default;

	void Init(Canvas2d* _canvas2d);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_leftTime">: 残り時間</param>
	/// <param name="scrollDirection">: 方向</param>
	void Update(float _scrolT, int32_t _scrollDirection);

	void Debug_Gui() override;

	RenderTargetType GetRenderTarget(uint32_t index);

private:

	const int32_t kMax = 3;
	int32_t stageNumMax_;

	std::vector<std::unique_ptr<StageSelectSegment>> segments_;
	std::vector<StageSelectSegment*> ptrs;
	std::vector<Vector2> centerPositions_;

	float scrollDuration_ = 0.6f;

	float scrollPos_;
	float slotSpacing_;


};

