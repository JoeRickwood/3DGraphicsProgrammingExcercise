#include "RenderableInstance.h"

RenderableInstance::RenderableInstance(RenderableType _type, glm::vec3 _position, float _rotation, glm::vec3 _scale)
{
	renderable = Renderable();

	translationMat = glm::mat4();
	rotationMat = glm::mat4();
	scaleMat = glm::mat4();
	modelMat = glm::mat4();

	position = _position;
	rotation = _rotation;
	scale = _scale;
}

RenderableInstance::~RenderableInstance()
{
}

void RenderableInstance::Init()
{
	renderable.Init();

	InitComponents();
}

void RenderableInstance::Render()
{
	//Set The New Shader Program
	glUseProgram(ShaderLoader::Instance().GetShaderProgram(0));

	GLint ModelMatLoc = glGetUniformLocation(ShaderLoader::Instance().GetShaderProgram(0), "ModelMatrix");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	//Draw Renderable
	glBindVertexArray(renderable.VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//Unbind The Shader Program
	glUseProgram(0);
}

void RenderableInstance::Update()
{
	UpdateComponents();

	translationMat = glm::translate(glm::mat4(1.0f), position);
	rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
	scaleMat = glm::scale(glm::mat4(1.0f), scale);


	modelMat = translationMat * rotationMat * scaleMat;
}

void RenderableInstance::InitComponents()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Init();
	}
}

void RenderableInstance::UpdateComponents()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}


//COMPONENTS
Component::Component()
{
	parent = nullptr;
}

Component::~Component()
{
}

void Component::Init()
{
}

void Component::Update()
{
}

