#include "ClearScene.h"
#include "Engine.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Game/Scene/GameScene.h"

ClearScene::ClearScene(){}

ClearScene::~ClearScene() { Finalize(); }

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void ClearScene::Init()
{
	JsonItems* adjust = JsonItems::GetInstance();
	adjust->Init("StageSelectScene");

	// -------------------------------------------------
	// ↓ Sceneの初期化
	// -------------------------------------------------

	sceneRenderer_ = SceneRenderer::GetInstance();
	sceneRenderer_->Init();
	EditorWindows::GetInstance()->SetSceneRenderer(sceneRenderer_);

	// -------------------------------------------------
	// ↓ cameraの初期化
	// -------------------------------------------------

	camera3d_ = std::make_unique<Camera3d>();
	debugCamera_ = std::make_unique<DebugCamera>();
	camera2d_ = std::make_unique<Camera2d>();
	camera3d_->Init();
	debugCamera_->Init();
	camera2d_->Init();

	// -------------------------------------------------
	// ↓ actorの初期化
	// -------------------------------------------------

	// GameSceneからの値を取得
	if (const auto& opt = GameScene::LastResult()) {
		ghostCount_ = opt->ghostCount;
	}

	ghostSoulManager_ = std::make_unique<GhostSoulManager>();
	ghostSoulManager_->InitClearScene(Engine::GetCanvas2d());
	for (int i = 0; i < ghostCount_; ++i) {
		ghostSoulManager_->CreateSoul(Vector2{ 64.0f,64.0f });
	}
	ghostSoulManager_->SetPosition(Vector2{ 640.0f,360.0f });

	worldObjects_ = std::make_unique<WorldObjects>();
	worldObjects_->Init();
	worldObjects_->SetTexture("clear.png");

}

void ClearScene::Finalize()
{
	sceneRenderer_->Finalize();
	ParticleManager::GetInstance()->Finalize();
	GpuParticleManager::GetInstance()->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void ClearScene::Update()
{
	// -------------------------------------------------
	// ↓ actorの更新
	// -------------------------------------------------

	worldObjects_->Update();

	ghostSoulManager_->UpdateClearScene();;

	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------
	if (debugCamera_->GetIsActive()) {
		debugCamera_->Update();
	} else {
		camera3d_->Update();
	}
	camera2d_->Update();

	// -------------------------------------------------
	// ↓ sceneの更新
	// -------------------------------------------------
	sceneRenderer_->Update();

	// -------------------------------------------------
	// ↓ あたり判定
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ 最後に行いたい更新
	// -------------------------------------------------

	sceneRenderer_->PostUpdate();
}

void ClearScene::Draw() const
{
	// Sceneの描画
	sceneRenderer_->Draw();
}
