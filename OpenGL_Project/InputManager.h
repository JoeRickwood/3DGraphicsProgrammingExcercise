#pragma once
#include <glew.h>
#include <glfw3.h>

class InputManager
{
public:
	static InputManager& Instance() { static InputManager shaderLoader; return shaderLoader; }

	void Init();

	bool GetKey(int _keyCheck);
	//bool GetKeyDown();
	//bool GetKeyUp();

	bool* keystates = new bool[GLFW_KEY_LAST];

private:
	InputManager();
	~InputManager();

	static void KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
};

