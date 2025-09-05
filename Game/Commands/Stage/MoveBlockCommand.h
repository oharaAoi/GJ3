#pragma once

#include "Game/Commands/IObjectCommand.h"

class StageRegistry;

/// math
#include "Engine/Lib/Math/Vector2Int.h"

class MoveBlockCommand
	:public IObjectCommand{
public:
	MoveBlockCommand(class StageRegistry* _stageRegistry,const Vector2Int& _fromIndex,const Vector2Int& _toIndex);
	~MoveBlockCommand() override;
	void Execute() override;
	void Undo() override;
private:
	StageRegistry* stageRegistry_;
	Vector2Int fromIndex_;
	Vector2Int toIndex_;
};

