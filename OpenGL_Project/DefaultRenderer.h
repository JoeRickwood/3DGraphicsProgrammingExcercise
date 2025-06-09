#pragma once
#include "Renderer.h"

class DefaultRenderer : public Renderer
{
private:
	glm::mat4 translationMat;
	glm::mat4 rotationMat;
	glm::mat4 scaleMat;
	glm::mat4 modelMat;

public:
	DefaultRenderer(std::string _shaderKey, ProjectionType _projectionType);
	~DefaultRenderer();

	void Init()override;
	void InitVBO()override;
	void Update()override;

	void Render()override;
};

