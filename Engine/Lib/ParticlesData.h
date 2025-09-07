
#pragma once
#include "Engine/Lib/Math/Vector3.h"
#include "Engine/Lib/Math/Vector4.h"
#include "Engine/Lib/Math/Quaternion.h"
#include "Engine/Lib/Math/Matrix4x4.h"
#include "Engine/Lib/Json/IJsonConverter.h"

enum class CpuEmitDirection {
	UP,			// 上方向に射出
	RANDOM,		// ランダム方向に射出
	OUTSIDE,	// 中心から反対に射出
	CENTERFOR	// 中心に向かって射出
};

enum class CpuEmitOrigin {
	CENTER,		// 中心から
	RANGE,		// 範囲からランダムに
};

enum class CpuEmitterShape {
	SPHERE,
	BOX,
	CONE
};

struct ParticleSingle {
	Vector4 color;			// 色
	Vector3 scale;			// 拡縮
	Quaternion rotate;		// 回転
	Vector3 translate;		// 座標
	Vector3 velocity;		// 速度
	Vector3 acceleration;	// 加速度
	Vector3 firstScale;		// 初期拡縮
	Vector3 emitterCenter;	// Emitterの中心
	float lifeTime;			// 生存時間
	float currentTime;		// 現在の時間
	float damping;			// 減衰
	float gravity;			// 重力
	bool isLifeOfScale = false;	// 生存時間によるサイズ
	bool isLifeOfAlpha = false;	// 生存時間による透明度
	bool isFadeInOut = false;
	float fadeInTime = 0.0f;
	float fadeOutTime = 0.0f;
	float initAlpha_ = 0.0f;
	bool isScaleUpScale = false;	// 生存時間による透明度
	bool isAddBlend = true;
	Vector3 upScale;
	bool isBillBord = true;
	bool isDraw2d = true;
	bool isCenterFor = false; 
};

struct ParticleEmit : public IJsonConverter {
	bool isLoop = true;						// Loopをするか
	float duration = 5.0f;					// 継続時間
	Vector3 translate = CVector3::ZERO;		// 位置
	Vector3 rotate = CVector3::ZERO;		// 射出方向
	uint32_t rateOverTimeCout = 10;			// 射出数
	int shape = 0;						// emitterの種類
	int emitDirection = 1;
	int emitOrigin = 0;
	
	// particle自体のparameter
	bool isRandomColor = false;
	Vector4 color = Vector4{ 1,1,1,1 };			// 色
	Vector4 randColor1 = Vector4{ 1,0,0,1 };	// 色
	Vector4 randColor2 = Vector4{ 0,0,1,1 };	// 色
	bool separateByAxisScale = false;
	Vector3 minScale = CVector3::UNIT;			// 最小の大きさ
	Vector3 maxScale = CVector3::UNIT;			// 最大の大きさ
	float speed = 1.0f;							// 速度
	float lifeTime = 5.0f;						// particleの生存時間
	float gravity = 0;							// 重力を付与するか
	float dampig = 0;							// 減衰率
	float angleMin = 0.0f;						// 最小の回転
	float angleMax = 360.0f;					// 最大の回転
	bool isDirectionRotate = false;				// 進行方向にParticleを向ける処理
	bool isLifeOfScale = false;					// 生存時間によるサイズ
	bool isLifeOfAlpha = false;					// 生存時間による透明度
	bool isFadeInOut = false;					// FadeInOutで出現
	float fadeInTime = 0;
	float fadeOutTime = 0;
	
	bool isParticleAddBlend = false;			// blendModeをAddBlendにするかのフラグ
	bool isDraw2d = false;						// blendModeをAddBlendにするかのフラグ
	bool isBillBord = true;

	bool isScaleUp;								// サイズを大きくするか
	Vector3 scaleUpScale;

	float radius = 0.5f;
	float angle = 27.f;
	float height = 1;
	Vector3 size = CVector3::UNIT;

	std::string useTexture = "circle.png";
	std::string useMesh = "plane";

	ParticleEmit() {
		toJsonFunction_ = [this](const std::string& id) {
			return this->ToJson(id);
			};
	}

	json ToJson(const std::string& id) const override {
		return JsonBuilder(id)
			.Add("isLoop", isLoop)
			.Add("duration", duration)
			.Add("rotate", rotate)
			.Add("translate", translate)
			.Add("shape", shape)
			.Add("rateOverTimeCout", rateOverTimeCout)
			.Add("emitDirection", emitDirection)
			.Add("emitOrigin", emitOrigin)
			.Add("color", color)
			.Add("isRandomColor", isRandomColor)
			.Add("randColor1", randColor1)
			.Add("randColor2", randColor2)
			.Add("separateByAxisScale", separateByAxisScale)
			.Add("minScale", minScale)
			.Add("maxScale", maxScale)
			.Add("speed", speed)
			.Add("lifeTime", lifeTime)
			.Add("gravity", gravity)
			.Add("damping", dampig)  // `damping` の変数名のスペルを修正
			.Add("angleMin", angleMin)
			.Add("angleMax", angleMax)
			.Add("isDirectionRotate", isDirectionRotate)
			.Add("isLifeOfScale", isLifeOfScale)
			.Add("isLifeOfAlpha", isLifeOfAlpha)
			.Add("isFadeInOut", isFadeInOut)
			.Add("fadeInTime", fadeInTime)
			.Add("fadeOutTime", fadeOutTime)
			.Add("isParticleAddBlend", isParticleAddBlend)
			.Add("isDraw2d", isDraw2d)
			.Add("isScaleUp", isScaleUp)
			.Add("scaleUpScale", scaleUpScale)
			.Add("isBillBord", isBillBord)
			.Add("useTexture", useTexture)
			.Add("useMesh", useMesh)
			.Add("radius", radius)
			.Add("size", size)
			.Add("angle", angle)
			.Add("height", height)
			.Build();
	}

	void FromJson(const json& jsonData) override {
		fromJson(jsonData, "isLoop", isLoop);
		fromJson(jsonData, "duration", duration);
		fromJson(jsonData, "rotate", rotate);
		fromJson(jsonData, "translate", translate);
		fromJson(jsonData, "shape", shape);
		fromJson(jsonData, "rateOverTimeCout", rateOverTimeCout);
		fromJson(jsonData, "emitDirection", emitDirection);
		fromJson(jsonData, "emitOrigin", emitOrigin);
		fromJson(jsonData, "color", color);
		fromJson(jsonData, "isRandomColor", isRandomColor);
		fromJson(jsonData, "randColor1", randColor1);
		fromJson(jsonData, "randColor2", randColor2);
		fromJson(jsonData, "separateByAxisScale", separateByAxisScale);
		fromJson(jsonData, "minScale", minScale);
		fromJson(jsonData, "maxScale", maxScale);
		fromJson(jsonData, "speed", speed);
		fromJson(jsonData, "lifeTime", lifeTime);
		fromJson(jsonData, "gravity", gravity);
		fromJson(jsonData, "damping", dampig);  // `damping` の変数名のスペルを修正
		fromJson(jsonData, "angleMin", angleMin);
		fromJson(jsonData, "angleMax", angleMax);
		fromJson(jsonData, "isDirectionRotate", isDirectionRotate);
		fromJson(jsonData, "isLifeOfScale", isLifeOfScale);
		fromJson(jsonData, "isLifeOfAlpha", isLifeOfAlpha);
		fromJson(jsonData, "isFadeInOut", isFadeInOut);
		fromJson(jsonData, "fadeInTime", fadeInTime);
		fromJson(jsonData, "fadeOutTime", fadeOutTime);
		fromJson(jsonData, "isParticleAddBlend", isParticleAddBlend);
		fromJson(jsonData, "isDraw2d", isDraw2d);
		fromJson(jsonData, "isScaleUp", isScaleUp);
		fromJson(jsonData, "scaleUpScale", scaleUpScale);
		fromJson(jsonData, "isBillBord", isBillBord);
		fromJson(jsonData, "useTexture", useTexture);
		fromJson(jsonData, "useMesh", useMesh);
		fromJson(jsonData, "radius", radius);
		fromJson(jsonData, "size", size);
		fromJson(jsonData, "angle", angle);
		fromJson(jsonData, "height", height);
	}

	void Attribute_Gui();
};
