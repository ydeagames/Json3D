#pragma once

class GameContext;

// �R���|�[�l���g
class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

public:
	// ����
	virtual void Initialize(GameContext& context) {};
	// �X�V
	virtual void Update(GameContext& context) {};
	// �`��
	virtual void Render(GameContext& context) {};
	// �j��
	virtual void Finalize(GameContext& context) {};
};
