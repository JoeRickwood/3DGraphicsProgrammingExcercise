#pragma once
#include "Renderer.h"

class TextRenderer : public Renderer
{
protected:
	glm::mat4 translationMat;
	glm::mat4 rotationMat;
	glm::mat4 scaleMat;
	glm::mat4 modelMat;

	std::string text;
	std::string fontKey;

public:
	TextRenderer(std::string _shaderKey, ProjectionType _projectionType);
	~TextRenderer();

	void Init()override;
	void InitVBO()override;
	void BindVBOData()override;
	void Update()override;

	void InitializeRenderingInfo(GLuint _program)override;
	void Render()override;

	void SetText(std::string _content);
	std::string GetText();

	void SetFont(std::string _fontKey);
	std::string GetFontKey();
};
