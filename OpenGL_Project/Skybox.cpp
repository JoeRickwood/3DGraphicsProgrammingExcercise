#include "Skybox.h"
#include "RenderingPipeline.h"

Skybox::Skybox(std::string _skyboxShaderKey, std::string _skyboxTextureKey) : Renderer(_skyboxShaderKey, ProjectionType::Perspective)
{
	//0 Is The Skybox Model
	mesh = AssetLoader::Instance().GetMesh("Cubemap");

	AddTexturePass("Texture_Skybox", _skyboxTextureKey, TextureType::CubeMap);

	shaderKey = _skyboxShaderKey;
}

Skybox::~Skybox()
{
	RenderingPipeline::RemoveRenderer(this);
}

void Skybox::Init()
{
	RenderingPipeline::AddRenderer(this);
}

void Skybox::Render()
{
	GLuint prgm = AssetLoader::Instance().GetShaderProgram(shaderKey);

	glUseProgram(prgm);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(textures[0].type, AssetLoader::Instance().GetSkybox(textures[0].textureKey));
	glUniform1i(glGetUniformLocation(prgm, textures[0].locationName.c_str()), 0);

	//truncate values
	glm::mat4 matrix = Camera::Instance().GetProjectionMatrix(Perspective) * glm::mat4(glm::mat3(Camera::Instance().viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(prgm, "VP"), 1, GL_FALSE, glm::value_ptr(matrix));

	glBindVertexArray(mesh->VAO);

	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());
	glDepthFunc(GL_LESS);

	glBindTexture(textures[0].type, NULL);
	glUseProgram(0);
}
