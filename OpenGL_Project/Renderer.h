#pragma once
#include "ObjectInstance.h"

class Renderer : public Component
{
protected:
	GLint cameraLoc;
	GLint timeLoc;

	GLint mainTexLoc;
	GLint mainTextureTilingLoc;

public:
	Mesh* mesh; //Points To Renderable Object From The RenderableLoader Static Class
	ShaderType shader;
	int textureID;
	glm::vec2 textureTiling = glm::vec2(1.f, 1.f);
	ProjectionType projection;

	Renderer(int _type = 0, ShaderType _shader = ShaderType::Texture, int _textureID = 0, ProjectionType _projectionType = ProjectionType::Perspective);
	~Renderer();

	virtual void InitializeRenderingInfo();
	Bounds GetWorldBounds();

	void ShaderInit()override;
	void Update() override;
	void Render() override;

};