#include "RenderingPipeline.h"
#include "Camera.h"
#include <iostream>
#include "Scene.h"

RenderingPipeline::RenderingPipeline()
{
	shadowmapFBO = NULL;
	shadowmapTexture = NULL;
}

RenderingPipeline::~RenderingPipeline()
{
}

void RenderingPipeline::InitializeShadowMapping()
{
	//Generate Frame Buffer + Depth Map Textures
	glGenFramebuffers(1, &Current().shadowmapFBO);

	glGenTextures(1, &Current().shadowmapTexture);
	glBindTexture(GL_TEXTURE_2D, Current().shadowmapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)Current().shadowMapWidth, (GLsizei)Current().shadowMapHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, Current().shadowmapFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Current().shadowmapTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, NULL, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	AssetLoader::Instance().AddTexture(Current().shadowmapTexture, "ShadowMap");
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

void RenderingPipeline::ShadowPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, Current().shadowmapFBO);

	glViewport(0, 0, (GLsizei)Current().shadowMapWidth, (GLsizei)Current().shadowMapHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint program = AssetLoader::Instance().GetShaderProgram("ShadowPass");
	glUseProgram(program);

	glm::mat4 VP = Camera::Instance().GetProjectionMatrix(ORTHOGRAPHIC) * Camera::Instance().GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "VP"), 1, GL_FALSE, glm::value_ptr(VP));

	for (int i = 0; i < Current().renderers.size(); ++i)
	{
		if (!Current().renderers[i]->renderShadows)
		{
			continue;
		}

		Current().renderers[i]->BindVBOData();

		//Current().renderers[i]->InitializeRenderingInfo(program);

		Current().renderers[i]->Render();
	}

	glUseProgram(0);

	//Bind The Current Frame Buffer And The Shader Program Back To Defaults
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingPipeline::Render()
{
	glm::vec2 windowSize = AssetLoader::Instance().GetWindowSize();
	glViewport(0, 0, (GLsizei)windowSize.x, (GLsizei)windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < Current().renderers.size(); ++i)
	{
		GLuint program = AssetLoader::Instance().GetShaderProgram(Current().renderers[i]->shaderKey);
		glUseProgram(program);

		Current().renderers[i]->BindVBOData();

		Current().renderers[i]->InitializeRenderingInfo(program);

		Current().renderers[i]->Render();

		glUseProgram(0);
	}

	glfwSwapBuffers(AssetLoader::Instance().currentWindow);
}

void RenderingPipeline::Render(std::string shaderKeyOverride)
{
	glm::vec2 windowSize = AssetLoader::Instance().GetWindowSize();
	glViewport(0, 0, (GLsizei)windowSize.x, (GLsizei)windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < Current().renderers.size(); ++i)
	{
		GLuint program = AssetLoader::Instance().GetShaderProgram(shaderKeyOverride);
		glUseProgram(program);

		Current().renderers[i]->BindVBOData();

		Current().renderers[i]->InitializeRenderingInfo(program);

		Current().renderers[i]->Render();

		glUseProgram(0);
	}

	glfwSwapBuffers(AssetLoader::Instance().currentWindow);
}