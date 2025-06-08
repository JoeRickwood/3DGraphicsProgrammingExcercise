#pragma once
#include "ObjectInstance.h"

enum TextureType 
{
	Texture2D = GL_TEXTURE_2D,
	CubeMap = GL_TEXTURE_CUBE_MAP
};

enum RenderType 
{
	RenderFront = GL_BACK,
	RenderBack = GL_FRONT,
	RenderNone = GL_FRONT_AND_BACK,
	RenderBoth = GL_CULL_FACE
};


struct TexturePass 
{
public:
	std::string locationName;
	std::string textureKey;
	TextureType type;

	TexturePass(std::string _location, std::string _texKey, TextureType _type)
	{
		locationName = _location;

		textureKey = _texKey;
		type = _type;
	}

	~TexturePass() 
	{

	}
};

class Renderer : public Component
{
protected:
	Mesh* mesh; 	 //Points To Renderable Object From The MeshLoader Static Class
	std::string shaderKey;
	ProjectionType projection;
	RenderType renderType;
	bool doubleSided;

	glm::vec2 textureTiling = glm::vec2(1.f, 1.f);

	GLuint VBO;

	std::vector<TexturePass> textures;
		
	virtual void InitializeRenderingInfo(GLuint program);

public:
	Renderer(std::string _shaderKey, ProjectionType _projectionType);
	~Renderer();

	void Update() override;

	virtual void InitVBO();
	virtual void Render();
	virtual void Render(std::string _shaderKeyOverride);

	void AddTexturePass(std::string _location, std::string _texKey, TextureType _type);
	void SetMesh(Mesh* _mesh);
	void SetShader(std::string _shaderKey);
	void SetTextureTiling(glm::vec2 _tiling);
	void SetRenderType(RenderType _type);
};