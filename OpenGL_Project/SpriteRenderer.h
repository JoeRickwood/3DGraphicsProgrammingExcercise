#pragma once
#include "Renderer.h"

class SpriteRenderer : public Renderer
{
private:
	glm::mat4 translationMat;
	glm::mat4 rotationMat;
	glm::mat4 scaleMat;
	glm::mat4 modelMat;

public:
	SpriteRenderer(std::string _shaderKey, ProjectionType _projectionType);
	~SpriteRenderer();

	void Init()override;
	void InitVBO()override;
	void BindVBOData()override;
	void Update()override;

	void Render()override;
};

