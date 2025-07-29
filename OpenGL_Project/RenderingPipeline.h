#pragma once
#include "Components.h"

class RenderingPipeline 
{
protected:
	std::map<int, std::vector<Renderer*>> renderers;

	RenderingPipeline();
	~RenderingPipeline();

	GLuint depthMapFBO;
	GLuint depthMap;
	const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;

public:
	static RenderingPipeline& Current() { static RenderingPipeline scene; return scene; }

	static void AddRenderer(Renderer* _renderer);
	static void RemoveRenderer(Renderer* _renderer);

	static void Render();
	static void Render(std::string shaderKeyOverride);

	static void ShadowPass();

	std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
	static glm::mat4 GetLightVPMatrix();

	static void InitShadowRendering();
	static GLuint GetShadowMap();


	void ClearAllRenderers();
};