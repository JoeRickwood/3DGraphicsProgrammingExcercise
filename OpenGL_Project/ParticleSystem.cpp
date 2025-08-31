#include "ParticleSystem.h"
#include "Time.h"
#include "RenderingPipeline.h"


ParticleSystem::ParticleSystem(std::string _shaderKey, std::string _computeShaderKey, ProjectionType _projectionType) : Renderer(_shaderKey, _projectionType)
{
	computeShaderKey = _computeShaderKey;

	groupCountX = 1000;
	workGroupSizeX = 128;
	numParticles = workGroupSizeX * groupCountX;

	VAO = NULL;
	VBO_PositionLife = NULL;
	VBO_Velocity = NULL;

	velocityLifeChange = glm::vec4();
}

ParticleSystem::~ParticleSystem() 
{

}

void ParticleSystem::Init()
{
	RenderingPipeline::AddRenderer(this);

	InitVBO();
}

void ParticleSystem::Update() 
{
	glm::vec3 gravity = glm::vec3(0.0f, -4.81f, 0.0f) * Time::Instance().deltaTime;

	velocityLifeChange = glm::vec4(gravity, Time::Instance().deltaTime);
}

void ParticleSystem::ComputeRender() 
{
	GLuint program = AssetLoader::Instance().GetComputeShaderProgram(computeShaderKey);
	glUseProgram(program);

	glUniform1i(glGetUniformLocation(program, "SeedLife"), 2312);
	glUniform1i(glGetUniformLocation(program, "SeedX"), 541);
	glUniform1i(glGetUniformLocation(program, "SeedY"), 383);
	glUniform1i(glGetUniformLocation(program, "SeedZ"), 745);

	glUniform1f(glGetUniformLocation(program, "Randomness"), randomness);
	glUniform1i(glGetUniformLocation(program, "Reset"), reset);

	glUniform4fv(glGetUniformLocation(program, "VelocityLifeChange"), 1, glm::value_ptr(velocityLifeChange));
	glUniform3fv(glGetUniformLocation(program, "EmitterOrigin"), 1, glm::value_ptr(parent->GetPosition()));

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO_PositionLife);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VBO_Velocity);

	glDispatchCompute(groupCountX, 1, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ParticleSystem::Render()
{
	//Draw Renderable
	glBindVertexArray(VAO);

	glDrawArrays(GL_POINTS, 0, numParticles);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindVertexArray(0);
}

void ParticleSystem::InitVBO()
{
	glGenBuffers(1, &VBO_PositionLife);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_PositionLife);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * numParticles, NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &VBO_Velocity);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_Velocity);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * numParticles, NULL, GL_DYNAMIC_DRAW);

	//Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_PositionLife);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void ParticleSystem::BindVBOData()
{
}

void ParticleSystem::SetReset(bool _doesReset)
{
	reset = (int)_doesReset;
}

void ParticleSystem::SetRandom(float _random)
{
	randomness = _random;
}


void ParticleSystem::InitializeRenderingInfo(GLuint program)
{
	Renderer::InitializeRenderingInfo(program);
}
