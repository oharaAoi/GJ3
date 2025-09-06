#pragma once

#include "Game/Commands/IObjectCommand.h"

class StageRegistry;

/// math
#include "Engine/Lib/Math/Vector2Int.h"

class ChangeBlockTypeGhostBlockToSpecial
	:public IObjectCommand{
public:
	ChangeBlockTypeGhostBlockToSpecial(StageRegistry* _stage,const Vector2Int& _index);
	~ChangeBlockTypeGhostBlockToSpecial() override;
	void Execute() override;
	void Undo() override;
private:
	StageRegistry* stage_;
	Vector2Int index_;
};