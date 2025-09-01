#pragma once
#include <memory>
#include "Game/Actor/Block/IBlock.h"

std::unique_ptr<IBlock> CreateBlock(uint32_t _type);

