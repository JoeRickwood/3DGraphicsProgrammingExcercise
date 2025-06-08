#include "RenderingPipeline.h"
#include "Camera.h"
#include "Scene.h"

RenderingPipeline::RenderingPipeline()
{
}

RenderingPipeline::~RenderingPipeline()
{
}

void RenderingPipeline::AddRenderer(Renderer* _renderer)
{
	Current().renderers.push_back(_renderer);
}

void RenderingPipeline::RemoveRenderer(Renderer* _renderer)
{
	for (int i = 0; i < Current().renderers.size(); i++)
	{
		if (Current().renderers[i] == _renderer)
		{
			Current().renderers.erase(Current().renderers.begin() + i);
			return;
		}
	}
}

void RenderingPipeline::Render()
{
	glViewport(0, 0, (GLsizei)AssetLoader::Instance().windowSize.x, (GLsizei)AssetLoader::Instance().windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < Current().renderers.size(); ++i)
	{
		Current().renderers[i]->Render();
	}

	glfwSwapBuffers(AssetLoader::Instance().currentWindow);
}

void RenderingPipeline::Render(std::string shaderKeyOverride)
{
	glViewport(0, 0, (GLsizei)AssetLoader::Instance().windowSize.x, (GLsizei)AssetLoader::Instance().windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < Current().renderers.size(); ++i)
	{
		Current().renderers[i]->Render(shaderKeyOverride);
	}

	glfwSwapBuffers(AssetLoader::Instance().currentWindow);
}

glm::mat4 RenderingPipeline::GetLightVPMatrix()
{
	Current().lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, Camera::Instance().nearPlane, Camera::Instance().farPlane);

	Current().lightView = glm::lookAt(Camera::Instance().cameraPosition + (Scene::Current().GetDirectionalLight()->direction * -10.f),
		Camera::Instance().cameraPosition,
		glm::vec3(0.0f, 1.0f, 0.0f));

	return Current().lightProjection * Current().lightView;
}

void RenderingPipeline::InitShadowRendering()
{
	glGenFramebuffers(1, &Current().depthMapFBO);

	glGenTextures(1, &Current().depthMap);
	glBindTexture(GL_TEXTURE_2D, Current().depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Current().SHADOW_WIDTH, Current().SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, Current().depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Current().depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	AssetLoader::Instance().AddTexture(Current().depthMap, "DepthMap");
}

void RenderingPipeline::ShadowPass()
{
	glViewport(0, 0, Current().SHADOW_WIDTH, Current().SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, Current().depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < Current().renderers.size(); ++i)
	{
		Current().renderers[i]->Render("DepthTesting");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint RenderingPipeline::GetShadowMap()
{
	return Current().depthMap;
}
