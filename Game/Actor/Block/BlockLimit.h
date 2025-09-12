#pragma once
#include <memory>
#include "Game/Actor/Block/IBlock.h"
#include "Game/Effect/LimitBlockEffect.h"
#include "Engine/Module/Components/Animation/VectorTween.h"


class BlockLimit :
	public IBlock {
public:

	BlockLimit() = default;
	~BlockLimit() override;

	void Init(Canvas2d* _canvas2d, const Vector2& _pos, const Vector2& _tileSize) override;
	void Update() override;
	void Debug_Gui() override;

private:

	Vector2 ConvertIndexToPosition(const Vector2Int& _index);

	std::shared_ptr<LimitBlockEffect> effect_;
};

