#pragma once
#include "Renderer.h"

class ScreenQuadRenderer : public Renderer
{
private:
	glm::mat4 translationMat;
	glm::mat4 rotationMat;
	glm::mat4 scaleMat;
	glm::mat4 modelMat;

public:
	ScreenQuadRenderer(std::string _shaderKey, ProjectionType _projectionType);
	~ScreenQuadRenderer();

	void Init()override;
	void InitVBO()override;
	void BindVBOData()override;
	void Update()override;

	void Render()override;
};

