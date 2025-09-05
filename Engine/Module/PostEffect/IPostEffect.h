#pragma once
#include "Engine/DirectX/Resource/DxResource.h"
#include "Engine/Lib/Json/JsonItems.h"
#include "Engine/Lib/Json/IJsonConverter.h"

class IPostEffect {
public:

	IPostEffect() = default;
	virtual ~IPostEffect() = default;

	virtual void Init() = 0;

	virtual void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) = 0;

	virtual void CheckBox() = 0;

	virtual void Debug_Gui() = 0;

	virtual void ApplySaveData() = 0;
	virtual void CopyData() = 0;

public:

	void SetIsEnable(bool _isEnable) { isEnable_ = _isEnable; }
	bool GetIsEnable() const { return isEnable_; }

protected:

	bool isEnable_ = false;

};

