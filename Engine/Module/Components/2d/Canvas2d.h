#pragma once
#include <list>
#include <memory>
#include <string>
#include "Engine/Module/Components/2d/Sprite.h"

class Canvas2d {
public:

	struct ObjectPair {
		std::unique_ptr<Sprite> sprite;
		std::string psoName;
		int renderQueue = 0;
	};

public:

	Canvas2d() = default;
	~Canvas2d();

	void Init();

	void Update();

	void Draw() const;

	void EditObject(const ImVec2& windowSize, const ImVec2& imagePos);

public:

	Sprite* AddSprite(const std::string& textureName, const std::string& psoName, int renderQueue = 0);

private:

	std::list<ObjectPair> spriteList_;

};

