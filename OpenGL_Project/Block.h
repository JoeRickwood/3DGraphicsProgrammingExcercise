#pragma once

#include "ObjectInstance.h"

class Block : public Component
{
public:
	Block();
	~Block();

	void AttackBlock(float amount);

	float health;
	void BreakBlock();
};