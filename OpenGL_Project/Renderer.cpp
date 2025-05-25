#include "Renderer.h"
#include "Time.h"
#include "Scene.h"
#include <iostream>

Renderer::Renderer(ShaderType _shader, ProjectionType _projectionType)
{
	shader = _shader;
	projection = _projectionType;
	mesh = nullptr;
}

Renderer::~Renderer()
{

}

void Renderer::Update()
{

}

void Renderer::InitializeRenderingInfo()
{
	GLuint prgm = GraphicsLoader::Instance().GetShaderProgram(shader);

	//Set The New Shader Program
	glUseProgram(prgm);

	//Pass In Uniforms
	glUniform3f(glGetUniformLocation(prgm, "CameraPos"), Camera::Instance().cameraPosition.x, Camera::Instance().cameraPosition.y, Camera::Instance().cameraPosition.z);
	glUniform1f(glGetUniformLocation(prgm, "Time"), Time::Instance().time);
	glUniform2f(glGetUniformLocation(prgm, "Tiling"), textureTiling.x, textureTiling.y);

	for (int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLuint tex = GraphicsLoader::Instance().GetTexture(textures[i].texID);
		glBindTexture(textures[i].type, tex);
		
		GLint loc = glGetUniformLocation(prgm, textures[i].locationName.c_str());

		glUniform1i(loc, i);
	}

	parent->ShaderUpdate();
}

void Renderer::Render()
{

}

void Renderer::AddTexture(std::string _location, int _texID, TextureType _type)
{
	textures.push_back(TexturePass(_location, _texID, _type));
}

void Renderer::SetMesh(Mesh* _mesh)
{
	mesh = _mesh;
}

void Renderer::SetShader(ShaderType _shader)
{
	shader = _shader;
}
