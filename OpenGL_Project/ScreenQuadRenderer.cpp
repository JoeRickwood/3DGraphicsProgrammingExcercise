#include "ScreenQuadRenderer.h"
#include "RenderingPipeline.h"
#include <iostream>

ScreenQuadRenderer::ScreenQuadRenderer(std::string _shaderKey, ProjectionType _projectionType) : Renderer(_shaderKey, _projectionType)
{
	translationMat = glm::mat4();
	rotationMat = glm::mat4();
	scaleMat = glm::mat4();
	modelMat = glm::mat4();
}

ScreenQuadRenderer::~ScreenQuadRenderer()
{
	//RenderingPipeline::RemoveRenderer(this);
}

void ScreenQuadRenderer::Init()
{
	//RenderingPipeline::AddRenderer(this);

	//InitVBO();
}

void ScreenQuadRenderer::InitVBO()
{
	glGenBuffers(1, &VBO);

	BindVBOData();
}

void ScreenQuadRenderer::BindVBOData()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &modelMat, GL_DYNAMIC_DRAW);

	glBindVertexArray(mesh->VAO);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ScreenQuadRenderer::Update()
{
	glm::vec2 windowSize = AssetLoader::Instance().windowSize;
	glm::vec3 halfPos = glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f, 0.0f);

	parent->SetPosition(halfPos);
	parent->SetScale(glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f, 1));

	translationMat = glm::translate(glm::mat4(1.0f), parent->GetPosition());

	rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(parent->GetRotation().z), glm::vec3(0.f, 0.f, 1.f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(parent->GetRotation().y), glm::vec3(0.f, 1.f, 0.f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(parent->GetRotation().x), glm::vec3(1.f, 0.f, 0.f));

	scaleMat = glm::scale(glm::mat4(1.0f), parent->GetScale());

	modelMat = translationMat * rotationMat * scaleMat;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &modelMat, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ScreenQuadRenderer::Render()
{
	if (mesh == nullptr)
	{
		std::cerr << "Mesh In Invalid Or Missing" << std::endl;
		return;
	}

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());

	glBindVertexArray(0);
}