#pragma once
#include "Game/Commands/IObjectCommand.h"

#include "Engine/Lib/Math/Vector2Int.h"

class MapCollisionSystem;

/// <summary>
/// プレイヤーがおばけを回収したときに実行するコマンド
/// </summary>
class PlayerGetGhostCommand
	:public IObjectCommand{
public:
	PlayerGetGhostCommand(MapCollisionSystem* _mapCollision,const Vector2Int& _index);
	~PlayerGetGhostCommand() override;
	void Execute() override;
	void Undo() override;

private:
	MapCollisionSystem* mapCollision_;
	Vector2Int index_;
};

