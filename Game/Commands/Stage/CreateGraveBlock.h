#pragma once

#include "Game/Commands/IObjectCommand.h"

class StageRegistry;

/// math
#include "Engine/Lib/Math/Vector2Int.h"
/// <summary>
/// 墓ブロックを作成するコマンドクラス
/// </summary>
class CreateGraveBlock :
	public IObjectCommand{
public:
	CreateGraveBlock(StageRegistry* _stageRegistry,Vector2Int _index);
	~CreateGraveBlock() override ;

	void Execute() override;
	void Undo() override;
private:
	StageRegistry* stageRegistry_;
	Vector2Int index_;
	bool isSpecialBlock_;
};

