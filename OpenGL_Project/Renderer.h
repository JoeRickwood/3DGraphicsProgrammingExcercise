#pragma once
#include "ObjectInstance.h"

struct Frame
{
	glm::vec2 bottomLeft;
	glm::vec2 topRight;

	Frame(glm::vec2 _bottomleft = glm::vec2(0.f, 0.0f), glm::vec2 _topRight = glm::vec2(1.0f, 1.0f))
	{
		bottomLeft = _bottomleft;
		topRight = _topRight;
	}

	~Frame()
	{

	}

	void FlipX();
	void FlipY();
};

class Renderer : public Component
{
public:
	Renderable* renderable; //Points To Renderable Object From The RenderableLoader Static Class
	ShaderType shader;
	int textureID;
	Frame uvFrame;
	ProjectionType projection;


	Renderer(RenderableType _type = RenderableType::Quad, ShaderType _shader = ShaderType::VertexColors, int _textureID = 0, ProjectionType _projectionType = ProjectionType::Perspective);
	~Renderer();

	void Render() override;
	void Update() override;

	void SetUVFrame(Frame frame);
	void FlipX(bool state);
	void FlipY(bool state);

	Bounds GetWorldBounds();


private:
	glm::mat4 translationMat;
	glm::mat4 rotationMat;
	glm::mat4 scaleMat;
	glm::mat4 modelMat;

	bool flipX = false;
	bool flipY = false;
};