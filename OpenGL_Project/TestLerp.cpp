#include "TestLerp.h"
#include "Renderer.h"

TestLerp::TestLerp(float _speed, int _texID0, int _texID1)
{
	speed = _speed;
	texID0 = _texID0;
	texID1 = _texID1;

	t = 0;
}

TestLerp::~TestLerp()
{
}

void TestLerp::Update()
{
	//Increase T By Time.delta Time To Be passed InTo The Shader
	t += Time::Instance().deltaTime;
}

void TestLerp::ShaderUpdate()
{
	//Get The Renderer Attached To The Object
	Renderer* renderer = parent->GetComponent<Renderer>();

	//Sets The Second Texture In The Used Shader, This Is Handled In THe Shader To Interpolate Between The First And Second Texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GraphicsLoader::Instance().GetTexture(texID1));
	glUniform1i(glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(renderer->shader), "Texture1"), 1);

	//Assign Time In The Shader
	GLint TimeLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(renderer->shader), "Time");
	glUniform1f(TimeLoc, t);
}
