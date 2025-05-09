#pragma once
// Library Includes
#include <string>
#include <vector>
#include<fstream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


enum ProjectionType
{
	Orthographic,
	Perspective
};

enum ShaderType
{
	Texture = 0,
	Instanced = 1,
	GrassSway = 2,
	Cubemap = 3,
	TextureReflective = 4
};

class GraphicsLoader
{
public:
	static GraphicsLoader& Instance() { static GraphicsLoader shaderLoader; return shaderLoader; }

	GLuint GetShaderProgram(int _ID);
	GLuint GetTexture(int _ID);
	GLuint GetSkybox(int _ID);

	void InitializeShaderPrograms();
	void InitializeTextures();
	
	glm::vec2 windowSize = glm::vec2(1920, 1080);
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLFWwindow* currentWindow;

private:
	GraphicsLoader();
	~GraphicsLoader();

	std::vector<GLuint> shaderPrograms;
	std::vector<GLuint> textures;
	std::vector<GLuint> skyboxes;

	static GLuint CreateShaderProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);

	static GLuint CreateTexture(std::string filename);
	static GLuint CreateSkybox(std::string filepaths[6]);
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
	inline const bool Contains(float x, float y)
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