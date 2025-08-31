#pragma once
#include "Components.h"

class RenderingPipeline 
{
protected:
	std::map<int, std::vector<Renderer*>> renderers;

	Renderer* screenQuad;

	RenderingPipeline();
	~RenderingPipeline();


	//Shadow Rendering
	GLuint depthMapFBO;
	GLuint depthMap;
	const unsigned int SHADOW_WIDTH = 4096*2, SHADOW_HEIGHT = 4096*2;

	//Frame Buffer Rendering
	GLuint framebufferFBO;
	GLuint framebufferTexture;
	GLuint depthRenderBuffer;

public:
	static RenderingPipeline& Current() { static RenderingPipeline scene; return scene; }

	static void AddRenderer(Renderer* _renderer);
	static void RemoveRenderer(Renderer* _renderer);

	static void Render();
	static void Render(std::string shaderKeyOverride);

	static void ShadowPass();
	static void FrameBufferPass();
	static void RenderToScreen();

	std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
	static glm::mat4 GetLightVPMatrix();

	static void InitShadowRendering();
	static GLuint GetShadowMap();

	static Renderer* GetScreenQuadRenderer();
	static void SetScreenQuadRenderer(Renderer* _renderer);

	//Initialize Frame Buffers At Runtime Start
	static void InitFrameBuffer();
	static GLuint GetFramebufferTexture();

	static void UpdateFramebufferTexture();

	void ClearAllRenderers();
};