#include "InstancedRenderer.h"
#include "RenderingPipeline.h"
#include <iostream>

InstancedRenderer::InstancedRenderer(std::string _shaderKey, ProjectionType _projectionType) : Renderer(_shaderKey, _projectionType)
{
	drawCount = 1;

	VBO = NULL;
}

InstancedRenderer::~InstancedRenderer()
{
	RenderingPipeline::RemoveRenderer(this);
}

void InstancedRenderer::AddInstance(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	++drawCount;

	positions.push_back(pos);
	rotations.push_back(rot);
	scales.push_back(scale);

	glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(rot.y), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos) * rotation * glm::scale(glm::mat4(1.0f), scale);

	ModelMatrixes.push_back(modelMat);
}

void InstancedRenderer::Init()
{
	RenderingPipeline::AddRenderer(this);
}

void InstancedRenderer::InitVBO()
{	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ModelMatrixes.size() * sizeof(glm::mat4), ModelMatrixes.data(), GL_DYNAMIC_DRAW);
	
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

void InstancedRenderer::Render()
{
	if (mesh == nullptr)
	{
		std::cerr << "Mesh In Invalid Or Missing" << std::endl;
		return;
	}

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)mesh->data.size(), drawCount);

	glBindVertexArray(0);
}
