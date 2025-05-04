#include "Water.h"
#include "Time.h"
#include "Renderer.h"

Water::Water(float _waveSpeed, float _waveFrequency, float _waveScale)
{
	waveSpeed = _waveSpeed;
	waveFrequency = _waveFrequency;
	waveScale = _waveScale;

	t = 0;
}

Water::~Water()
{
}

void Water::Update()
{
	t += Time::Instance().deltaTime * waveSpeed;
}

void Water::ShaderUpdate()
{
	GLint TimeLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(parent->GetComponent<Renderer>()->shader), "WaterTime");
	glUniform1f(TimeLoc, t);

	GLint ScaleLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(parent->GetComponent<Renderer>()->shader), "WaterScale");
	glUniform1f(ScaleLoc, waveScale);

	GLint FreqLoc = glGetUniformLocation(GraphicsLoader::Instance().GetShaderProgram(parent->GetComponent<Renderer>()->shader), "WaterFrequency");
	glUniform1f(FreqLoc, waveFrequency);
}
