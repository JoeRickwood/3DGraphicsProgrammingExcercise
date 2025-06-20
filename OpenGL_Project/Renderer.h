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

enum TilingType 
{
	ClampEdges,
	Repeat,
	ClampBorder
};


struct TexturePass 
{
public:
	std::string locationName;
	std::string textureKey;
	TextureType type;
	TilingType tilingType;

	TexturePass(std::string _location, std::string _texKey, TextureType _type, TilingType _tilingType)
	{
		locationName = _location;

		textureKey = _texKey;
		type = _type;
		tilingType = _tilingType;
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
	bool renderShadows;

	glm::vec2 textureTiling = glm::vec2(1.f, 1.f);

	GLuint VBO;

	std::vector<TexturePass> textures;

	friend class RenderingPipeline;
		
	virtual void InitializeRenderingInfo(GLuint program);

public:
	Renderer(std::string _shaderKey, ProjectionType _projectionType);
	~Renderer();

	void Update() override;

	virtual void InitVBO();
	virtual void BindVBOData();
	virtual void Render();

	void AddTexturePass(std::string _location, std::string _texKey, TextureType _type, TilingType _tilingType);
	void SetMesh(Mesh* _mesh);
	void SetShader(std::string _shaderKey);
	void SetTextureTiling(glm::vec2 _tiling);
	void SetRenderType(RenderType _type);
	void SetShadowRendering(bool _on);
};