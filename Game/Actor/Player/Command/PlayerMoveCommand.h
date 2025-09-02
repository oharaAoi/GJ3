#pragma once
#include "IPlayerCommand.h"

/// <summary>
/// プレイヤーを左へ移動させるコマンド
/// </summary>
class PlayerLeftMoveCommand :
	public IPlayerCommand{
public:
	PlayerLeftMoveCommand(Player* _host)
		:IPlayerCommand(_host){}
	virtual ~PlayerLeftMoveCommand() = default;
	void Execute() override;
	void Undo() override;
};

/// <summary>
/// プレイヤーを右へ移動させるコマンド
/// </summary>
class PlayerRightMoveCommand :
	public IPlayerCommand{
public:
	PlayerRightMoveCommand(Player* _host)
		:IPlayerCommand(_host){}
	virtual ~PlayerRightMoveCommand() = default;
	void Execute() override;
	void Undo() override;
};

/// <summary>
/// プレイヤーを上へ移動させるコマンド
/// </summary>
class PlayerUpMoveCommand :
	public IPlayerCommand{
public:
	PlayerUpMoveCommand(Player* _host)
		:IPlayerCommand(_host){}
	virtual ~PlayerUpMoveCommand() = default;
	void Execute() override;
	void Undo() override;
};

/// <summary>
/// プレイヤーを下へ移動させるコマンド
/// </summary>
class PlayerDownMoveCommand :
	public IPlayerCommand{
public:
	PlayerDownMoveCommand(Player* _host)
		:IPlayerCommand(_host){}
	virtual ~PlayerDownMoveCommand() = default;
	void Execute() override;
	void Undo() override;
};

