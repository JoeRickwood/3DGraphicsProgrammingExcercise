#pragma once
#include "Animator.h"

class Player : public Component 
{
public:
	Player();
	~Player();

	void Init()override;
	void Update()override;

	float speed = 1.f;
	float input = 0.f;


private:
	Renderer* playerRenderer;
	Animator* animator;

	float gravity;
	bool isGrounded;
	bool jumpLock;
	float jumpResetTimer;
};