#pragma once
#include "Engine/DirectX/Pipeline/Pipeline.h"
#include "Engine/DirectX/Utilities/DirectXUtils.h"
#include "Engine/Lib/Math/Vector2.h"
#include "Engine/Lib/Math/MathStructures.h"
#include "Engine/Lib/Json/IJsonConverter.h"
#include "Engine/Module/Components/ScreenTransform.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

class Render;

class Sprite :
	public AttributeGui{
public:

	struct TextureMesh{
		Vector4 pos;
		Vector2 texcoord;
		float padding[2];
	};

	struct TextureMaterial{
		Vector4 color;
		Matrix4x4 uvTransform;
		Vector2 uvMinSize;		// 0~1の範囲で指定
		Vector2 uvMaxSize;		// 0~1の範囲で指定
	};

	struct SpriteData : public IJsonConverter {
		// material関連
		Vector4 color = {1,1,1,1};
		Vector3 uvScale = CVector3::UNIT;
		Vector3 uvRotate = CVector3::ZERO;
		Vector3 uvTranslate = CVector3::ZERO;
		Vector2 uvMinSize = {0,0};		// 0~1の範囲で指定
		Vector2 uvMaxSize = {1,1};		// 0~1の範囲で指定

		// baseParam
		Vector2 scale = {1,1};
		Vector3 rotate = CVector3::ZERO;
		Vector3 centerPos = CVector3::ZERO;
		Vector2 textureSize = { 1,1 };
		Vector2 drawRange = { 1,1 };
		Vector2 leftTop = { 0,0 };
		Vector2 anchorPoint = { 0.5f, 0.5f };

		bool isFlipX = false;	// 左右フリップ
		bool isFlipY = false;	// 上下フリップ

		bool isBackGround = false;
		bool isFront = false;

		std::string textureName = "white.png";

		json ToJson(const std::string& id) const override;

		void FromJson(const json& jsonData) override;
	};

public:

	Sprite();
	~Sprite();

	void Init(const std::string& fileName);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="rectRange">: 描画する範囲</param>
	/// <param name="leftTop">: 左上座標</param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pipeline">: 描画パイプラインのポインタ</param>
	void Draw(const Pipeline* pipeline);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void PostDraw(ID3D12GraphicsCommandList* commandList,const Pipeline* pipeline) const;

	void Debug_Gui() override;

	void ApplySaveData();

public:

	/// <summary>
	/// Textureをセットする
	/// </summary>
	/// <param name="fileName">: セットするTexture名</param>
	void ReSetTexture(const std::string& fileName);

	/// <summary>
	/// Textureの中心位置を変える(Screen座標系)
	/// </summary>
	/// <param name="centerPos">: position</param>
	void SetTranslate(const Vector2& centerPos){ transform_->SetTranslate(centerPos); };

	/// <summary>
	/// Textureのサイズを再設計する
	/// </summary>
	/// <param name="size"></param>
	void ReSetTextureSize(const Vector2& size);

	// 描画する範囲の設定
	void SetDrawRange(const Vector2& rectRange){ drawRange_ = rectRange; };
	// 描画する範囲の設定
	void SetLeftTop(const Vector2& leftTop){ leftTop_ = leftTop; }
	// Pivotの位置を変更する
	void SetAnchorPoint(const Vector2& point){ anchorPoint_ = point; }

	void SetScale(const Vector2 scale){ transform_->SetScale(scale); }
	void SetRotate(float rotate){ transform_->SetRotate(rotate); }

	const Vector4& GetColor() const{ return materialData_->color; }
	void SetColor(const Vector4& color){ materialData_->color = color; };
	void SetIsFlipX(bool isFlipX){ isFlipX_ = isFlipX; }
	void SetIsFlipY(bool isFlipY){ isFlipY_ = isFlipY; }

	/// <summary>
	/// UVを直接する
	/// </summary>
	/// <param name="range"></param>
	void SetUvMinSize(const Vector2& range){ materialData_->uvMinSize = range; }

	void SetUvMaxSize(const Vector2& range){ materialData_->uvMaxSize = range; }

	void FillAmount(float amount,int type);

	void SetTextureName(const std::string& _textureName) { textureName_ = _textureName; };

	const Vector2 GetTranslate() const{ return transform_->GetTranslate(); }
	const Vector2 GetScale() const{ return transform_->GetScale(); }
	const float GetRotate() const{ return transform_->GetRotate(); }
	const Vector2 GetTextureSize() const{ return textureSize_; }
	const Vector2 GetSpriteSize() const{ return spriteSize_; }
	const bool GetIsFlipX() const{ return isFlipX_; }
	const bool GetIsFlipY() const{ return isFlipY_; }

	const bool GetEnable() const{ return isEnable_; }
	void SetEnable(bool _isEnable){ isEnable_ = _isEnable; }

	void SetIsDestroy(bool isDestroy){ isDestroy_ = isDestroy; }
	bool GetIsDestroy() const{ return isDestroy_; }

	void SetIsBackGround(bool _isBackGround){ isBackGround_ = _isBackGround; }
	bool GetIsBackGround() const{ return isBackGround_; }

	void SetIsFront(bool _isFront){ isFront_ = _isFront; }

	ScreenTransform* GetTransform(){ return transform_.get(); }

	void SetTextureResource(DxResource* _resource){ textureResource_ = _resource; }

private:

	void SaveData();

private:

	bool isEnable_;
	bool isDestroy_;
	bool isBackGround_;
	bool isFront_;

	DxResource* textureResource_;

	// 定数バッファ
	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;
	ComPtr<ID3D12Resource> materialBuffer_;

	// view
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

	// データ
	TextureMesh* vertexData_;
	uint32_t* indexData_;
	TextureMaterial* materialData_;

	// Transform情報
	std::unique_ptr<ScreenTransform> transform_;
	SRT uvTransform_;

	std::string textureName_;

	// 描画する範囲
	Vector2 drawRange_ = {0.0f,0.0f};
	// 左上座標
	Vector2 leftTop_ = {0.0f,0.0f};
	Vector2 centerPos_;

	Vector2 anchorPoint_;

	bool isFlipX_ = false;	// 左右フリップ
	bool isFlipY_ = false;	// 上下フリップ

	// Textureのサイズ
	Vector2 textureSize_;
	// このspriteのサイズ
	Vector2 spriteSize_;

	SpriteData spriteData_;
};