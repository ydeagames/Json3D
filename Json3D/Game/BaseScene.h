#pragma once
#include <Framework/ISceneBuilder.h>

// �Q�[�����W�b�N
class BaseScene : public ISceneBuilder
{
public:
	static int mode1p;
	static int mode2p;

public:
	// ����
	void Build(GameContext& context);
};

