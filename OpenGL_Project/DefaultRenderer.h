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
	DefaultRenderer(ShaderType _shader, ProjectionType _projectionType);
	~DefaultRenderer();

	void InitializeRenderingInfo() override;
	void Update()override;
	void Render()override;
};

