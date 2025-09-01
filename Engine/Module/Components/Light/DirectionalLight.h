#pragma once
#include <functional>
#include "BaseLight.h"
#include "Engine/Module/Components/Attribute/AttributeGui.h"

class DirectionalLight :
	public BaseLight,
	public AttributeGui {
public: // メンバ構造体

	struct DirectionalLightData {
		Matrix4x4 viewProjection;
		Vector4 color; // ライトの色
		Vector3 direction; // 方向
		float pad;
		Vector3 eyePos;
		float intensity; // 輝度
		float limPower; // リムライトの強さ
	};

	struct Paramter : public IJsonConverter {
		Vector4 color = Vector4(1,1,1,1);		// ライトの色
		Vector3 direction = Vector3(0,-1,0);	// 方向
		float intensity = 1.0f;	// 輝度
		float limPower = 0.5f;		// リムライトの強さ

		Paramter() {
			toJsonFunction_ = [this](const std::string& id) {
				return this->ToJson(id);
				};
		}

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("color", color)
				.Add("direction", direction)
				.Add("intensity", intensity)
				.Add("limPower", limPower)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "color", color);
			fromJson(jsonData, "direction", direction);
			fromJson(jsonData, "intensity", intensity);
			fromJson(jsonData, "limPower", limPower);
		}
	};

public:

	DirectionalLight();
	~DirectionalLight();

	void Init(ID3D12Device* device, const size_t& size) override;

	void Finalize() override;

	void Update() override;

	void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) override;

	void Reset();

public:

	void Debug_Gui() override;

	void SetEyePos(const Vector3& pos) { directionalLightData_->eyePos = pos; }

	void SetIntensity(float _intensity) { directionalLightData_->intensity = _intensity; }

private:

	DirectionalLightData* directionalLightData_;

	Paramter parameter_;

	bool isActive_;
};