#pragma once
#include "Engine.h"
#include "Engine/Module/Components/2d/BaseEntity2d.h"
#include "Engine/Module/Components/2d/Canvas2d.h"
#include "Engine/System/Editer/Window/EditorWindows.h"
#include "Game/Information/BlockType.h"

/// <summary>
/// Blockの基底クラス
/// </summary>
class IBlock : 
	public BaseEntity2d {
public:

	IBlock() = default;
	virtual ~IBlock() = default;

	virtual void Init() = 0;
	
	virtual void Update() = 0;

	virtual void Debug_Gui() override = 0;

protected:

	BlockType type_ = BlockType::None;

};

