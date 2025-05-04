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

	deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(newT - oldT).count() / 1000.f;
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
