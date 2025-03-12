#pragma once
#include <chrono>

class Time
{
public:
	static Time& Instance() { static Time time; return time; }

	float deltaTime;

	void Update();
private:
	Time();
	~Time();

	std::chrono::steady_clock::time_point oldT;
	std::chrono::steady_clock::time_point newT;
};

