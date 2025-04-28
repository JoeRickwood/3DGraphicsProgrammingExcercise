#include "Renderer.h"

Renderer::Renderer(int _type, ShaderType _shader, int _textureID, ProjectionType _projectionType)
{
	mesh = MeshLoader::Instance().GetMesh(_type);

	translationMat = glm::mat4();
	rotationMat = glm::mat4();
	scaleMat = glm::mat4();
	modelMat = glm::mat4();

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

	GLint ModelMatLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "ModelMatrix");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	GLint ViewMatLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "ViewMatrix");
	glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(Camera::Instance().viewMatrix));

	GLint ProjectionMatLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "ProjectionMatrix");
	glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(Camera::Instance().GetProjectionMatrix(projection)));

	GLint CameraLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "CameraPos");
	glUniform3f(CameraLoc, Camera::Instance().cameraPosition.x, Camera::Instance().cameraPosition.y, Camera::Instance().cameraPosition.z);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GraphicsLoader::Instance().GetTexture(textureID));
	glUniform1i(glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "Texture0"), 0);

	parent->ShaderUpdate();
}


void Renderer::Render()
{
	InitializeRenderingInfo();

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());

	glBindVertexArray(0);
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

	modelMat = translationMat * rotationMat * scaleMat;
}


Bounds Renderer::GetWorldBounds()
{
	float x = parent->position.x;
	float y = -parent->position.y;

	Bounds ret = Bounds((-0.5f * parent->scale.x) + x, (-0.5f * parent->scale.y) + y, (0.5f * parent->scale.x) + x, (0.5f * parent->scale.y) + y);
	return ret;
}
