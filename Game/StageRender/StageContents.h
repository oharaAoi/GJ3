#pragma once
#include <memory>
#include <vector>
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Game/Manager/StageRegistry.h"
#include "Game/Camera/StageContentCamera.h"
#include "Game/StageRender/StageRenderTarget.h"
#include "Game/UI/StageSelect/StageSelectCollection.h"
#include "Game/Actor/Player/Player.h"

class StageContents {
public:

	StageContents() = default;
	~StageContents();

	void Init(uint32_t maxStageNum);

	void Update();

	void ResetCurrentDrawIndex(int32_t currentStageIndex);

public:

	void SetStageSelectCollection(StageSelectCollection* _collection) { pStageCollection_ = _collection; }

private:

	StageSelectCollection* pStageCollection_;

	std::vector<std::unique_ptr<Canvas2d>> canvas2ds_;
	std::vector<std::unique_ptr<StageRegistry>> stageRegistries_;

	std::unique_ptr<StageContentCamera> camera_;

	uint32_t maxStageNum_;

	std::vector<int> drawIndex_;
	int centerIndex_;
};

