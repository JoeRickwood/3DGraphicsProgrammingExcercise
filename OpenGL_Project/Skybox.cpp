#include "Skybox.h"


Skybox::Skybox(int _skybox, int _shader)
{
	//0 Is The Skybox Model
	mesh = MeshLoader::Instance().GetMesh(0);

	skyboxShader = _shader;
	skyboxTexID = _skybox;
}

Skybox::~Skybox()
{
}

void Skybox::Render()
{
	GLuint prgm = GraphicsLoader::Instance().GetShaderProgram(skyboxShader);

	glUseProgram(prgm);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, GraphicsLoader::Instance().GetSkybox(skyboxTexID));
	glUniform1i(glGetUniformLocation(prgm, "Texture_Skybox"), 0);

	//truncate values
	glm::mat4 matrix = Camera::Instance().GetProjectionMatrix(Perspective) * glm::mat4(glm::mat3(Camera::Instance().viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(prgm, "VP"), 1, GL_FALSE, glm::value_ptr(matrix));

	glBindVertexArray(mesh->VAO);

	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());
	glDepthFunc(GL_LESS);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}
