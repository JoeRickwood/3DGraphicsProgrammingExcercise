#pragma once
#include "Renderable.h"

class RenderableLoader
{
public:
	static RenderableLoader& Instance() { static RenderableLoader shaderLoader; return shaderLoader; }

	Renderable* renderables = new Renderable[RenderableType::MAX];

	void Init();
	void LinkRenderables();
	Renderable* GetRenderable(RenderableType _type);

private:
	RenderableLoader();
	~RenderableLoader();


};
