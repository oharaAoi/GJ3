#pragma once
#include "Engine/Module/PostEffect/IPostEffect.h"

class BrightnessThreshold :
	public IPostEffect {
public:	// 構造体

	struct BloomSettings {
		float threshold = 1.0f;
	};

public:

	BrightnessThreshold() = default;
	~BrightnessThreshold() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override {};
	void CopyData() override {};

private:

	std::unique_ptr<DxResource> bloomBuffer_;
	BloomSettings* bloomSetting_;

public:
	float GetThreshold() const{ return bloomSetting_->threshold; }
	void SetThreshold(float _threshold){ *bloomSetting_ = BloomSettings(_threshold); }
};

