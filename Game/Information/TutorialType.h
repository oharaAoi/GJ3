#pragma once

enum class TutorialType {
	Self = 0,			// 自己紹介
	Move = 1,			// 移動を教える
	CreateGhost = 2,	// ゴーストを作らせる(ブロックで挟んで)
	Explanation = 3,	// ゴーストが何故できるか説明
	CollisionGhost = 4,	// ゴーストを取らせる
	Goal = 5,			// ゴールさせる
	Finish = 6			// チュートリアル終了
};
