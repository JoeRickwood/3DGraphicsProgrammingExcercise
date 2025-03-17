#include "Renderer.h"

Renderer::Renderer(RenderableType _type, ShaderType _shader, int _textureID)
{
	renderable = RenderableLoader::Instance().GetRenderable(_type);

	translationMat = glm::mat4();
	rotationMat = glm::mat4();
	scaleMat = glm::mat4();
	modelMat = glm::mat4();

	shader = _shader;

	textureID = _textureID;

	flipX = false;
	flipY = false;
	SetUVFrame(uvFrame);
}

Renderer::~Renderer()
{
}

void Renderer::Render()
{
	//Set The New Shader Program
	glUseProgram(GraphicsLoader::Instance().GetShaderProgram(shader));

	GLint ModelMatLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "ModelMatrix");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	Frame frame = uvFrame;

	if (flipX)
	{
		frame.FlipX();
	}

	if (flipY)
	{
		frame.FlipY();
	}

	//Set The UV Frame, Mostly Used For Animation But Also To Reposition The Bounds Of The Image
	GLint UVFrameBLLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "UVFrameBottomLeft");
	glUniform2f(UVFrameBLLoc, frame.bottomLeft.x, frame.bottomLeft.y);

	GLint UVFrameTRLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "UVFrameTopRight");
	glUniform2f(UVFrameTRLoc, frame.topRight.x, frame.topRight.y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GraphicsLoader::Instance().GetTexture(textureID));
	glUniform1i(glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(shader), "Texture0"), 0);

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

	modelMat = GraphicsLoader::Instance().zoomMatrix * aspectMat * translationMat * GraphicsLoader::Instance().viewMatrix * rotationMat * scaleMat;
}

void Renderer::SetUVFrame(Frame frame)
{
	uvFrame = frame;
}

void Renderer::FlipX(bool state)
{
	flipX = state;
}

void Renderer::FlipY(bool state)
{
	flipY = state;

}

Bounds Renderer::GetWorldBounds()
{
	float x = parent->position.x;
	float y = -parent->position.y;

	Bounds ret = Bounds((-0.5f * parent->scale.x) + x, (-0.5f * parent->scale.y) + y, (0.5f * parent->scale.x) + x, (0.5f * parent->scale.y) + y);
	return ret;
}

void Frame::FlipX()
{
	bottomLeft.x = 1.f - bottomLeft.x;
	topRight.x = 1.f - topRight.x;
}

void Frame::FlipY()
{
	bottomLeft.y = 1.f - bottomLeft.y;
	topRight.y = 1.f - topRight.y;
}
