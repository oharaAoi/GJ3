#pragma once
#include <string>
#include "Engine/Module/PostEffect/IPostEffect.h"
#include "Engine/Lib/Math/MyMatrix.h"
#include "Engine/Lib/Math/MathStructures.h"

/// <summary>
/// 2d画像を利用して行うGotRay
/// </summary>
class ScreenGotRay :
	public IPostEffect {
public:

	struct GotRayParameter {
		Vector4 color;
		Vector2 pos;
		float pad[2];
		float angle;	// 光の角度
		float spread;	// 光の広がり
		float cutoff;	// 光筋のカットオフ
		float falloff;	// 下方向へのフェード
		float edgeFade;	// 左右端のフェード
		float speed;
		float ray1Density;
		float ray2Density;
		float ray3Density;
		float seed;
		float time;
	};

public:

	ScreenGotRay() = default;
	~ScreenGotRay() override;

	void Init() override;

	void SetCommand(ID3D12GraphicsCommandList* commandList, DxResource* pingResource) override;

	void CheckBox() override;

	void Debug_Gui() override;

private:

	std::unique_ptr<DxResource> cBuffer_;
	GotRayParameter* setting_;

};

