#pragma once

/// base
#include "Game/Commands/IObjectCommand.h"

/// actor
#include "Game/Actor/Player/Player.h"

class Player;
class IPlayerCommand
	:public IObjectCommand{
public:
	IPlayerCommand(Player* _host = nullptr):host_(_host){}
	virtual ~IPlayerCommand() = default;

	virtual void Execute() = 0;
	virtual void Undo() = 0;
protected:
	Player* host_ = nullptr;
public:
	Player* GetHost()const{ return host_; }
	void SetHost(Player* _host){ host_ = _host; }
};

