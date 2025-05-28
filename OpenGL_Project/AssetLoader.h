#pragma once
// Library Includes
#include <string>
#include <vector>
#include <map>
#include<fstream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct VertexStandard
{
public:
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;

	VertexStandard()
	{
		position = glm::vec3();
		texcoord = glm::vec2();
		normal = glm::vec3();
	};

	VertexStandard(glm::vec3 _position, glm::vec2 _texcoord, glm::vec3 _normal)
	{
		position = _position;
		texcoord = _texcoord;
		normal = _normal;
	};
};

class Renderer;

class Mesh
{
private: //Renderables Contain The Base Data To Be Drawn to The Screen, Used As Basically A "Stamp" Which Multiple Instances Can Use
	bool initialized;

public:
	std::vector<VertexStandard> data;

	GLuint VAO;

	Mesh();
	Mesh(std::vector<VertexStandard> _data);
	~Mesh();

	void Init();

	friend class Renderer;
};


enum ProjectionType
{
	Orthographic,
	Perspective
};

class AssetLoader
{
public:
	static AssetLoader& Instance() { static AssetLoader shaderLoader; return shaderLoader; }

	GLuint GetShaderProgram(std::string _key);
	GLuint GetTexture(std::string _key);
	GLuint GetSkybox(std::string _key);
	Mesh* GetMesh(std::string _key);

	void LoadAssets(const char* folderPath);

	void InitializeShaderPrograms();
	void InitializeTextures();
	void InitializeMeshes();
	
	glm::vec2 windowSize = glm::vec2(1920, 1080);

	GLFWwindow* currentWindow;

protected:
	AssetLoader();
	~AssetLoader();

	std::map<std::string, GLuint> shaderPrograms;
	std::map<std::string, GLuint> textures;
	std::map<std::string, GLuint> skyboxes;
	std::map<std::string, Mesh*> meshes;


	const std::string supportedImageFileExtensions[2] = { ".png", ".jpg" };
	const std::string supportedModelFileExtensions[1] = { ".obj" };

	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);

	static void CreateShaderProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename, std::string _shaderKey);
	static void CreateTexture(std::string _filename, std::string _shaderKey);
	static void CreateSkybox(std::string _filepaths[6], std::string _skyboxKey);
	static void LoadMesh(std::string _filepath, std::string _meshKey);
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

static glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t) 
{
	return glm::vec3
	(
		lerp(a.x, b.x, t),
		lerp(a.y, b.y, t),
		lerp(a.z, b.z, t)
	);
}