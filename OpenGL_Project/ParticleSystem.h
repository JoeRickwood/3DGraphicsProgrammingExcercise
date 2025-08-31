#pragma once
#include "Renderer.h"

class ParticleSystem : public Renderer
{
public:
	ParticleSystem(std::string _shaderKey, std::string _computeShaderKey, ProjectionType _projectionType);
	~ParticleSystem();

	void Init() override;

	void Update() override;
	void ComputeRender() override;
	void Render() override;

	void InitVBO() override;
	void BindVBOData() override;

	void SetReset(bool _doesReset);
	void SetRandom(float _random);


protected:
	std::string computeShaderKey;

	GLuint VAO;
	GLuint VBO_PositionLife;
	GLuint VBO_Velocity;

	glm::vec4 velocityLifeChange;

	int groupCountX;
	int workGroupSizeX;
	int numParticles;

	int reset = 1;
	float randomness = 1.0f;

	void InitializeRenderingInfo(GLuint program)override;
};

