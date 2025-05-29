#include "DefaultRenderer.h"
#include "RenderingPipeline.h"
#include <iostream>

DefaultRenderer::DefaultRenderer(std::string _shaderKey, ProjectionType _projectionType) : Renderer(_shaderKey, _projectionType)
{
	translationMat = glm::mat4();
	rotationMat = glm::mat4();
	scaleMat = glm::mat4();
	modelMat = glm::mat4();
}

DefaultRenderer::~DefaultRenderer()
{
	RenderingPipeline::RemoveRenderer(this);
}

void DefaultRenderer::Init() 
{
	RenderingPipeline::AddRenderer(this);
}

void DefaultRenderer::InitializeRenderingInfo(GLuint program)
{
	Renderer::InitializeRenderingInfo(program);

	GLint ModelMatLoc = glGetUniformLocation(AssetLoader::Instance().GetShaderProgram(shaderKey), "ModelMatrix");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	GLint ViewMatLoc = glGetUniformLocation(AssetLoader::Instance().GetShaderProgram(shaderKey), "ViewMatrix");
	glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(Camera::Instance().viewMatrix));

	GLint ProjectionMatLoc = glGetUniformLocation(AssetLoader::Instance().GetShaderProgram(shaderKey), "ProjectionMatrix");
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

	GLuint program = AssetLoader::Instance().GetShaderProgram(shaderKey);

	//Set The New Shader Program
	glUseProgram(program);

	InitializeRenderingInfo(program);

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());

	glBindVertexArray(0);
	glUseProgram(0);
}

void DefaultRenderer::Render(std::string _shaderKeyOverride)
{
	if (mesh == nullptr)
	{
		std::cerr << "Mesh In Invalid Or Missing" << std::endl;
	}

	GLuint program = AssetLoader::Instance().GetShaderProgram(_shaderKeyOverride);

	//Set The New Shader Program
	glUseProgram(program);

	InitializeRenderingInfo(program);

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());

	glBindVertexArray(0);
	glUseProgram(0);
}