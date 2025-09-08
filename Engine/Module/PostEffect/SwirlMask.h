#pragma once
#include <string>
#include <memory>
#include "Engine/Module/Components/2d/Sprite.h"

/// <summary>
/// 渦巻きTextureの透明部分をマスクするクラス
/// </summary>
class SwirlMask {
public:

	SwirlMask() = default;
	~SwirlMask() = default;

	void Init() ;

	void SetCommand();

	void Debug_Gui();

public:

	void SetPatternAlpha(float _alpha) { pattern_->SetAlpha(_alpha); }

private:

	std::unique_ptr<Sprite> mask_;
	std::unique_ptr<Sprite> pattern_;

};

