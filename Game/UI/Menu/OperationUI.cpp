#include "OperationUI.h"

#include "Engine.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Engine/Lib/GameTimer.h"

void OperationUI::Init()
{
	SetName("OperationUI");
	operation_ = Engine::GetCanvas2d()->AddSprite("Menu_operation.png", GetName(), "Sprite_Normal.json", 12);

	AddChild(operation_);
	EditorWindows::AddObjectWindow(this, GetName());
	
	operation_->SetTranslate(Vector2{ 640.0f,360.0f });
	//operation_->ReSetTextureSize(Vector2{ 960.0f,540.0f });
}

void OperationUI::Update(bool openOperation)
{
	if (openOperation) {
		fadeFrame_ += GameTimer::DeltaTime();	// FadeIn
	} else {
		fadeFrame_ -= GameTimer::DeltaTime();	// FadeOut
	}
	// 範囲外に行かないように
	float alpha = fadeFrame_ = std::clamp(fadeFrame_, 0.0f, 1.0f);
	// alphaをセットする
	operation_->SetColor(Vector4{ 1.0f,1.0f,1.0f,alpha });
}

void OperationUI::Debug_Gui()
{
}
