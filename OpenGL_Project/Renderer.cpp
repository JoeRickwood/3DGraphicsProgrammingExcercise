#include "Renderer.h"
#include "Time.h"
#include "Scene.h"

Renderer::Renderer(int _type, ShaderType _shader, int _textureID, ProjectionType _projectionType)
{
	mesh = MeshLoader::Instance().GetMesh(_type);

	shader = _shader;

	textureID = _textureID;

	projection = _projectionType;

	cameraLoc = NULL;
	timeLoc = NULL;
	mainTexLoc = NULL;
	mainTextureTilingLoc = NULL;
}

Renderer::~Renderer()
{

}


//Uses This Function To Store The Uniform Locations Of The Shader
void Renderer::ShaderInit()
{
	GLuint prgm = GraphicsLoader::Instance().GetShaderProgram(shader);

	cameraLoc = glGetUniformLocation(prgm, "CameraPos");
	timeLoc = glGetUniformLocation(prgm, "Time");
	mainTexLoc = glGetUniformLocation(prgm, "Texture0");
	mainTextureTilingLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "Tiling");
}

void Renderer::InitializeRenderingInfo()
{
	GLuint prgm = GraphicsLoader::Instance().GetShaderProgram(shader);

	//Set The New Shader Program
	glUseProgram(prgm);

	//Pass In Uniforms
	glUniform3f(cameraLoc, Camera::Instance().cameraPosition.x, Camera::Instance().cameraPosition.y, Camera::Instance().cameraPosition.z);
	glUniform1f(timeLoc, Time::Instance().time);
	glUniform2f(mainTextureTilingLoc, textureTiling.x, textureTiling.y);

	glActiveTexture(GL_TEXTURE0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, GraphicsLoader::Instance().GetTexture(textureID));

	glUniform1i(mainTexLoc, 0);

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
