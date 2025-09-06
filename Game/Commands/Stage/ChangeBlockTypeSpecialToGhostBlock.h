#pragma once

#include "Game/Commands/IObjectCommand.h"

class StageRegistry;

/// math
#include "Engine/Lib/Math/Vector2Int.h"

class ChangeBlockTypeSpecialToGhostBlock
	:public IObjectCommand{
public:
	ChangeBlockTypeSpecialToGhostBlock(StageRegistry* _stageBlock,const Vector2Int& _index);
	~ChangeBlockTypeSpecialToGhostBlock() override;

	void Execute() override;
	void Undo() override;

private:
	StageRegistry* pStageRegistry_ = nullptr;
	Vector2Int index_ = {0,0};
};

