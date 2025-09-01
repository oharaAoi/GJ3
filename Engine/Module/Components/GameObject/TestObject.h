#pragma once
#include <memory>
#include "Engine/Module/Components/GameObject/BaseEntity.h"

class TestObject
	: public BaseEntity {
public:

	TestObject();
	~TestObject();

	void Finalize();
	void Init();
	void Update();
	
	void Debug_Gui() override;

private:

	Matrix4x4 swordMat_;

};

