#pragma once
#include "Renderer.h"

class Animation
{
public:
	Animation(int _textureID, int _columns, int _rows, int _animLength);
	~Animation();

	int textureID;
	int columns;
	int rows;
	int length;
};

class Animator : public Component
{
public:
	Animator(float _speed);
	~Animator();

	void Init()override;
	void Update()override;

	void AddAnimation(Animation anim);

	void SetAnimation(int index);

	void UpdateUVFrame();

	float speed;


private:
	std::vector<Animation> animations;
	Animation* currentAnimation;
	Renderer* renderer;

	float t;
	int currentFrame;
};