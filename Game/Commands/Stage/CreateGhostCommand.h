#pragma once

#include "Game/Commands/IObjectCommand.h"

#include "Game/Manager/StageRegistry.h"

#include "Engine/Lib/Math/Vector2Int.h"

class CreateGhostCommand :
	public IObjectCommand{
public:
	CreateGhostCommand(StageRegistry* _stageRegistry,const Vector2Int& _index);
	virtual ~CreateGhostCommand();

	void Execute() override;
	void Undo() override;
private:
	bool isExecute_ = false;
	StageRegistry* stageRegistry_;
	Vector2Int index_;
};

