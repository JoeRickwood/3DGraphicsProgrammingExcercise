#include "TestCamera.h"

TestCamera::TestCamera(float _speed, float _amplitude)
{
	speed = _speed;
	amplitude = _amplitude;

	time = 0.f;
}

TestCamera::~TestCamera()
{
}

void TestCamera::Update()
{
	time += Time::Instance().deltaTime * speed; //Increase Time By The Delta Time Of The Current Frame Multiplied By A Speed

	Camera::Instance().cameraPosition.x = sin(time) * amplitude; //Amplitude Is The Limit Distance Of The Movement, So We Multiply The Sin By It
}
