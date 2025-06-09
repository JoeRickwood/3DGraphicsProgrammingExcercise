#pragma once
#include "Components.h"

class RenderingPipeline 
{
protected:
	std::vector<Renderer*> renderers;

	RenderingPipeline();
	~RenderingPipeline();

	GLuint depthMapFBO;
	GLuint depthMap;
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

	glm::mat4 lightProjection;
	glm::mat4 lightView;

public:
	static RenderingPipeline& Current() { static RenderingPipeline scene; return scene; }

	static void AddRenderer(Renderer* _renderer);
	static void RemoveRenderer(Renderer* _renderer);

	static void Render();
	static void Render(std::string shaderKeyOverride);

	static glm::mat4 GetLightVPMatrix();
	static void InitShadowRendering();
	static void ShadowPass();
	static GLuint GetShadowMap();
};