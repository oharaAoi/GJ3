#pragma once

#include "Game/Commands/IObjectCommand.h"

class StageRegistry;
class MapCollisionSystem;

/// math
#include "Engine/Lib/Math/Vector2Int.h"

class MoveBlockCommand
	:public IObjectCommand{
public:
	MoveBlockCommand(class StageRegistry* _stageRegistry,MapCollisionSystem* _mapCollisionSystem,const Vector2Int& _fromIndex,const Vector2Int& _toIndex);
	~MoveBlockCommand() override;
	void Execute() override;
	void Undo() override;
private:
	StageRegistry* stageRegistry_;
	MapCollisionSystem* mapCollisionSystem_;
	Vector2Int fromIndex_;
	Vector2Int toIndex_;

	bool isExecute_ = false;
};

