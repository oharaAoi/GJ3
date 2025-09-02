#include "BlockFactory.h"
#include "Game/Actor/Block/BlockWall.h"

std::unique_ptr<IBlock> CreateBlock(uint32_t _type) {
	switch (static_cast<BlockType>(_type)) {
	case BlockType::None:
		return nullptr;
	case BlockType::Player:
		return nullptr;
		break;
	case BlockType::Goal:
		return nullptr;
		break;
	case BlockType::Wall:
		return std::make_unique<BlockWall>();
	default:
		return nullptr;
	}
}
