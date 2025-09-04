#include "BlockFactory.h"
#include "Game/Actor/Block/BlockWall.h"
#include "Game/Actor/Block/BlockGoal.h"
#include "Game/Actor/Block/BlockNormal.h"
#include "Game/Actor/Block/BlockGhost.h"
#include "Game/Actor/Block/TokenGhost.h"
#include "Game/Actor/Block/BlockGrave.h"
#include "Game/Actor/Block/BlockSpecial.h"
#include "Game/Actor/Block/BlockLimit.h"

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
	case BlockType::GraveBlock:
		return std::make_unique<BlockGrave>();
	case BlockType::SpecialBlock:
		return std::make_unique<BlockSpecial>();
	case BlockType::LimitBlock:
		return std::make_unique<BlockLimit>();
	default:
		return nullptr;
	}
}
