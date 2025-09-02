#pragma once

#include "StageRegistry.h"

class MapCollisionSystem
{
public:

	void Init(StageRegistry* stageRegistry);

	void Update();

private:

	StageRegistry* stageRegistry_ = nullptr;

};

