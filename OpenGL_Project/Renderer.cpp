#include "Renderer.h"

Renderer::Renderer(RenderableType _type)
{
	renderable = RenderableLoader::Instance().GetRenderable(_type);

	translationMat = glm::mat4();
	rotationMat = glm::mat4();
	scaleMat = glm::mat4();
	modelMat = glm::mat4();
}

Renderer::~Renderer()
{
}

void Renderer::Render()
{
	//Set The New Shader Program
	glUseProgram(GraphicsLoader::Instance().GetShaderProgram(0));

	GLint ModelMatLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(0), "ModelMatrix");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	parent->ShaderUpdate();

	//Draw Renderable
	glBindVertexArray(renderable->VAO);

	glDrawElements(GL_TRIANGLES, renderable->indices.size(), GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//Unbind The Shader Program
	glUseProgram(0);
}

void Renderer::Update()
{
	translationMat = glm::translate(glm::mat4(1.0f), parent->position);
	rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(parent->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	scaleMat = glm::scale(glm::mat4(1.0f), parent->scale);


	int width = 0;
	int height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::mat4 aspectMat = glm::scale(glm::mat4(1.0f), glm::vec3(800.f / (float)width, 800.f / (float)height, 1.f));

	modelMat = aspectMat * translationMat * rotationMat * scaleMat;
}

Bounds Renderer::GetWorldBounds()
{
	float x = parent->position.x;
	float y = -parent->position.y;

	Bounds ret = Bounds((-0.5f * parent->scale.x) + x, (-0.5f * parent->scale.y) + y, (0.5f * parent->scale.x) + x, (0.5f * parent->scale.y) + y);
	return ret;
}

