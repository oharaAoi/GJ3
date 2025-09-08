#pragma once
#include <vector>
#include <memory>
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
// game
#include "Game/UI/StageSelectSegment.h"

/// <summary>
/// Stageの内容をまとめたクラス
/// </summary>
class StageSelectCollection {
public:

	StageSelectCollection() = default;
	~StageSelectCollection() = default;

	void Init(Canvas2d* _canvas2d);

	void Update();

private:

	const uint32_t kMax = 1;
	std::vector<std::unique_ptr<StageSelectSegment>> segments_;

};

