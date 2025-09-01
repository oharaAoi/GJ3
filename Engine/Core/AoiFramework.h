#pragma once
#include "Engine/Utilities/Logger.h"

/// <summary>
/// ゲーム全体
/// </summary>
class AoiFramework {
public:

	AoiFramework() {};
	virtual ~AoiFramework() {};

	virtual void Init(); 
	virtual void Finalize();
	virtual void Update();
	virtual void Draw() = 0;

	void Run();

	virtual bool isEndRequest() { return endRequest_; }

private:

	bool endRequest_;

	Logger logger_;

};

