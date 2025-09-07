#pragma once
#include <memory>
#include <vector>
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Game/Manager/StageRegistry.h"
#include "Game/Camera/StageContentCamera.h"
#include "Game/StageRender/StageRenderTarget.h"
#include "Game/Actor/Player/Player.h"

class StageContents {
public:

	StageContents() = default;
	~StageContents();

	void Init(uint32_t maxStageNum);

	void Update();

public:

	StageRenderTarget* GetStageRenderTarget() { return stageRenderTarget_.get(); }

private:

	std::vector<std::unique_ptr<Canvas2d>> canvas2ds_;
	std::vector<std::unique_ptr<StageRegistry>> stageRegistries_;

	std::unique_ptr<StageContentCamera> camera_;
	std::unique_ptr<StageRenderTarget> stageRenderTarget_;

	uint32_t maxStageNum_;
};

