#pragma once
#include "ObjectInstance.h"

class Renderer : public Component
{
protected:
	Mesh* mesh; 			//Points To Renderable Object From The MeshLoader Static Class
	Material* material;		//Points To The Material Object Being Used By This Renderer 
	
	GLuint VBO;				//VBO Data To Hotswap When Rendering Different Kinds, Allows For Instanced, And Non-Instanced Objects And Text To Be Rendered The Same

	std::string shaderKey;
	ProjectionType projection;
	RenderType renderType;

	bool doubleSided;
	bool renderShadows;

	glm::vec4 color;

	glm::vec2 textureTiling = glm::vec2(1.0f, 1.0f);

	//UI ONLY CONVERT TO SHADER PASS INS OR MATERIAL LATER
	float borderSize = 50;
	glm::vec2 textureSize = glm::vec2(64, 64);

	std::vector<TexturePass> textures;

	friend class RenderingPipeline;
		
	virtual void InitializeRenderingInfo(GLuint _program);

public:
	Renderer(std::string _shaderKey, ProjectionType _projectionType);
	~Renderer();

	void Update() override;

	virtual void InitVBO();
	virtual void BindVBOData();
	virtual void Render();

	void AddTexturePass(std::string _location, std::string _texKey, TextureType _type, TextureTilingType _tilingType);

	void SetMesh(Mesh* _mesh);
	void SetShader(std::string _shaderKey);
	void SetTextureTiling(glm::vec2 _tiling);
	void SetRenderType(RenderType _type);
	void SetShadowRendering(bool _on);

	glm::vec4 GetColor() const;
	void SetColor(glm::vec4 _color);

	void SetMaterial(Material* _material);
	Material* GetMaterial();

};