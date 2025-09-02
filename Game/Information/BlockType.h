#pragma once

enum class BlockType {
	None		= 0,		// 無し
	Player		= 1,		// プレイヤー
	Goal		= 2,		// ゴール
	Wall		= 3,		// 壁(不動ブロック)
	NormalBlock	= 4,		// 通常ブロック(変化する系)
	GhostBlock	= 5,		// おばけブロック
};