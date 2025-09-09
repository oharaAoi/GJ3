#pragma once
#include <memory>
#include <string>
#include "Engine/DirectX/Resource/DxResource.h"
#include "Engine/DirectX/Utilities/DirectXUtils.h"
#include "Engine/Lib/Math/Vector2.h"
#include "Engine/Module/Components/ScreenTransform.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

class GhostEffect :
	public AttributeGui {
public:

	struct TextureMesh {
		Vector4 pos;
		Vector2 texcoord;
		float padding[2];
	};

	struct TextureMaterial {
		Vector4 color = {1,1,1,1};
		Matrix4x4 uvTransform;
	};

	struct DissolveParam {
		Matrix4x4 uvTransform;
		Vector4 color = { 1,1,1,1 };
		Vector4 edgeColor = { 1,1,1,1 };
		float threshold = 0.0f;
	};

public:

	GhostEffect() = default;
	~GhostEffect();

	void Init(const Vector2& tileSize);

	void Update();

	void Draw() const;

	void Debug_Gui() override;

public:

	void SetTextureName(const std::string& _textureName) { textureName_ = _textureName; };
	void SetDissolveTextureName(const std::string& _name) { dissolveTextureName_ = _name; }

	void SetScale(const Vector2& scale) { transform_->SetScale(scale); }
	void SetRotate(float rotate) { transform_->SetRotate(rotate); }
	void SetTranslate(const Vector2& translate) { transform_->SetTranslate(translate); }

	const Vector4& GetColor() const { return materialData_->color; }
	void SetColor(const Vector4& color) { materialData_->color = color; };

	const Vector2 GetTranslate() const { return transform_->GetTranslate(); }
	const Vector2 GetScale() const { return transform_->GetScale(); }
	const float GetRotate() const { return transform_->GetRotate(); }

	ScreenTransform* GetTransform() { return transform_.get(); }

	void SetIsDestroy(bool isDestroy) { isDestroy_ = isDestroy; }
	bool GetIsDestroy() const { return isDestroy_; }

private:

	// mesh情報
	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

	TextureMesh* vertexData_;
	uint32_t* indexData_;

	// material情報
	ComPtr<ID3D12Resource> materialBuffer_;
	TextureMaterial* materialData_;

	// Transform情報
	std::unique_ptr<ScreenTransform> transform_;
	SRT uvTransform_;

	Vector2 textureSize_;
	// 描画する範囲
	Vector2 drawRange_ = { 0.0f,0.0f };
	// 左上座標
	Vector2 leftTop_ = { 0.0f,0.0f };

	Vector2 pivotOffset_;

	// textureの名前
	std::string textureName_;
	std::string dissolveTextureName_;

	// ==========================================
	
	std::unique_ptr<DxResource> dissolveResource_;
	DissolveParam* dissolveParam_;
	SRT dissolveUvTransform_;

	bool isDestroy_ = false;

};

