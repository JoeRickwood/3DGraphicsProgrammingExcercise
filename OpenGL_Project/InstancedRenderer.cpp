#include "InstancedRenderer.h"

InstancedRenderer::InstancedRenderer(int _type, ShaderType _shader, int _textureID, ProjectionType _projectionType) : Renderer(_type, _shader, _textureID, _projectionType)
{
	drawCount = 1;
}

InstancedRenderer::~InstancedRenderer()
{
}

void InstancedRenderer::AddInstance(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	drawCount++;

	positions.push_back(pos);
	rotations.push_back(rot);
	scales.push_back(scale);

	InstancedMVPs.push_back(glm::mat4());
}

void InstancedRenderer::InitInstancing()
{
	//Generate The Instanced VBO
	glGenBuffers(1, &VBO_Instanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
	glBufferData(GL_ARRAY_BUFFER, InstancedMVPs.size() * sizeof(glm::mat4), InstancedMVPs.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedRenderer::Render()
{
	InitializeRenderingInfo();

	for (int i = 0; i < drawCount; i++)
	{
		glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), positions[i]) * glm::scale(glm::mat4(1.0f), scales[i]);

		InstancedMVPs[i] = Camera::Instance().GetProjectionMatrix(projection) * Camera::Instance().viewMatrix * modelMat;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
	glBufferSubData(GL_ARRAY_BUFFER, 0, InstancedMVPs.size() * sizeof(glm::mat4), InstancedMVPs.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)mesh->data.size(), drawCount);

	glBindVertexArray(0);
	glUseProgram(0);
}
