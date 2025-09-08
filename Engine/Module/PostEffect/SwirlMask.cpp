#include "SwirlMask.h"
#include "Engine.h"
#include "Engine/Core/GraphicsContext.h"

void SwirlMask::Init() {
	mask_ = std::make_unique<Sprite>();
	mask_->Init("white.png");
	mask_->ReSetTextureSize({640, 360});
	mask_->SetTranslate({ 640, 360 });

	pattern_ = std::make_unique<Sprite>();
	pattern_->Init("swirl_bg.png");
	pattern_->SetTranslate({ 640, 360 });
}

void SwirlMask::SetCommand() {
	mask_->ReSetTextureSize({ 640, 360 });
	pattern_->ReSetTextureSize({ kWindowWidth_, kWindowHeight_ });

	Pipeline* pso = Engine::SetPipeline(PSOType::Sprite, "Sprite_Normal_16.json");
	pattern_->Draw(pso);
	//mask_->Draw(pso);
}

void SwirlMask::Debug_Gui() {
	mask_->Debug_Gui();
}