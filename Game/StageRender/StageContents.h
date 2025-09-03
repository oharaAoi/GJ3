#pragma once
#include <memory>
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Game/Manager/StageRegistry.h"
#include "Game/Camera/StageContentCamera.h"
#include "Game/StageRender/StageRenderTarget.h"
#include "Game/Actor/Player/Player.h"

class StageContents {
public:

	StageContents() = default;
	~StageContents();

	void Init();

	void Update();

public:

	StageRenderTarget* GetStageRenderTarget() { return stageRenderTarget_.get(); }

private:

	std::unique_ptr<Canvas2d> canvas2ds_[3];
	std::unique_ptr<Player> player_[3];
	std::unique_ptr<StageRegistry> stageRegistries_[3];

	std::unique_ptr<StageContentCamera> camera_;
	std::unique_ptr<StageRenderTarget> stageRenderTarget_;
};

