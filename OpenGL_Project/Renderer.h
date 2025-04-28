#pragma once
#include "ObjectInstance.h"

class Renderer : public Component
{
public:
	Mesh* mesh; //Points To Renderable Object From The RenderableLoader Static Class
	ShaderType shader;

	int textureID;
	ProjectionType projection;

	Renderer(int _type = 0, ShaderType _shader = ShaderType::Texture, int _textureID = 0, ProjectionType _projectionType = ProjectionType::Perspective);
	~Renderer();

	void InitializeRenderingInfo();

	void Render() override;
	void Update() override;

	Bounds GetWorldBounds();


private:
	glm::mat4 translationMat;
	glm::mat4 rotationMat;
	glm::mat4 scaleMat;
	glm::mat4 modelMat;
};