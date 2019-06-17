#pragma once
#include <Framework/ISceneBuilder.h>

// ゲームロジック
class BaseScene : public ISceneBuilder
{
public:
	static int mode1p;
	static int mode2p;

public:
	// 生成
	void Build(GameContext& context);
};

