#include "Skybox.h"
#include "RenderingPipeline.h"

Skybox::Skybox(std::string _skyboxShaderKey, std::string _skyboxTextureKey) : Renderer(_skyboxShaderKey, ProjectionType::Perspective)
{
	//0 Is The Skybox Model
	mesh = AssetLoader::Instance().GetMesh("Cubemap");

	AddTexturePass("Texture_Skybox", _skyboxTextureKey, TextureType::CubeMap, TilingType::ClampEdges);

	shaderKey = _skyboxShaderKey;
}

Skybox::~Skybox()
{
	RenderingPipeline::RemoveRenderer(this);
}

void Skybox::InitializeRenderingInfo(GLuint program)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(textures[0].type, AssetLoader::Instance().GetSkybox(textures[0].textureKey));
	glUniform1i(glGetUniformLocation(program, textures[0].locationName.c_str()), 0);

	//truncate values
	glm::mat4 matrix = Camera::Instance().GetProjectionMatrix(Perspective) * glm::mat4(glm::mat3(Camera::Instance().GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(program, "VP"), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Skybox::Init()
{
	RenderingPipeline::AddRenderer(this);
}

void Skybox::Render()
{
	glBindVertexArray(mesh->VAO);

	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());

	glDepthFunc(GL_LESS);
	glBindTexture(textures[0].type, NULL);
}
