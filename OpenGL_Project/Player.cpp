#include "Player.h"

Player::Player()
{
	input = 0.f;
	speed = 1.f;

	isGrounded = false;

	jumpLock = false;
	jumpResetTimer = 0.f;

	playerRenderer = nullptr;
	animator = nullptr;
	physicsObject = nullptr;
}

Player::~Player()
{
}

void Player::Init()
{
	playerRenderer = parent->GetComponent<Renderer>();
	animator = parent->GetComponent<Animator>();
	physicsObject = parent->GetComponent<PhysicsObject>();
}

void Player::Update()
{
	isGrounded = Physics::Instance().CheckPoint(glm::vec3(parent->position.x, physicsObject->collider->GetRect()->bottom - 0.025f, 0.0f));

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
			physicsObject->velocity.y = 4.f;
		}
		else
		{
			physicsObject->velocity.y = -1.f;
			jumpLock = false;
		}

	}
	else 
	{
		physicsObject->velocity.y -= Time::Instance().deltaTime * 9.81f;
	}

	physicsObject->velocity.x = input * speed;

	GraphicsLoader::Instance().viewMatrix = glm::translate(glm::mat4(1.0f), parent->position * -1.f);

	//std::cout << parent->position.y;
}


