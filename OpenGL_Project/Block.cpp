#include "Block.h"
#include "Collider.h"

Block::Block()
{
	health = 100.f;
}

Block::~Block()
{
}

void Block::AttackBlock(float amount)
{
	health -= amount;

	if (health <= 0.f) 
	{
		BreakBlock();
	}
}


void Block::BreakBlock()
{
	delete parent;
}
