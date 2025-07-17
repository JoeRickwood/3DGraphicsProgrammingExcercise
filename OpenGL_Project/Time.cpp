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

void Time::BeginTimer(std::string _timerName)
{
	timerT1 = std::chrono::high_resolution_clock::now();
	timerName = _timerName;
}

void Time::EndTimer()
{
	timerT2 = std::chrono::high_resolution_clock::now();

	std::cout << "Time Taken To Complete " << timerName << " : " << (float)std::chrono::duration_cast<std::chrono::microseconds>(timerT2 - timerT1).count() << " Microseconds \n";
}

Time::Time()
{
	time = 0.f;
	deltaTime = 0.f;
}

Time::~Time()
{
}
