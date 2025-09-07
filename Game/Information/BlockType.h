#pragma once

enum class BlockType {
	None			= 0,		// 無し
	Player			= 1,		// プレイヤー
	Goal			= 2,		// ゴール
	Wall			= 3,		// 壁(不動ブロック)
	NormalBlock		= 4,		// 通常ブロック
	GhostBlock		= 5,		// おばけブロック
	Ghost			= 6,		// おばけ
	GraveBlock		= 7,		// お墓ブロック
	SpecialBlock	= 8,		// 特殊ブロック
	LimitBlock		= 9,		// 制限ブロック
};