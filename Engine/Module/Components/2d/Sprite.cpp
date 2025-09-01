#include "Sprite.h"
#include "Render.h"
#include "Engine/Module/Geometry/Structs/Vertices.h"
#include "Engine/Module/Components/Meshes/Mesh.h"
#include "Engine/System/Manager/TextureManager.h"
#include "Engine/System/Manager/ImGuiManager.h"

Sprite::Sprite() {}
Sprite::~Sprite() {
	vertexData_ = nullptr;
	indexData_ = nullptr;
	materialData_ = nullptr;
	transformData_ = nullptr;
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
	materialBuffer_.Reset();
	transformBuffer_.Reset();
}

void Sprite::Init(ID3D12Device* device, const std::string& fileName) {
	textureSize_ = TextureManager::GetInstance()->GetTextureSize(fileName);
	textureName_ = fileName;
	drawRange_ = textureSize_;
	leftTop_ = { 0.0f, 0.0f };
	anchorPoint_ = { 0.5f, 0.5f };

	// ----------------------------------------------------------------------------------
	vertexBuffer_ = CreateBufferResource(device, sizeof(TextureMesh) * 4);
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
	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * 6);
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
	materialBuffer_ = CreateBufferResource(device, sizeof(TextureMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->uvTransform = Matrix4x4::MakeUnit();
	materialData_->uvMinSize = { 0.0f, 0.0f };
	materialData_->uvMaxSize = { 1.0f, 1.0f };

	// ----------------------------------------------------------------------------------
	transformBuffer_ = CreateBufferResource(device, sizeof(TextureTransformData));
	transformBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	transform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };
	uvTransform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };

	transformData_->wvp = Matrix4x4(
		transform_.MakeAffine()
		* Matrix4x4::MakeUnit()
		* Matrix4x4::MakeOrthograhic(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f)
	);

	isEnable_ = true;
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

	Matrix4x4 projection = Render::GetProjection2D();
	if (isBackGround) {
		transform_.translate.z = Render::GetFarClip();
	}
	// アフィン変換行列の作成
	Matrix4x4 affineMatrix = transform_.MakeAffine();
	// テクスチャ位置を保持するための補正行列
	Matrix4x4 correctionTranslation = Vector3({ pivotOffset.x, pivotOffset.y, 0.0f }).MakeTranslateMat();

	// 最終的なスプライトの変換行列
	transformData_->wvp = Matrix4x4(
		affineMatrix *  // ピボットによる変位を元に戻す
		correctionTranslation *
		projection
	);

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
	commandList->SetGraphicsRootConstantBufferView(index, transformBuffer_->GetGPUVirtualAddress());
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
// ↓　Textureの中心位置を変更する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite::SetTranslate(const Vector2& centerPos) {
	transform_.translate.x = centerPos.x;
	transform_.translate.y = centerPos.y;
	transform_.translate.z = 0;
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
	if (ImGui::TreeNode("transform")) {
		ImGui::DragFloat3("translation", &transform_.translate.x, 0.1f);
		ImGui::DragFloat2("scale", &transform_.scale.x, 0.01f);
		ImGui::SliderAngle("rotation", &transform_.rotate.z);
		ImGui::TreePop();
	}

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
