#include "RenderingPipeline.h"
#include "Camera.h"
#include <iostream>
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
	glm::vec2 windowSize = AssetLoader::Instance().GetWindowSize();
	glViewport(0, 0, windowSize.x, windowSize.y);
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