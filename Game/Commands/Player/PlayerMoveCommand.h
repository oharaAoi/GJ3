#pragma once
#include "IPlayerCommand.h"

/// <summary>
/// プレイヤーを左へ移動させるコマンド
/// </summary>
class PlayerLeftMoveCommand :
	public IPlayerCommand{
public:
	PlayerLeftMoveCommand(Player* _host,const Vector2Int& _preIndex)
		:IPlayerCommand(_host),preIndex_(_preIndex){}
	virtual ~PlayerLeftMoveCommand() = default;
	void Execute() override;
	void Undo() override;
private:
	Vector2Int preIndex_;
};

/// <summary>
/// プレイヤーを右へ移動させるコマンド
/// </summary>
class PlayerRightMoveCommand :
	public IPlayerCommand{
public:
	PlayerRightMoveCommand(Player* _host,const Vector2Int& _preIndex)
		:IPlayerCommand(_host),preIndex_(_preIndex){}
	virtual ~PlayerRightMoveCommand() = default;
	void Execute() override;
	void Undo() override;

private:
	Vector2Int preIndex_;

};

/// <summary>
/// プレイヤーを上へ移動させるコマンド
/// </summary>
class PlayerUpMoveCommand :
	public IPlayerCommand{
public:
	PlayerUpMoveCommand(Player* _host,const Vector2Int& _preIndex)
		:IPlayerCommand(_host),preIndex_(_preIndex){}
	virtual ~PlayerUpMoveCommand() = default;
	void Execute() override;
	void Undo() override;
private:
	Vector2Int preIndex_;
};

/// <summary>
/// プレイヤーを下へ移動させるコマンド
/// </summary>
class PlayerDownMoveCommand :
	public IPlayerCommand{
public:
	PlayerDownMoveCommand(Player* _host,const Vector2Int& _preIndex)
		:IPlayerCommand(_host),preIndex_(_preIndex){}
	virtual ~PlayerDownMoveCommand() = default;
	void Execute() override;
	void Undo() override;
private:
	Vector2Int preIndex_;
};

