#pragma once
#include "ObjectInstance.h"

enum TextureType 
{
	Texture2D = GL_TEXTURE_2D,
	CubeMap = GL_TEXTURE_CUBE_MAP
};


struct TexturePass 
{
public:
	std::string locationName;
	
	int texID;
	TextureType type;

	TexturePass(std::string _location, int _texID, TextureType _type)
	{
		locationName = _location;

		texID = _texID;
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
	ShaderType shader;
	ProjectionType projection;

	glm::vec2 textureTiling = glm::vec2(1.f, 1.f);

	std::vector<TexturePass> textures;
		
public:
	Renderer(ShaderType _shader = ShaderType::Texture, ProjectionType _projectionType = ProjectionType::Perspective);
	~Renderer();

	virtual void InitializeRenderingInfo();

	void Update() override;
	void Render() override;

	void AddTexture(std::string _location, int _texID, TextureType _type);
	void SetMesh(Mesh* _mesh);
	void SetShader(ShaderType _shader);
	void SetTextureTiling(glm::vec2 _tiling);
};