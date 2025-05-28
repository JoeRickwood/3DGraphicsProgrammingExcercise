#include "Renderer.h"
#include "Time.h"
#include "Scene.h"
#include <iostream>

Renderer::Renderer(std::string _shaderKey = "Default", ProjectionType _projectionType = ProjectionType::Perspective)
{
	shaderKey = _shaderKey;
	projection = _projectionType;
	mesh = nullptr;
}

Renderer::~Renderer()
{

}

void Renderer::Update()
{

}

void Renderer::InitializeRenderingInfo()
{
	GLuint prgm = AssetLoader::Instance().GetShaderProgram(shaderKey);

	//Set The New Shader Program
	glUseProgram(prgm);

	//Pass In Uniforms
	glUniform3f(glGetUniformLocation(prgm, "CameraPos"), Camera::Instance().cameraPosition.x, Camera::Instance().cameraPosition.y, Camera::Instance().cameraPosition.z);
	glUniform1f(glGetUniformLocation(prgm, "Time"), Time::Instance().time);
	glUniform2f(glGetUniformLocation(prgm, "Tiling"), textureTiling.x, textureTiling.y);


	//Pass In Point Lights
	glUniform1ui(glGetUniformLocation(prgm, "PointLightCount"), Scene::Current().GetPointLightCount());
	PointLight** pointLights = Scene::Current().GetPointLights();

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if (pointLights[i] == nullptr)
		{
			continue;
		}

		std::string loc = ("PointLights[" + std::to_string(i) + "].");

		glUniform3fv(glGetUniformLocation(prgm, (loc + "Position").c_str()), 1, glm::value_ptr(pointLights[i]->parent->position));
		glUniform3fv(glGetUniformLocation(prgm, (loc + "Color").c_str()), 1, glm::value_ptr(pointLights[i]->color));
		glUniform1f(glGetUniformLocation(prgm, (loc + "SpecularStrength").c_str()), pointLights[i]->specularStrength);
	
		glUniform1f(glGetUniformLocation(prgm, (loc + "AttenuationConstant").c_str()), pointLights[i]->attenuationConstant);
		glUniform1f(glGetUniformLocation(prgm, (loc + "AttenuationLinear").c_str()), pointLights[i]->attenuationLinear);
		glUniform1f(glGetUniformLocation(prgm, (loc + "AttenuationExponent").c_str()), pointLights[i]->attenuationExponent);
	}

	//Pass In Spot Lights
	glUniform1ui(glGetUniformLocation(prgm, "SpotLightCount"), Scene::Current().GetSpotLightCount());
	SpotLight** spotLights = Scene::Current().GetSpotLights();

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		if (spotLights[i] == nullptr)
		{
			continue;
		}

		std::string loc = ("SpotLights[" + std::to_string(i) + "].");

		glUniform3fv(glGetUniformLocation(prgm, (loc + "Position").c_str()), 1, glm::value_ptr(spotLights[i]->parent->position));
		glUniform3fv(glGetUniformLocation(prgm, (loc + "Direction").c_str()), 1, glm::value_ptr(spotLights[i]->direction));
		glUniform3fv(glGetUniformLocation(prgm, (loc + "Color").c_str()), 1, glm::value_ptr(spotLights[i]->color));

		glUniform1f(glGetUniformLocation(prgm, (loc + "InnerCone").c_str()), glm::radians(spotLights[i]->innerCone));
		glUniform1f(glGetUniformLocation(prgm, (loc + "OuterCone").c_str()), glm::radians(spotLights[i]->outerCone));
	}


	DirectionalLight* dirLight = Scene::Current().GetDirectionalLight();

	//Pass In Directional Light
	if (dirLight != nullptr) 
	{
		glUniform3fv(glGetUniformLocation(prgm, "DirLight.Direction"), 1, glm::value_ptr(dirLight->direction));
		glUniform3fv(glGetUniformLocation(prgm, "DirLight.Color"), 1, glm::value_ptr(dirLight->color));
		glUniform1f(glGetUniformLocation(prgm, "DirLight.SpecularStrength"), dirLight->specularStrength);
	}

	//Pass In Textures
	for (int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLuint tex = AssetLoader::Instance().GetTexture(textures[i].textureKey);
		glBindTexture(textures[i].type, tex);
		
		GLint loc = glGetUniformLocation(prgm, textures[i].locationName.c_str());

		glUniform1i(loc, i);
	}

	parent->ShaderUpdate();
}

void Renderer::Render()
{

}

void Renderer::AddTexture(std::string _location, std::string _texKey, TextureType _type)
{
	textures.push_back(TexturePass(_location, _texKey, _type));
}

void Renderer::SetMesh(Mesh* _mesh)
{
	mesh = _mesh;
}

void Renderer::SetShader(std::string _shaderKey)
{
	shaderKey = _shaderKey;
}

void Renderer::SetTextureTiling(glm::vec2 _tiling)
{
	textureTiling = _tiling;
}
