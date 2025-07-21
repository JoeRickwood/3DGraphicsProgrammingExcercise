#pragma once
#include "Components.h"

class RenderingPipeline 
{
protected:
	std::vector<Renderer*> renderers;

	RenderingPipeline();
	~RenderingPipeline();

public:
	static RenderingPipeline& Current() { static RenderingPipeline scene; return scene; }

	static void AddRenderer(Renderer* _renderer);
	static void RemoveRenderer(Renderer* _renderer);

	static void Render();
	static void Render(std::string shaderKeyOverride);
};