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
	DefaultRenderer(int _type = 0, ShaderType _shader = ShaderType::Texture, int _textureID = 0, ProjectionType _projectionType = ProjectionType::Perspective, int _reflectionTexID = -1);
	~DefaultRenderer();

	void InitializeRenderingInfo() override;
	void Update()override;
	void Render()override;
};

