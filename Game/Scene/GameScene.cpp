#include "GameScene.h"
#include "Engine.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Game/Commands/ObjectCommandInvoker.h"

GameScene::GameScene() {}
GameScene::~GameScene() { Finalize(); }

void GameScene::Finalize()
{
	sceneRenderer_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Init()
{
	Engine::GetCanvas2d()->Init();

	JsonItems *adjust = JsonItems::GetInstance();
	adjust->Init("GameScene");

	// -------------------------------------------------
	// ↓ Sceneの初期化
	// -------------------------------------------------

	sceneRenderer_ = SceneRenderer::GetInstance();
	sceneRenderer_->Init();
	EditorWindows::GetInstance()->SetSceneRenderer(sceneRenderer_);

	skybox_ = SceneRenderer::GetInstance()->AddObject<Skybox>("Skybox", "Object_Skybox.json", -999);
	Render::SetSkyboxTexture(skybox_->GetTexture());

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

	player_ = std::make_unique<Player>();
	player_->Init(Engine::GetCanvas2d());

	stageRegistry_ = std::make_unique<StageRegistry>();
	stageRegistry_->Init(Engine::GetCanvas2d());
	stageRegistry_->SetPlayer(player_.get());
	stageRegistry_->Register("stage_0.json");

	mapCollision_ = std::make_unique<MapCollisionSystem>();
	mapCollision_->Init(stageRegistry_.get());

	worldObjects_ = std::make_unique<WorldObjects>();
	worldObjects_->Init();

	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------
	ObjectCommandInvoker::GetInstance().Initialize();

	// -------------------------------------------------
	// ↓ spriteの初期化
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ その他設定
	// -------------------------------------------------
	player_->SetTileSize(stageRegistry_->GetTileSize());
	player_->GetSprite()->ReSetTextureSize(stageRegistry_->GetTileSize());
	player_->SetIndex(stageRegistry_->GetStartIndex());
	player_->SetPosition(stageRegistry_->GetStartPos());
	player_->SetOffset(stageRegistry_->GetMapOffset());
	player_->SetMapCollision(mapCollision_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update()
{

	// -------------------------------------------------
	// ↓ actorの更新
	// -------------------------------------------------
	player_->Update();
	ObjectCommandInvoker::GetInstance().Update();
	worldObjects_->Update();

	stageRegistry_->Update();
	
	mapCollision_->Update();

	// -------------------------------------------------
	// ↓ spriteの更新
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------
	if (debugCamera_->GetIsActive())
	{
		debugCamera_->Update();
	}
	else
	{
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Draw() const
{
	// Sceneの描画
	sceneRenderer_->Draw();
}
