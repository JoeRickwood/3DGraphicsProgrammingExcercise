#include "DefaultRenderer.h"
#include <iostream>

DefaultRenderer::DefaultRenderer(ShaderType _shader, ProjectionType _projectionType) : Renderer(_shader, _projectionType)
{
	translationMat = glm::mat4();
	rotationMat = glm::mat4();
	scaleMat = glm::mat4();
	modelMat = glm::mat4();
}

DefaultRenderer::~DefaultRenderer()
{
}

void DefaultRenderer::InitializeRenderingInfo()
{
	Renderer::InitializeRenderingInfo();

	GLint ModelMatLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "ModelMatrix");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	GLint ViewMatLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "ViewMatrix");
	glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(Camera::Instance().viewMatrix));

	GLint ProjectionMatLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "ProjectionMatrix");
	glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(Camera::Instance().GetProjectionMatrix(projection)));

}

void DefaultRenderer::Update()
{
	translationMat = glm::translate(glm::mat4(1.0f), parent->position);
	rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(parent->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	scaleMat = glm::scale(glm::mat4(1.0f), parent->scale);

	modelMat = translationMat * rotationMat * scaleMat;
}

void DefaultRenderer::Render()
{
	if (mesh == nullptr) 
	{
		std::cerr << "Mesh In Invalid Or Missing" << std::endl;
	}

	InitializeRenderingInfo();

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());

	glBindVertexArray(0);
	glUseProgram(0);
}
