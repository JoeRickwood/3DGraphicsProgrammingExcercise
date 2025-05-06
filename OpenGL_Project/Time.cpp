#include "Time.h"
#include <iostream>

void Time::Init()
{
	newT = std::chrono::high_resolution_clock::now();
	oldT = newT;
}

void Time::Update()
{
	oldT = newT;

	newT = std::chrono::high_resolution_clock::now();

	deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(newT - oldT).count() / 1000000.0f;
	time += deltaTime;
}

Time::Time()
{
	time = 0.f;
	deltaTime = 0.f;
}

Time::~Time()
{
}
