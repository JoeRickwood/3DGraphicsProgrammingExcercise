#include "Animator.h"

Animation::Animation(int _textureID, int _columns, int _rows, int _animLength)
{
	textureID = _textureID;
	columns = _columns;
	rows = _rows;
	length = _animLength;
}

Animation::~Animation()
{
}

Animator::Animator(float _speed)
{
	currentAnimation = nullptr;

	speed = _speed;
	t = 0.f;

	currentFrame = 0;

	renderer = nullptr;
}

Animator::~Animator()
{
}

void Animator::Init()
{
	renderer = parent->GetComponent<Renderer>();
}

void Animator::Update()
{
	if (currentAnimation == nullptr) 
	{
		return;
	}

	t -= Time::Instance().deltaTime * speed;

	if (t <= 0.f) 
	{
		currentFrame++;

		if (currentFrame >= currentAnimation->length) 
		{
			currentFrame = 0;
		}

		UpdateUVFrame();

		t = 1.f / speed;
	}	
}

void Animator::AddAnimation(Animation anim)
{
	animations.push_back(anim);

	currentAnimation = &animations[0];
}

void Animator::SetAnimation(int index)
{
	if (animations.size() <= index) 
	{
		return;
	}

	currentAnimation = &animations[index];
}

void Animator::UpdateUVFrame()
{
	//Set UV Frame
	int difference = currentFrame % currentAnimation->columns;

	int positionX = difference;
	int positionY = (currentFrame - difference) / currentAnimation->columns;

	float iterationX = 1.f / currentAnimation->columns;
	float iterationY = 1.f / currentAnimation->rows;

	float xOffset = iterationX * positionX;
	float yOffset = iterationY * positionY;

	glm::vec2 bottomLeft = glm::vec2(xOffset, yOffset);
	glm::vec2 topRight = glm::vec2((iterationX + xOffset), iterationY + yOffset);

	Frame frame = Frame(bottomLeft, topRight);

	renderer->SetUVFrame(frame);
}
