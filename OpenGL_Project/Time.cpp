#include "Time.h"

void Time::Update()
{
	oldT = newT;

	newT = std::chrono::high_resolution_clock::now();

	deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(newT - oldT).count() / 1000.f;
}

Time::Time()
{
	deltaTime = 0.f;
}

Time::~Time()
{
}
