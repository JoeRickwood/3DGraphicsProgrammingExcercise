#pragma once
#include "RenderableLoader.h"
#include "ObjectInstance.h"

class Renderer : public Component
{
public:
	Renderable* renderable; //Points To Renderable Object From The RenderableLoader Static Class

	Renderer(RenderableType _type = RenderableType::Quad);
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

