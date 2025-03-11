#pragma once
#include "ObjectInstance.h"

class Renderer : public Component
{
public:
	Renderable* renderable; //Points To Renderable Object From The RenderableLoader Static Class
	ShaderType shader;


	Renderer(RenderableType _type = RenderableType::Quad, ShaderType _shader = ShaderType::VertexColors);
	~Renderer();

	void Render() override;
	void Update() override;

	Bounds GetWorldBounds();


private:
	glm::mat4 translationMat;
	glm::mat4 rotationMat;
	glm::mat4 scaleMat;
	glm::mat4 modelMat;
};

