#include "BlockFactory.h"
#include "Game/Actor/Block/BlockWall.h"
#include "Game/Actor/Block/BlockGoal.h"
#include "Game/Actor/Block/BlockNormal.h"
#include "Game/Actor/Block/BlockGhost.h"
#include "Game/Actor/Block/TokenGhost.h"

std::unique_ptr<IBlock> CreateBlock(uint32_t _type) {
	switch (static_cast<BlockType>(_type)) {
	case BlockType::None:
		return nullptr;
	case BlockType::Player:
		return nullptr;
	case BlockType::Goal:
		return std::make_unique<BlockGoal>();
	case BlockType::Wall:
		return std::make_unique<BlockWall>();
	case BlockType::NormalBlock:
		return std::make_unique<BlockNormal>();
	case BlockType::GhostBlock:
		return std::make_unique<BlockGhost>();
	case BlockType::Ghost:
		return std::make_unique<TokenGhost>();
	default:
		return nullptr;
	}
}
