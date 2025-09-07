#pragma once
#include <string>
#include <memory>
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Engine/Module/Components/2d/Sprite.h"

/// <summary>
/// 渦巻きTextureの透明部分をマスクするクラス
/// </summary>
class SwirlMask :
	public IPostEffect {
public:

	SwirlMask() = default;
	~SwirlMask() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

	void ApplySaveData() override {};

	void CopyData() override {};

private:

	std::unique_ptr<Sprite> sprite_;

};

