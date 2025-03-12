#pragma once
#include "GraphicsLoader.h"

class Input
{
public:
	static Input& Instance() { static Input input; return input; }

	bool GetKey(int key);
	void Init();

private:
	Input();
	~Input();

	bool* keystates;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

