#include "Tests.h"

Tests::Tests(float _speed, float _offset, float _rotationSpeed)
{
	speed = _speed;
	offset = _offset;
	rotationSpeed = _rotationSpeed;
}

Tests::~Tests()
{
}

void Tests::Update()
{
	time += (1.f / 60.f) * speed;

	rot += (1.f / 60.f) * rotationSpeed;
	x = sin(time) * offset;
	y = cos(time) * offset;

	parent->position = glm::vec3(x, y, 0.f);
	parent->rotation = glm::vec3(0.f, 0.f, rot);

	parent->scale = glm::vec3(1.f) * lerp(0.25f, 0.75f, (sin(time) + 1.f) / 2.f); //Assuming 0.5f Is Default Scale, 0.25 Is 50% and 0.75 is 150%
}

void Tests::ShaderUpdate()
{
	GLint CurrentTimeLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(0), "CurrentTime");
	glUniform1f(CurrentTimeLoc, time);
}
