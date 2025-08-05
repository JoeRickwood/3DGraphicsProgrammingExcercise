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

#include <ft2build.h>
#include FT_FREETYPE_H

struct TextCharacter 
{
public:
	GLuint textureID;  // ID handle of the glyph texture
	glm::ivec2   size;       // Size of glyph
	glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
	unsigned int advanceOffset;    // Offset to advance to next glyph

	TextCharacter(GLuint _textureID = NULL, glm::ivec2 _size = glm::ivec2(0, 0), glm::ivec2 _bearing = glm::ivec2(0, 0), unsigned int _advanceOffset = 0)
	{
		textureID = _textureID;
		size = _size;
		bearing = _bearing;
		advanceOffset = _advanceOffset;
	}
};

struct Font 
{
private:
	std::map<char, TextCharacter> fontGlyphs;

public:
	void AddGlyph(TextCharacter _textCharacter, char _key) 
	{
		fontGlyphs.emplace(_key, _textCharacter);
	}

	TextCharacter GetGlyph(char _key) 
	{
		return fontGlyphs[_key];
	}

	bool GlyphExists(char _key) 
	{
		return fontGlyphs.find(_key) != fontGlyphs.end();
	}
};

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
	ORTHOGRAPHIC,
	PERSPECTIVE,
	SCREEN_ORTHOGRAPHIC
};

class AssetLoader
{
public:
	static AssetLoader& Instance() { static AssetLoader shaderLoader; return shaderLoader; }

	GLuint GetShaderProgram(std::string _key);
	GLuint GetTexture(std::string _key);
	GLuint GetSkybox(std::string _key);
	Mesh* GetMesh(std::string _key);
	Font* GetFont(std::string _fontKey);
	TextCharacter GetGlyph(std::string _fontKey, const char _glyph);
	void AddTexture(GLuint _tex, std::string _key);

	static void CreateSkybox(std::string _filepaths[6], std::string _skyboxKey);
	static Mesh* CreateMesh(std::vector<glm::vec3> _positions, std::vector<int> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> _texcoords);

	void LoadAssets(const char* folderPath);

	glm::vec2 GetWindowSize();
	void SetWindowSize(glm::vec2 _windowSize);

	GLFWwindow* currentWindow;

protected:
	AssetLoader();
	~AssetLoader();

	std::map<std::string, GLuint> shaderPrograms;
	std::map<std::string, GLuint> textures;
	std::map<std::string, GLuint> skyboxes;
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Font*> fonts;

	const std::string supportedImageFileExtensions[2] = { ".png", ".jpg" };
	const std::string supportedModelFileExtensions[1] = { ".obj" };
	const std::string supportedShaderFileExtensions[1] = { ".shader" };
	const std::string supportedFontFileExtensions[1] = { ".ttf" };

	glm::vec2 windowSize = glm::vec2(1920, 1080);

	//Helpers
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);

	//Used For Asset Loading Indirectly
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);

	//Asset Loading + Creation From Filepaths
	static void CreateShaderProgram(const char* _filename, std::string _shaderKey);
	static void CreateTexture(std::string _filename, std::string _shaderKey);
	static void LoadMesh(std::string _filepath, std::string _meshKey);
	static void LoadFont(std::string _filepath, std::string _fontKey);
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
	inline bool Contains(float x, float y) const
	{
		if (x > minX && x < maxX &&
			y > minY && y < maxY)
		{
			return true;
		}

		return false;
	}
};