#pragma once
#include "Engine/Module/PostEffect/BrightnessThreshold.h"
#include "Engine/Module/PostEffect/GaussianBlurWidth.h"
#include "Engine/Module/PostEffect/GaussianBlurHeight.h"
#include "Engine/Module/PostEffect/PingPongBuffer.h"
#include "Engine/Lib/Math/Vector2.h"

class Bloom :
	public IPostEffect {
public:	// 構造体

	struct BloomSettings {
		float bloomIntensity;
	};

	struct BloomParam : public IJsonConverter {
		float bloomIntensity;
		bool isEnable;
		
		BloomParam() { SetName("GaussianFilter"); }

		json ToJson(const std::string& id) const override {
			return JsonBuilder(id)
				.Add("bloomIntensity", bloomIntensity)
				.Add("isEnable", isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "bloomIntensity", bloomIntensity);
			fromJson(jsonData, "isEnable", isEnable);
		}
	};

public:

	Bloom() = default;
	~Bloom() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

	void SetPongResource(PingPongBuffer* _resource) { postProcessResource_ = _resource; }

	void SetDepthHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) { depthHandle_ = handle; }

private:

	std::unique_ptr<PingPongBuffer> pingPongBuff_;

	std::unique_ptr<BrightnessThreshold> brightnessBuffer_;
	std::unique_ptr<GaussianBlurWidth> blurWidthBuffer_;
	std::unique_ptr<GaussianBlurHeight> blurHeightBuffer_;
	
	std::unique_ptr<DxResource> settingBuffer_;
	BloomSettings* setting_;

	PingPongBuffer* postProcessResource_;
	std::unique_ptr<DxResource> sceneBuffer_;


	ID3D12Device* device_ = nullptr;
	DescriptorHeap* dxHeap_ = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE depthHandle_;

	BloomParam param_;
};

