#pragma once

/// <summary>
/// コマンド基底クラス これを継承して オブジェクト(Player,Stage)の動きを作る
/// </summary>
class IObjectCommand{
public:
	IObjectCommand() = default;
	virtual ~IObjectCommand() = default;

	virtual void Execute() = 0;
	virtual void Undo() = 0;
};

