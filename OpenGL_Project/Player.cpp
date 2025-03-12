#include "Player.h"

Player::Player()
{
	input = 0.f;
	speed = 1.f;

	isGrounded = false;
	gravity = 0.0f;
}

Player::~Player()
{
}

void Player::Init()
{
	playerRenderer = parent->GetComponent<Renderer>();
	animator = parent->GetComponent<Animator>();


	parent->position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Player::Update()
{
	isGrounded = parent->position.y - 0.55f <= -1.f;

	jumpResetTimer -= Time::Instance().deltaTime;

	input = (Input::Instance().GetKey(GLFW_KEY_D) == true ? 1.f : 0.f) + (Input::Instance().GetKey(GLFW_KEY_A) == true ? -1.f : 0.f);

	if (input < -0.01f) 
	{
		playerRenderer->FlipX(true);
	}
	else if(input > 0.01f)
	{
		playerRenderer->FlipX(false);
	}

	if (abs(input) > 0.01f) 
	{
		animator->SetAnimation(1);
	}
	else 
	{
		animator->SetAnimation(0);
	}

	if (isGrounded && jumpResetTimer <= 0.f) 
	{
		GLFWwindow* window = glfwGetCurrentContext();
		int state = glfwGetKey(window, GLFW_KEY_SPACE);

		if (!jumpLock && state == GLFW_PRESS)
		{
			jumpLock = true;
			jumpResetTimer = 0.2f;
			gravity = 4.f;
		}
		else
		{
			gravity = -1.f;
			jumpLock = false;
		}

	}
	else 
	{
		gravity -= Time::Instance().deltaTime * 9.81f;
	}

	parent->position += glm::vec3(input * speed, gravity, 0.f) * Time::Instance().deltaTime;

	if (parent->position.y - 0.55f < -1.f)
	{
		float difference = -1.f - (parent->position.y - 0.55f);

		parent->position.y += difference;
	}

	std::cout << parent->position.y << std::endl;
}


