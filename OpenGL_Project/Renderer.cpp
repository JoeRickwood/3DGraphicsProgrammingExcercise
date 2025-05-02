#include "Renderer.h"

Renderer::Renderer(int _type, ShaderType _shader, int _textureID, ProjectionType _projectionType)
{
	mesh = MeshLoader::Instance().GetMesh(_type);

	shader = _shader;

	textureID = _textureID;

	projection = _projectionType;
}

Renderer::~Renderer()
{

}

void Renderer::InitializeRenderingInfo()
{
	//Set The New Shader Program
	glUseProgram(GraphicsLoader::Instance().GetShaderProgram(shader));

	GLint CameraLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "CameraPos");
	glUniform3f(CameraLoc, Camera::Instance().cameraPosition.x, Camera::Instance().cameraPosition.y, Camera::Instance().cameraPosition.z);

	GLint TextureTilingLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "Tiling");
	glUniform2f(TextureTilingLoc, textureTiling.x, textureTiling.y);

	glActiveTexture(GL_TEXTURE0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, GraphicsLoader::Instance().GetTexture(textureID));
	glUniform1i(glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "Texture0"), 0);

	parent->ShaderUpdate();
}


void Renderer::Render()
{

}

void Renderer::Update()
{
}


Bounds Renderer::GetWorldBounds()
{
	float x = parent->position.x;
	float y = -parent->position.y;

	Bounds ret = Bounds((-0.5f * parent->scale.x) + x, (-0.5f * parent->scale.y) + y, (0.5f * parent->scale.x) + x, (0.5f * parent->scale.y) + y);
	return ret;
}
