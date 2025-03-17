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
enum ProjectionType
{
	Orthographic,
	Perspective
};

class GraphicsLoader
{
public:
	static GraphicsLoader& Instance() { static GraphicsLoader shaderLoader; return shaderLoader; }

	GLuint GetShaderProgram(int _ID);
	GLuint GetTexture(int _ID);
	void InitializeShaderPrograms();
	void InitializeTextures();

	static void CalculateViewMatrix();
	static void CalculateProjectionMatrix();

	glm::mat4 GetProjectionMatrix(ProjectionType _type);

	//Camera Stuff
	glm::mat4 viewMatrix;

	glm::vec3 cameraPosition = glm::vec3(3.0f, 6.0f, 5.0f);
	glm::vec3 cameraLookDir = glm::vec3(1.0f, 0.0f, -1.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

	float fieldOfView = 45.f;
	
	glm::vec2 windowSize = glm::vec2(800, 800);

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

	glm::mat4 orthoProjectionMatrix;
	glm::mat4 perspectiveProjectionMatrix;
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