#include "GhostTakenSwirl.h"
#include "Render.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"
#include "Engine/Module/Geometry/Structs/Vertices.h"

GhostTakenSwirl::~GhostTakenSwirl() {
	vertexData_ = nullptr;
	indexData_ = nullptr;
	materialData_ = nullptr;
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
	materialBuffer_.Reset();
	transform_.reset();
	dissolveResource_->Finalize();
	swirlResource_->Finalize();
}

void GhostTakenSwirl::Init(const Vector2& tileSize) {
	SetName("GhostTakenSwirl");
	GraphicsContext* ctx = GraphicsContext::GetInstance();
	ID3D12Device* pDevice = ctx->GetDevice();

	Vector2 anchorPoint = { 0.5f, 0.5f };
	textureSize_ = tileSize;
	drawRange_ = textureSize_;
	leftTop_ = { 0.0f, 0.0f };

	// ----------------------------------------------------------------------------------
	vertexBuffer_ = CreateBufferResource(pDevice, sizeof(TextureMesh) * 4);
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(TextureMesh) * 4;
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(TextureMesh);
	// Resourceにデータを書き込む 
	vertexData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	Vector3 pivotOffset = {
		textureSize_.x * anchorPoint.x,
		textureSize_.y * anchorPoint.y,
		0.0f
	};

	// スプライトのサイズを基に頂点を設定
	RectangleVertices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
		{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;		// 左下
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	vertexData_[1].pos = rect.leftTop;			// 左上
	vertexData_[1].texcoord = { 0.0f, 0.0f };
	vertexData_[2].pos = rect.rightBottom;		// 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;			// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };

	// ----------------------------------------------------------------------------------
	// indexBufferを作成
	indexBuffer_ = CreateBufferResource(pDevice, sizeof(uint32_t) * 6);
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 6);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;

	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;
	// ---------------------------------------------------------------------------------
	// Materialを作成
	materialBuffer_ = CreateBufferResource(pDevice, sizeof(TextureMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->uvTransform = Matrix4x4::MakeUnit();

	// ----------------------------------------------------------------------------------
	// Tarnsformを作成
	transform_ = std::make_unique<ScreenTransform>();
	transform_->Init(pDevice);

	uvTransform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };
	indexBuffer_ = CreateBufferResource(pDevice, sizeof(uint32_t) * 6);
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 6);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;

	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;
	// ----------------------------------------------------------------------------------
	materialBuffer_ = CreateBufferResource(pDevice, sizeof(TextureMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->uvTransform = Matrix4x4::MakeUnit();

	// ----------------------------------------------------------------------------------
	transform_ = std::make_unique<ScreenTransform>();
	transform_->Init(pDevice);

	uvTransform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };

	pivotOffset_ = {
		(anchorPoint.x - 0.5f) * textureSize_.x,  // ピボットオフセット（中心からのオフセット）
		(anchorPoint.y - 0.5f) * textureSize_.y   // ピボットオフセット（中心からのオフセット）
	};

	// ----------------------------------------------------------------------------------
	// dissolveのデータを作成する
	dissolveResource_ = std::make_unique<DxResource>();
	dissolveResource_->Init(ctx->GetDevice(), ctx->GetDxHeap(), ResourceType::COMMON);
	dissolveResource_->CreateResource(sizeof(DissolveParam));
	dissolveResource_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&dissolveParam_));

	dissolveParam_->uvTransform = Matrix4x4::MakeUnit();
	dissolveParam_->color = { 1,1,1,1 };
	dissolveParam_->edgeColor = { 1,1,1,1 };
	dissolveParam_->threshold = 0.0f;

	dissolveUvTransform_.scale = { 1,1,1 };
	dissolveUvTransform_.rotate = { 0,0,0 };
	dissolveUvTransform_.translate = { 0,0,0 };

	// swirlのデータを作成する
	swirlResource_ = std::make_unique<DxResource>();
	swirlResource_->Init(ctx->GetDevice(), ctx->GetDxHeap(), ResourceType::COMMON);
	swirlResource_->CreateResource(sizeof(SwirlParam));
	swirlResource_->GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&swirlParam_)); 

	swirlParam_->center = { 0.5f, 0.5f };
	swirlParam_->swirlStrength = 2.0f;
	swirlParam_->time = 0.0f;
	timeRate_ = 1.f;

	textureName_ = "swrilflash.png";
	dissolveTextureName_ = "FireNoize.png";
	maskTextureName_ = "white.png";
	isDestroy_ = false;
}

void GhostTakenSwirl::Update() {
	swirlParam_->time += GameTimer::DeltaTime() * timeRate_;

	materialData_->uvTransform = uvTransform_.MakeAffine();
	dissolveParam_->uvTransform = dissolveUvTransform_.MakeAffine();

	// -------------------------------------------------
	// ↓ UVの変更
	// -------------------------------------------------
	materialData_->uvTransform.m[0][0] = drawRange_.x / textureSize_.x;	// Xスケーリング
	materialData_->uvTransform.m[1][1] = drawRange_.y / textureSize_.y;	// Yスケーリング
	materialData_->uvTransform.m[3][0] = leftTop_.x / textureSize_.x;	// Xオフセット
	materialData_->uvTransform.m[3][1] = leftTop_.y / textureSize_.y;	// Yオフセット
}

void GhostTakenSwirl::Draw() const {
	GraphicsContext* ctx = GraphicsContext::GetInstance();
	ID3D12GraphicsCommandList* commandList = ctx->GetCommandList();

	Matrix4x4 projection = Render::GetViewport2D() * Render::GetProjection2D();
	// テクスチャ位置を保持するための補正行列
	Matrix4x4 correctionTranslation = Vector3({ pivotOffset_.x, pivotOffset_.y, 0.0f }).MakeTranslateMat();
	transform_->Update(correctionTranslation, projection);

	Pipeline* pso = Engine::SetPipeline(PSOType::Sprite, "Sprite_GhostTakenEffect.json");
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	// material
	UINT index = pso->GetRootSignatureIndex("gMaterial");
	commandList->SetGraphicsRootConstantBufferView(index, materialBuffer_->GetGPUVirtualAddress());
	// dissolve
	index = pso->GetRootSignatureIndex("gDissolve");
	commandList->SetGraphicsRootConstantBufferView(index, dissolveResource_->GetResource()->GetGPUVirtualAddress());
	// swirl
	index = pso->GetRootSignatureIndex("gSwirl");
	commandList->SetGraphicsRootConstantBufferView(index, swirlResource_->GetResource()->GetGPUVirtualAddress());

	index = pso->GetRootSignatureIndex("gTransformationMatrix");
	transform_->BindCommand(commandList, index);

	index = pso->GetRootSignatureIndex("gBaseTexture");
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName_, index);
	index = pso->GetRootSignatureIndex("gDissolveTexture");
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, dissolveTextureName_, index);
	index = pso->GetRootSignatureIndex("gMaskTexture");
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, maskTextureName_, index);

	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void GhostTakenSwirl::Debug_Gui() {
	// baseSpriteの設定
	if (ImGui::CollapsingHeader("Base Param")) {
		transform_->Debug_Gui();
		if (ImGui::TreeNode("uvTransform")) {
			ImGui::DragFloat2("uvTranslation", &uvTransform_.translate.x, 0.01f);
			ImGui::DragFloat2("uvScale", &uvTransform_.scale.x, 0.01f);
			ImGui::SliderAngle("uvRotation", &uvTransform_.rotate.z);
			ImGui::TreePop();
		}
		ImGui::ColorEdit4("baseColor", &materialData_->color.x);
	}

	// ディゾルブの設定
	if (ImGui::CollapsingHeader("Dissolve Param")) {
		if (ImGui::TreeNode("uvTransform")) {
			ImGui::DragFloat2("uvTranslation", &dissolveUvTransform_.translate.x, 0.01f);
			ImGui::DragFloat2("uvScale", &dissolveUvTransform_.scale.x, 0.01f);
			ImGui::SliderAngle("uvRotation", &dissolveUvTransform_.rotate.z);
			ImGui::TreePop();
		}

		ImGui::ColorEdit4("dissolveColor", &dissolveParam_->color.x);
		ImGui::ColorEdit4("edgeColor", &dissolveParam_->edgeColor.x);
		ImGui::DragFloat("threshold", &dissolveParam_->threshold, 0.01f);
	}

	// 渦の設定
	if (ImGui::CollapsingHeader("Swirl Param")) {
		ImGui::DragFloat2("center", &swirlParam_->center.x, 0.01f);
		ImGui::DragFloat("swirlStrength", &swirlParam_->swirlStrength, 0.01f);
		ImGui::DragFloat("timeRate", &timeRate_, 0.01f);
	}

	// Textureの設定
	TextureManager* textureManager = TextureManager::GetInstance();
	if (ImGui::CollapsingHeader("Base Texture")) {
		textureName_ = textureManager->SelectTexture(textureName_);
	}
	if (ImGui::CollapsingHeader("Dissolve Texture")) {
		dissolveTextureName_ = textureManager->SelectTexture(dissolveTextureName_);
	}
	if (ImGui::CollapsingHeader("Mask Texture")) {
		maskTextureName_ = textureManager->SelectTexture(maskTextureName_);
	}

	if (ImGui::Button("Save")) {
		CopyData();
		JsonItems::Save(GetName(), saveItems_.ToJson(saveItems_.GetName()));
	}
	if (ImGui::Button("Apply")) {
		saveItems_.FromJson(JsonItems::GetData(GetName(), saveItems_.GetName()));
	}
}

void GhostTakenSwirl::ApplySaveData(const std::string& effectName) {
	saveItems_.SetName(effectName);
	saveItems_.FromJson(JsonItems::GetData(GetName(), saveItems_.GetName()));
	materialData_->color = saveItems_.color;
	dissolveParam_->color = saveItems_.dissolveColor;
	dissolveParam_->edgeColor = saveItems_.edgeColor;
	dissolveParam_->threshold = saveItems_.threshold;
	swirlParam_->swirlStrength = saveItems_.swirlStrength;
	timeRate_ = saveItems_.timeRate;

	textureName_ = saveItems_.baseTexture;
	dissolveTextureName_ = saveItems_.dissolveTexture;
	maskTextureName_ = saveItems_.maskTexture;
}

void GhostTakenSwirl::CopyData() {
	saveItems_.color = materialData_->color;
	saveItems_.dissolveColor = dissolveParam_->color;
	saveItems_.edgeColor = dissolveParam_->edgeColor;
	saveItems_.threshold = dissolveParam_->threshold;
	saveItems_.swirlStrength = swirlParam_->swirlStrength;
	saveItems_.timeRate = timeRate_;

	saveItems_.baseTexture = textureName_;
	saveItems_.dissolveTexture = dissolveTextureName_;
	saveItems_.maskTexture = maskTextureName_;
}
