#include "RenderingPipeline.h"
#include "Camera.h"
#include <iostream>
#include "Scene.h"

RenderingPipeline::RenderingPipeline()
{
	depthMap = 0;
	depthMapFBO = 0;
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
		GLuint program = AssetLoader::Instance().GetShaderProgram(Current().renderers[i]->shaderKey);
		glUseProgram(program);

		Current().renderers[i]->InitializeRenderingInfo(program);

		Current().renderers[i]->Render();

		glUseProgram(0);
	}

	glfwSwapBuffers(AssetLoader::Instance().currentWindow);
}

void RenderingPipeline::Render(std::string shaderKeyOverride)
{
	glViewport(0, 0, (GLsizei)AssetLoader::Instance().windowSize.x, (GLsizei)AssetLoader::Instance().windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < Current().renderers.size(); ++i)
	{
		GLuint program = AssetLoader::Instance().GetShaderProgram(shaderKeyOverride);
		glUseProgram(program);

		Current().renderers[i]->InitializeRenderingInfo(program);

		Current().renderers[i]->Render();

		glUseProgram(0);
	}

	glfwSwapBuffers(AssetLoader::Instance().currentWindow);
}

void RenderingPipeline::ShadowPass()
{
	//Use The Shadow Program
	GLuint program = AssetLoader::Instance().GetShaderProgram("DepthTesting");

	glUseProgram(program);

	//Pass The Light VP Into The Shader Uniform
	glUniformMatrix4fv(glGetUniformLocation(program, "LightVP"), 1, GL_FALSE, glm::value_ptr(GetLightVPMatrix()));

	//Set The Viewport To Be The Same Size As The Shadow Texture
	glViewport(0, 0, Current().SHADOW_WIDTH, Current().SHADOW_HEIGHT);


	glBindFramebuffer(GL_FRAMEBUFFER, Current().depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);


	//Cull Front Faces For Peter Panning
	glCullFace(GL_FRONT);

	for (int i = 0; i < Current().renderers.size(); ++i)
	{
		if (!Current().renderers[i]->renderShadows) 
		{
			continue;
		}

		Current().renderers[i]->Render();
	}

	glCullFace(GL_BACK);

	//Bind The Current Frame Buffer And The Shader Program Back To Defaults
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}




std::vector<glm::vec4> RenderingPipeline::GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
	const auto inv = glm::inverse(proj * view);

	std::vector<glm::vec4> frustumCorners;
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const glm::vec4 pt =
					inv * glm::vec4(
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						2.0f * z - 1.0f,
						1.0f);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
} 

glm::mat4 RenderingPipeline::GetLightVPMatrix()
{
	std::vector<glm::vec4> corners = Current().GetFrustumCornersWorldSpace(Camera::Instance().GetProjectionMatrix(ShadowPerspective), Camera::Instance().viewMatrix);

	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const auto& v : corners)
	{
		center += glm::vec3(v);
	}
	center /= corners.size();

	const auto lightView = glm::lookAt(
		center + (Scene::Current().GetDirectionalLight() != nullptr ? - Scene::Current().GetDirectionalLight()->direction : glm::vec3(0.f)),
		center,
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const auto& v : corners)
	{
		const auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	const float zNearOffset = 5.0f;
	const float zFarOffset = 5.0f;

	constexpr float zMult = 10.0f;
	if (minZ < 0)
	{
		minZ *= zMult;
	}
	else
	{
		minZ /= zMult;
	}
	if (maxZ < 0)
	{
		maxZ /= zMult;
	}
	else
	{
		maxZ *= zMult;
	}

	minZ -= zNearOffset;
	maxZ += zFarOffset;


	const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

	return lightProjection * lightView;
}  


void RenderingPipeline::InitShadowRendering()
{
	//Generate Frame Buffer + Depth Map Textures
	glGenFramebuffers(1, &Current().depthMapFBO);

	glGenTextures(1, &Current().depthMap);
	glBindTexture(GL_TEXTURE_2D, Current().depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Current().SHADOW_WIDTH, Current().SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, Current().depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Current().depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	AssetLoader::Instance().AddTexture(Current().depthMap, "DepthMap");
}

GLuint RenderingPipeline::GetShadowMap()
{
	return Current().depthMap;
}
