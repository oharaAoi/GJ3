#pragma once
#include "Engine/Module/PostEffect/BrightnessThreshold.h"
#include "Engine/Module/PostEffect/GaussianBlurWidth.h"
#include "Engine/Module/PostEffect/GaussianBlurHeight.h"
#include "Engine/Module/PostEffect/PingPongBuffer.h"
#include "Engine/Lib/Math/Vector2.h"

class Bloom :
	public IPostEffect{
public:	// 構造体

	struct BloomSettings{
		float bloomIntensity;
	};

	struct BloomParam : public IJsonConverter{
		float bloomIntensity = 0.5f;
		bool isEnable = false;

		BloomParam(){ SetName("GaussianFilter"); }

		json ToJson(const std::string& id) const override{
			return JsonBuilder(id)
				.Add("bloomIntensity",bloomIntensity)
				.Add("isEnable",isEnable)
				.Build();
		}

		void FromJson(const json& jsonData) override{
			fromJson(jsonData,"bloomIntensity",bloomIntensity);
			fromJson(jsonData,"isEnable",isEnable);
		}
	};

public:

	Bloom() = default;
	~Bloom() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList,DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override;

	void CopyData() override;

	void SetPongResource(PingPongBuffer* _resource){ postProcessResource_ = _resource; }

	void SetDepthHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle){ depthHandle_ = handle; }

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
public:
	const BloomParam& GetParam() const{ return param_; }

	bool isEnable() const{ return param_.isEnable; }
	void SetEnable(const bool& enable){ param_.isEnable = enable; }

	float GetIntensity() const{ return param_.bloomIntensity; }
	void SetIntensity(const float& intensity){ param_.bloomIntensity = intensity; }

	float GetThreshold() const{ return brightnessBuffer_->GetThreshold(); }

	const Vector2& GetGaussianWidthTexelSize() const{ return blurWidthBuffer_->GetTexelSize(); }
	void SetGaussianWidthTexelSize(const Vector2& size){ blurWidthBuffer_->SetTexelSize(size); }

	const Vector2& GetGaussianHeightTexelSize() const{ return blurHeightBuffer_->GetTexelSize(); }
	void SetGaussianHeightTexelSize(const Vector2& size){ blurHeightBuffer_->SetTexelSize(size); }
};

