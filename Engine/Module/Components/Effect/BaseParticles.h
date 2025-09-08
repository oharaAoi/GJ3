 #pragma once
#include <memory>
#include <list>
#include "Engine/Module/Components/GameObject/GeometryObject.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"
#include "Engine/Lib/ParticlesData.h"
#include "Engine/Render/ParticleInstancingRenderer.h"

class BaseParticles :
	public AttributeGui {
public:

	BaseParticles() = default;
	virtual ~BaseParticles() override {};

	void Init(const std::string& name);

	void Update();

	void DrawShape();

	void Emit(const Vector3& pos);

	void EmitUpdate();

	void Reset();

	void Debug_Gui() override;

	void ChangeMesh();

public:		// json関連

	json GetJsonData() const { return emitter_.ToJson(particleName_); }

	void SetJsonData(const json& _jsonData) { 
		emitter_.FromJson(_jsonData);
	};
	
public:

	void SetIsStop(bool _stop) { isStop_ = _stop; }

	void SetPos(const Vector3& pos) { emitter_.translate = pos; }

	void SetParent(const Matrix4x4& parentMat);

	std::shared_ptr<Mesh> GetMesh() const { return shape_; }

	void SetParticlesList(const std::shared_ptr<std::list<ParticleSingle>>& list) { particleArray_ = list; }

	const std::string& GetUseTexture() const { return emitter_.useTexture; }

	std::shared_ptr<Material> GetShareMaterial() { return shareMaterial_; }
	void SetShareMaterial(std::shared_ptr<Material> _material) { shareMaterial_ = _material; }

	bool GetIsAddBlend() const {return emitter_.isParticleAddBlend;}

	void SetLoop(bool _loop) { emitter_.isLoop = _loop; }

	bool GetChangeMesh() const { return changeMesh_; }

protected:

	// 最大数
	const uint32_t kMaxParticles = 200;
	// groupの名前
	const std::string kGroupName = "CPU";
	// particleName
	std::string particleName_ = "new particles";

	bool isAddBlend_;

	// meshの形状
	std::shared_ptr<Mesh> shape_;
	std::shared_ptr<Material> shareMaterial_;

	// Particleの情報
	std::shared_ptr<std::list<ParticleSingle>> particleArray_;

	// emitter
	ParticleEmit emitter_;
	float emitAccumulator_;
	float currentTimer_;
	bool isStop_;

	bool changeMesh_ = false;

	std::string meshName_;

	// 親のMatrix
	const Matrix4x4* parentWorldMat_ = nullptr;
public:
	// ========================
	// ParticleEmit Accessors
	// ========================

	// 基本
	void SetLoop(bool loop){ emitter_.isLoop = loop; }
	bool GetLoop() const{ return emitter_.isLoop; }

	void SetDuration(float duration){ emitter_.duration = duration; }
	float GetDuration() const{ return emitter_.duration; }

	void SetTranslate(const Vector3& pos){ emitter_.translate = pos; }
	const Vector3& GetTranslate() const{ return emitter_.translate; }

	void SetRotate(const Vector3& rot){ emitter_.rotate = rot; }
	const Vector3& GetRotate() const{ return emitter_.rotate; }

	void SetRateOverTime(uint32_t count){ emitter_.rateOverTimeCout = count; }
	uint32_t GetRateOverTime() const{ return emitter_.rateOverTimeCout; }

	void SetShape(int shape){ emitter_.shape = shape; }
	int GetShape() const{ return emitter_.shape; }

	void SetEmitDirection(int dir){ emitter_.emitDirection = dir; }
	int GetEmitDirection() const{ return emitter_.emitDirection; }

	void SetEmitOrigin(int origin){ emitter_.emitOrigin = origin; }
	int GetEmitOrigin() const{ return emitter_.emitOrigin; }

	// 色
	void SetIsRandomColor(bool flag){ emitter_.isRandomColor = flag; }
	bool GetIsRandomColor() const{ return emitter_.isRandomColor; }

	void SetColor(const Vector4& col){ emitter_.color = col; }
	const Vector4& GetColor() const{ return emitter_.color; }

	void SetRandColor1(const Vector4& col){ emitter_.randColor1 = col; }
	const Vector4& GetRandColor1() const{ return emitter_.randColor1; }

	void SetRandColor2(const Vector4& col){ emitter_.randColor2 = col; }
	const Vector4& GetRandColor2() const{ return emitter_.randColor2; }

	// スケール
	void SetSeparateByAxisScale(bool flag){ emitter_.separateByAxisScale = flag; }
	bool GetSeparateByAxisScale() const{ return emitter_.separateByAxisScale; }

	void SetMinScale(const Vector3& s){ emitter_.minScale = s; }
	const Vector3& GetMinScale() const{ return emitter_.minScale; }

	void SetMaxScale(const Vector3& s){ emitter_.maxScale = s; }
	const Vector3& GetMaxScale() const{ return emitter_.maxScale; }

	// 物理
	void SetSpeed(float s){ emitter_.speed = s; }
	float GetSpeed() const{ return emitter_.speed; }

	void SetLifeTime(float t){ emitter_.lifeTime = t; }
	float GetLifeTime() const{ return emitter_.lifeTime; }

	void SetGravity(float g){ emitter_.gravity = g; }
	float GetGravity() const{ return emitter_.gravity; }

	void SetDamping(float d){ emitter_.dampig = d; }
	float GetDamping() const{ return emitter_.dampig; }

	// 回転
	void SetAngleMin(float a){ emitter_.angleMin = a; }
	float GetAngleMin() const{ return emitter_.angleMin; }

	void SetAngleMax(float a){ emitter_.angleMax = a; }
	float GetAngleMax() const{ return emitter_.angleMax; }

	void SetIsDirectionRotate(bool flag){ emitter_.isDirectionRotate = flag; }
	bool GetIsDirectionRotate() const{ return emitter_.isDirectionRotate; }

	// ライフ依存挙動
	void SetIsLifeOfScale(bool flag){ emitter_.isLifeOfScale = flag; }
	bool GetIsLifeOfScale() const{ return emitter_.isLifeOfScale; }

	void SetIsLifeOfAlpha(bool flag){ emitter_.isLifeOfAlpha = flag; }
	bool GetIsLifeOfAlpha() const{ return emitter_.isLifeOfAlpha; }

	void SetIsFadeInOut(bool flag){ emitter_.isFadeInOut = flag; }
	bool GetIsFadeInOut() const{ return emitter_.isFadeInOut; }

	void SetFadeInTime(float t){ emitter_.fadeInTime = t; }
	float GetFadeInTime() const{ return emitter_.fadeInTime; }

	void SetFadeOutTime(float t){ emitter_.fadeOutTime = t; }
	float GetFadeOutTime() const{ return emitter_.fadeOutTime; }

	// 描画
	void SetIsAddBlend(bool flag){ emitter_.isParticleAddBlend = flag; }
	bool GetIsAddBlend() const{ return emitter_.isParticleAddBlend; }

	void SetIsDraw2d(bool flag){ emitter_.isDraw2d = flag; }
	bool GetIsDraw2d() const{ return emitter_.isDraw2d; }

	void SetIsBillboard(bool flag){ emitter_.isBillBord = flag; }
	bool GetIsBillboard() const{ return emitter_.isBillBord; }

	// サイズ拡大
	void SetIsScaleUp(bool flag){ emitter_.isScaleUp = flag; }
	bool GetIsScaleUp() const{ return emitter_.isScaleUp; }

	void SetScaleUpScale(const Vector3& s){ emitter_.scaleUpScale = s; }
	const Vector3& GetScaleUpScale() const{ return emitter_.scaleUpScale; }

	// エミッタ形状パラメータ
	void SetRadius(float r){ emitter_.radius = r; }
	float GetRadius() const{ return emitter_.radius; }

	void SetAngle(float a){ emitter_.angle = a; }
	float GetAngle() const{ return emitter_.angle; }

	void SetHeight(float h){ emitter_.height = h; }
	float GetHeight() const{ return emitter_.height; }

	void SetSize(const Vector3& s){ emitter_.size = s; }
	const Vector3& GetSize() const{ return emitter_.size; }

};

