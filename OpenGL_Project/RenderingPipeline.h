#pragma once
#include "Components.h"

class RenderingPipeline 
{
protected:
	std::vector<Renderer*> renderers;

	RenderingPipeline();
	~RenderingPipeline();

	GLuint shadowmapFBO;
	GLuint shadowmapTexture;

	float shadowMapHeight = 2048, shadowMapWidth = 2048;


public:
	static RenderingPipeline& Current() { static RenderingPipeline renderingPipeline; return renderingPipeline; }
	
	static void InitializeShadowMapping();

	static void AddRenderer(Renderer* _renderer);
	static void RemoveRenderer(Renderer* _renderer);

	static void ShadowPass();

	static void Render();
	static void Render(std::string shaderKeyOverride);
};