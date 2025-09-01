#include "Sprite.h"
#include "Render.h"
#include "Engine/Module/Geometry/Structs/Vertices.h"
#include "Engine/Core/GraphicsContext.h"
#include "Engine/System/Manager/TextureManager.h"
#include "Engine/System/Manager/ImGuiManager.h"

Sprite::Sprite() {}
Sprite::~Sprite() {
	vertexData_ = nullptr;
	indexData_ = nullptr;
	materialData_ = nullptr;
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
	materialBuffer_.Reset();
	transform_.reset();
}

void Sprite::Init(const std::string& fileName) {
	SetName(fileName);
	GraphicsContext* ctx = GraphicsContext::GetInstance();
	ID3D12Device* pDevice = ctx->GetDevice();

	textureSize_ = TextureManager::GetInstance()->GetTextureSize(fileName);
	textureName_ = fileName;
	drawRange_ = textureSize_;
	leftTop_ = { 0.0f, 0.0f };
	anchorPoint_ = { 0.5f, 0.5f };

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
		textureSize_.x * anchorPoint_.x,
		textureSize_.y * anchorPoint_.y,
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
	materialData_->uvMinSize = { 0.0f, 0.0f };
	materialData_->uvMaxSize = { 1.0f, 1.0f };

	// ----------------------------------------------------------------------------------
	transform_ = std::make_unique<ScreenTransform>();
	transform_->Init(pDevice);
	
	uvTransform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };

	isEnable_ = true;
	isDestroy_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::Update() {
	materialData_->uvTransform = uvTransform_.MakeAffine();
	
	// -------------------------------------------------
	// ↓ UVの変更
	// -------------------------------------------------
	materialData_->uvTransform.m[0][0] = drawRange_.x / textureSize_.x;	// Xスケーリング
	materialData_->uvTransform.m[1][1] = drawRange_.y / textureSize_.y;	// Yスケーリング
	materialData_->uvTransform.m[3][0] = leftTop_.x / textureSize_.x;	// Xオフセット
	materialData_->uvTransform.m[3][1] = leftTop_.y / textureSize_.y;	// Yオフセット
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画前処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::Draw(const Pipeline* pipeline, bool isBackGround) {
	Vector2 pivotOffset = {
		(anchorPoint_.x - 0.5f) * textureSize_.x,  // ピボットオフセット（中心からのオフセット）
		(anchorPoint_.y - 0.5f) * textureSize_.y   // ピボットオフセット（中心からのオフセット）
	};

	Matrix4x4 projection = Render::GetViewport2D() * Render::GetProjection2D();
	if (isBackGround) {
		transform_->SetTranslateZ(Render::GetFarClip());
	}
	
	// テクスチャ位置を保持するための補正行列
	Matrix4x4 correctionTranslation = Vector3({ pivotOffset.x, pivotOffset.y, 0.0f }).MakeTranslateMat();
	transform_->Update(correctionTranslation, projection);

	Render::DrawSprite(this, pipeline);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　実際にDrawCallを呼び出す
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::PostDraw(ID3D12GraphicsCommandList* commandList, const Pipeline* pipeline) const {
	UINT index = pipeline->GetRootSignatureIndex("gMaterial");
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(index, materialBuffer_->GetGPUVirtualAddress());
	index = pipeline->GetRootSignatureIndex("gTransformationMatrix");
	transform_->BindCommand(commandList, index);
	index = pipeline->GetRootSignatureIndex("gTexture");
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName_, index);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Textureを再設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::ReSetTexture(const std::string& fileName) {
	textureName_ = fileName;
	textureSize_ = TextureManager::GetInstance()->GetTextureSize(fileName);
	drawRange_ = textureSize_;
	leftTop_ = { 0.0f, 0.0f };

	Vector3 pivotOffset = {
		textureSize_.x * anchorPoint_.x,
		textureSize_.y * anchorPoint_.y,
		0.0f
	};

	// スプライトのサイズを基に頂点を設定
	RectangleVertices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
		{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Textureのサイズを再設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::ReSetTextureSize(const Vector2& size) {
	Vector3 pivotOffset = {
		size.x * anchorPoint_.x,
		size.y * anchorPoint_.y,
		0.0f
	};

	// スプライトのサイズを基に頂点を設定
	RectangleVertices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{size.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, size.y - pivotOffset.y, 0.0f , 1.0f},
		{size.x - pivotOffset.x, size.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };
}

void Sprite::FillAmount(float amount, int type) {
	// 右が動く
	if (type == 0) {
		materialData_->uvMaxSize = Vector2(amount, 1.0f);

		// 中心から
	} else if (type == 1) {
		float halfAmount = amount * 0.5f;
		materialData_->uvMaxSize = Vector2(1.0f - (0.5f - halfAmount), 1.0f);
		materialData_->uvMinSize = Vector2(0.5f - halfAmount, 0.0f);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::Debug_Gui() {
	transform_->Debug_Gui();

	if (ImGui::TreeNode("uv")) {
		ImGui::DragFloat2("uvTranslation", &uvTransform_.translate.x, 0.01f);
		ImGui::DragFloat2("uvScale", &uvTransform_.scale.x, 0.01f);
		ImGui::SliderAngle("uvRotation", &uvTransform_.rotate.z);
		ImGui::TreePop();
	}

	ImGui::DragFloat2("anchorPoint", &anchorPoint_.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat2("textureSize", &textureSize_.x, 1.0f);
	ImGui::DragFloat2("drawRange", &drawRange_.x, 1.0f);
	ImGui::DragFloat2("leftTop", &leftTop_.x, 1.0f);
	ImGui::DragFloat2("uvMin", &materialData_->uvMinSize.x, 0.01f);
	ImGui::DragFloat2("uvMax", &materialData_->uvMaxSize.x, 0.01f);

	ImGui::ColorEdit4("color", &materialData_->color.x);

}
