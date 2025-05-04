#include "InstancedRenderer.h"

InstancedRenderer::InstancedRenderer(int _type, ShaderType _shader, int _textureID, ProjectionType _projectionType) : Renderer(_type, _shader, _textureID, _projectionType)
{
	drawCount = 1;

	VBO_Instanced = NULL;
	VP = glm::mat4();
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

	ModelMatrixes.push_back(glm::mat4());
}

void InstancedRenderer::InitInstancing()
{
	//Generate The Instanced VBO
	glBindVertexArray(mesh->VAO);

	glGenBuffers(1, &VBO_Instanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
	glBufferData(GL_ARRAY_BUFFER, ModelMatrixes.size() * sizeof(glm::mat4), ModelMatrixes.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (int i = 0; i < drawCount - 1; i++)
	{
		glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotations[i].y), glm::vec3(0.f, 1.f, 0.f));

		glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), positions[i]) * rotation * glm::scale(glm::mat4(1.0f), scales[i]);

		ModelMatrixes[i] = modelMat;
	}
}

void InstancedRenderer::Render()
{
	InitializeRenderingInfo();

	VP = Camera::Instance().GetProjectionMatrix(projection) * Camera::Instance().viewMatrix;

	GLint VPLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "VP");
	glUniformMatrix4fv(VPLoc, 1, GL_FALSE, glm::value_ptr(VP));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);

	glBufferSubData(GL_ARRAY_BUFFER, 0, ModelMatrixes.size() * sizeof(glm::mat4), ModelMatrixes.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)mesh->data.size(), drawCount);

	glBindVertexArray(0);
	glUseProgram(0);
}
