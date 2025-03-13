#pragma once
#include <iostream>
#include "windows.h"
#include <string>
#include <vector>
#include<fstream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Time.h"

// Library Includes


class GraphicsLoader
{
public:
	static GraphicsLoader& Instance() { static GraphicsLoader shaderLoader; return shaderLoader; }

	GLuint GetShaderProgram(int _ID);
	GLuint GetTexture(int _ID);
	void InitializeShaderPrograms();
	void InitializeTextures();

	glm::mat4 viewMatrix;

private:
	GraphicsLoader();
	~GraphicsLoader();

	std::vector<GLuint> shaderPrograms;
	std::vector<GLuint> textures;

	static GLuint CreateShaderProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);

	static GLuint CreateTexture(std::string filename);
};


//Bounds Is Used To Check The Points At Which The Object Resides Within
struct Bounds
{
public:
	float minX;
	float minY;
	float maxX;
	float maxY;

	Bounds(float _minX = 0.f, float _minY = 0.f, float _maxX = 0.f, float _maxY = 0.f)
	{
		minX = _minX;
		minY = _minY;
		maxX = _maxY;
		maxY = _maxY;
	}

	//Basic Method To Check If The Bounding Box Contains Point Of x, y
	bool Contains(float x, float y)
	{
		if (x > minX && x < maxX &&
			y > minY && y < maxY)
		{
			return true;
		}

		return false;
	}
};


static float lerp(float a, float b, float t)
{
	return ((b - a) * t) + a;
}


class Texture 
{
public:
	int width;
	int height;
	int components;

	Texture();
	~Texture() {};
};

enum ShaderType
{
	VertexColors = 0,
	Texture = 1
};