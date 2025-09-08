#pragma once
#include <memory>
#include "Game/Scene/BaseScene.h"
// engine
#include "Engine/System/Audio/AudioPlayer.h"
#include "Engine/Render/SceneRenderer.h"
#include "Engine/Module/Components/Effect/BaseParticles.h"
// camera
#include "Game/Camera/Camera3d.h"
#include "Game/Camera/Camera2d.h"
#include "Game/Camera/DebugCamera.h"
// actor
#include "Game/WorldObject/Skybox.h"
#include "Game/WorldObject/WorldObjects.h"
// ui
#include "Game/UI/StageSelector.h"
#include "Game/UI/StageSelectCollection.h"
#include "Game/StageRender/StageContents.h"
#include "Game/Tool/StageLoader.h"
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Game/Effect/SwirlTransition.h"
// effect
#include "Game/Effect/TitleFlashEffect.h"

class IStageSelectSceneBehavior;

class StageSelectScene
	: public BaseScene{
	friend class SelectingStageBehavior;
public:
	StageSelectScene();
	~StageSelectScene();

	void Init() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;

private:
	void ChangeBehavior(IStageSelectSceneBehavior* newBehavior);

private:
	// ------------------- camera ------------------- //
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<Camera3d> camera3d_;
	std::unique_ptr<Camera2d> camera2d_;

	// ------------------- actor ------------------- //

	std::unique_ptr<WorldObjects> worldObjects_;

	// ------------------- behavior ------------------- //
	std::unique_ptr<IStageSelectSceneBehavior> behavior_;

	// ------------------- ui ------------------- //
	std::unique_ptr<StageLoader> stageLoader_;


	std::unique_ptr<StageContents> stageContents_;

	// ------------------- effect ------------------- //
	BaseParticles* particle_;

	// ------------------- audio ------------------- //
	std::unique_ptr<AudioPlayer> bgm_;

	SceneRenderer* sceneRenderer_;

	std::shared_ptr<IPostEffect> gotRay_;
};

#pragma region Behavior

class IStageSelectSceneBehavior{
public:
	IStageSelectSceneBehavior(StageSelectScene* _host):host_(_host){}
	virtual ~IStageSelectSceneBehavior() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
protected:
	StageSelectScene* host_;
};

/// <summary>
/// �I�𒆂̋����V�[��
/// </summary>
class SelectingStageBehavior
	: public IStageSelectSceneBehavior{
public:
	SelectingStageBehavior(StageSelectScene* _host);
	~SelectingStageBehavior() override;
	void Init() override;
	void Update() override;
private:
	std::unique_ptr<StageSelectCollection> stageCollection;
	std::unique_ptr<LightFlash> lightFlash_;

};

class TransitionToGameBehavior
	: public IStageSelectSceneBehavior{
public:
	TransitionToGameBehavior(StageSelectScene* _host);
	~TransitionToGameBehavior() override;

	void Init() override;
	void Update() override;
private:
	float transitionTime_ = 1.0f;
	float currentTime_ = 0.0f;

	std::unique_ptr<SwirlTransition> swirlTransition_;

};


#pragma endregion
