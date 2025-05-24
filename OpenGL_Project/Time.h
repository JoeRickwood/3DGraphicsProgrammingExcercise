#pragma once
#include <chrono>
#include <string>

class Time
{
public:
	static Time& Instance() { static Time time; return time; }

	float deltaTime;
	float time;

	void Init();
	void Update();

	void BeginTimer(std::string _timerName);
	void EndTimer();

private:
	Time();
	~Time();

	std::chrono::steady_clock::time_point oldT;
	std::chrono::steady_clock::time_point newT;


	std::chrono::steady_clock::time_point timerT1;
	std::chrono::steady_clock::time_point timerT2;
	std::string timerName;
};

