#pragma once
#include <memory>
#include <string>
#include "Engine/DirectX/Resource/DxResource.h"
#include "Engine/DirectX/Utilities/DirectXUtils.h"
#include "Engine/Lib/Math/Vector2.h"
#include "Engine/Lib/Json/IJsonConverter.h"
#include "Engine/Module/Components/ScreenTransform.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

class GhostTakenSwirl :
	public AttributeGui {
public:

	struct TextureMesh {
		Vector4 pos;
		Vector2 texcoord;
		float padding[2];
	};

	struct TextureMaterial {
		Vector4 color = { 1,1,1,1 };
		Matrix4x4 uvTransform;
	};

	struct DissolveParam {
		Matrix4x4 uvTransform;
		Vector4 color = { 1,1,1,1 };
		Vector4 edgeColor = { 1,1,1,1 };
		float threshold = 0.0f;
	};

	struct SwirlParam {
		Vector2 center;        // 渦の中心 (0~1, uv空間で指定)
		float swirlStrength;  // 渦の強さ (正で右巻き, 負で左巻き)
		float time;           // アニメーション回転 (不要なら0)
	};

	struct SaveItems : public IJsonConverter {
		Vector4 color = { 1,1,1,1 };
		Vector4 dissolveColor = { 1,1,1,1 };
		Vector4 edgeColor = { 1,1,1,1 };
		float threshold = 0.0f;

		std::string baseTexture = "white.png";
		std::string dissolveTexture = "white.png";

		SaveItems() { SetName("GhostEffect"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("color", color)
				.Add("dissolveColor", dissolveColor)
				.Add("edgeColor", edgeColor)
				.Add("threshold", threshold)
				.Add("baseTexture", baseTexture)
				.Add("dissolveTexture", dissolveTexture)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "color", color);
			fromJson(jsonData, "dissolveColor", dissolveColor);
			fromJson(jsonData, "edgeColor", edgeColor);
			fromJson(jsonData, "threshold", threshold);
			fromJson(jsonData, "baseTexture", baseTexture);
			fromJson(jsonData, "dissolveTexture", dissolveTexture);
		}
	};

public:

	GhostTakenSwirl() = default;
	~GhostTakenSwirl();

	void Init(const Vector2& tileSize);

	void Update();

	void Draw() const;

	void Debug_Gui() override;

	void ApplySaveData(const std::string& effectName);

	void CopyData();

public:

	// textureName
	void SetTextureName(const std::string& _textureName) { textureName_ = _textureName; };
	void SetDissolveTextureName(const std::string& _name) { dissolveTextureName_ = _name; }

	// transform
	void SetScale(const Vector2& scale) { transform_->SetScale(scale); }
	void SetRotate(float rotate) { transform_->SetRotateZ(rotate); }
	void SetTranslate(const Vector2& translate) { transform_->SetTranslate(translate); }

	const Vector2& GetTranslate() const { return transform_->GetTranslate(); }
	const Vector2& GetScale() const { return transform_->GetScale(); }
	const float GetRotate() const { return transform_->GetRotateZ(); }

	// uvTransform
	void SetUvScale(const Vector3& _scale) { uvTransform_.scale = _scale; }
	void SetUvRotate(const Vector3& _rotate) { uvTransform_.rotate = _rotate; }
	void SetUvTranslate(const Vector3& _translate) { uvTransform_.translate = _translate; }

	const Vector3& GetUvScale() const { return uvTransform_.scale; }
	const Vector3& GetUvRotate() const { return uvTransform_.rotate; }
	const Vector3& GetUvTranslate() const { return uvTransform_.translate; }

	// dissolveUvTransform
	void SetDissolveUvScale(const Vector3& _scale) { dissolveUvTransform_.scale = _scale; }
	void SetDissolveUvRotate(const Vector3& _rotate) { dissolveUvTransform_.rotate = _rotate; }
	void SetDissolveUvTranslate(const Vector3& _translate) { dissolveUvTransform_.translate = _translate; }

	const Vector3& GetDissolveUvScale() const { return dissolveUvTransform_.scale; }
	const Vector3& GetDissolveUvRotate() const { return dissolveUvTransform_.rotate; }
	const Vector3& GetDissolveUvTranslate() const { return dissolveUvTransform_.translate; }

	// color
	const Vector4& GetColor() const { return materialData_->color; }
	void SetColor(const Vector4& color) { materialData_->color = color; }

	// dissolveColor
	const Vector4& GetDissolveColor() const { return dissolveParam_->color; }
	void SetDissolveColor(const Vector4& color) { dissolveParam_->color = color; }

	// edgeColor
	const Vector4& GetDissolveEdgeColor() const { return dissolveParam_->edgeColor; }
	void SetDissolveEdgeColor(const Vector4& color) { dissolveParam_->edgeColor = color; }

	ScreenTransform* GetTransform() { return transform_.get(); }

	void SetIsDestroy(bool isDestroy) { isDestroy_ = isDestroy; }
	bool GetIsDestroy() const { return isDestroy_; }

	void SetTextureSize(const Vector2& _size) { textureSize_ = _size; }
	const Vector2& GetTextureSize() const { return textureSize_; }

	// しきい値
	void SetThreshold(float _threshold) { dissolveParam_->threshold = _threshold; }
	float GetThreshold() const { return dissolveParam_->threshold; }

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

	// ディゾルブ
	std::unique_ptr<DxResource> dissolveResource_;
	DissolveParam* dissolveParam_;
	SRT dissolveUvTransform_;

	// 渦
	std::unique_ptr<DxResource> swirlResource_;
	SwirlParam* swirlParam_;
	float timeRate_ = 1.0f;

	// mask
	std::string maskTextureName_;

	bool isDestroy_ = false;

	SaveItems saveItems_;

};

