#pragma once

// Library Includes
#include "windows.h"
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <vector>

class ShaderLoader
{
public:
	static ShaderLoader& Instance() { static ShaderLoader shaderLoader; return shaderLoader; }

	GLuint GetShaderProgram(int _ID);
	void InitializeShaderPrograms();

private:
	ShaderLoader();
	~ShaderLoader();

	std::vector<GLuint> shaderPrograms;

	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};
