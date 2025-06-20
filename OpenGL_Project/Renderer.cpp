#include "Renderer.h"
#include "Time.h"
#include "Scene.h"
#include <iostream>
#include "RenderingPipeline.h"

Renderer::Renderer(std::string _shaderKey = "Default", ProjectionType _projectionType = ProjectionType::Perspective)
{
	shaderKey = _shaderKey;
	projection = _projectionType;
	mesh = nullptr;
	renderType = RenderFront;
	doubleSided = false;
	renderShadows = true;

	VBO = NULL;
}

Renderer::~Renderer()
{

}

void Renderer::Update()
{

}

void Renderer::InitVBO() 
{

}

void Renderer::BindVBOData()
{
}


void Renderer::InitializeRenderingInfo(GLuint program)
{
	if (doubleSided) 
	{
		glDisable(GL_CULL_FACE);
	}
	else 
	{
		glEnable(GL_CULL_FACE);
	}

	glCullFace(renderType);


	glm::mat4 VP = Camera::Instance().GetProjectionMatrix(projection) * Camera::Instance().viewMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "VP"), 1, GL_FALSE, glm::value_ptr(VP));


	//Pass In Uniforms
	glUniform3f(glGetUniformLocation(program, "CameraPos"), Camera::Instance().cameraPosition.x, Camera::Instance().cameraPosition.y, Camera::Instance().cameraPosition.z);
	glUniform1f(glGetUniformLocation(program, "Time"), Time::Instance().time);
	glUniform2f(glGetUniformLocation(program, "Tiling"), textureTiling.x, textureTiling.y);

	//Pass In Point Lights
	glUniform1ui(glGetUniformLocation(program, "PointLightCount"), Scene::Current().GetPointLightCount());
	PointLight** pointLights = Scene::Current().GetPointLights();

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if (pointLights[i] == nullptr)
		{
			continue;
		}

		std::string loc = ("PointLights[" + std::to_string(i) + "].");

		glUniform3fv(glGetUniformLocation(program, (loc + "Position").c_str()), 1, glm::value_ptr(pointLights[i]->parent->position));
		glUniform3fv(glGetUniformLocation(program, (loc + "Color").c_str()), 1, glm::value_ptr(pointLights[i]->color));
		glUniform1f(glGetUniformLocation(program, (loc + "SpecularStrength").c_str()), pointLights[i]->specularStrength);
	
		glUniform1f(glGetUniformLocation(program, (loc + "AttenuationConstant").c_str()), pointLights[i]->attenuationConstant);
		glUniform1f(glGetUniformLocation(program, (loc + "AttenuationLinear").c_str()), pointLights[i]->attenuationLinear);
		glUniform1f(glGetUniformLocation(program, (loc + "AttenuationExponent").c_str()), pointLights[i]->attenuationExponent);
	}

	//Pass In Spot Lights
	glUniform1ui(glGetUniformLocation(program, "SpotLightCount"), Scene::Current().GetSpotLightCount());
	SpotLight** spotLights = Scene::Current().GetSpotLights();

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		if (spotLights[i] == nullptr)
		{
			continue;
		}

		std::string loc = ("SpotLights[" + std::to_string(i) + "].");

		glUniform3fv(glGetUniformLocation(program, (loc + "Position").c_str()), 1, glm::value_ptr(spotLights[i]->parent->position));
		glUniform3fv(glGetUniformLocation(program, (loc + "Direction").c_str()), 1, glm::value_ptr(spotLights[i]->direction));
		glUniform3fv(glGetUniformLocation(program, (loc + "Color").c_str()), 1, glm::value_ptr(spotLights[i]->color));

		glUniform1f(glGetUniformLocation(program, (loc + "InnerCone").c_str()), glm::radians(spotLights[i]->innerCone));
		glUniform1f(glGetUniformLocation(program, (loc + "OuterCone").c_str()), glm::radians(spotLights[i]->outerCone));
		glUniform1f(glGetUniformLocation(program, (loc + "Range").c_str()), glm::radians(spotLights[i]->range));
	}

	DirectionalLight* dirLight = Scene::Current().GetDirectionalLight();

	//Pass In Directional Light
	if (dirLight != nullptr) 
	{
		glUniform3fv(glGetUniformLocation(program, "DirLight.Direction"), 1, glm::value_ptr(dirLight->direction));
		glUniform3fv(glGetUniformLocation(program, "DirLight.Color"), 1, glm::value_ptr(dirLight->color));
		glUniform1f(glGetUniformLocation(program, "DirLight.SpecularStrength"), dirLight->specularStrength);
	}

	//Pass In Ambient Light
	glUniform3fv(glGetUniformLocation(program, "Ambient"), 1, glm::value_ptr(Scene::Current().GetAmbientLight()));

	//Pass In Textures
	for (int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		switch (textures[i].tilingType)
		{
		case TilingType::ClampEdges:
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_T, GL_CLAMP);
			break;
		case TilingType::Repeat:
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TilingType::ClampBorder:
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;
		}

		GLuint tex = AssetLoader::Instance().GetTexture(textures[i].textureKey);
		glBindTexture(textures[i].type, tex);
		
		GLint loc = glGetUniformLocation(program, textures[i].locationName.c_str());

		glUniform1i(loc, i);
	}

	//Pass In Light VP
	glUniformMatrix4fv(glGetUniformLocation(program, "LightVP"), 1, GL_FALSE, glm::value_ptr(RenderingPipeline::GetLightVPMatrix()));

	parent->ShaderUpdate();
}


void Renderer::Render()
{

}

void Renderer::AddTexturePass(std::string _location, std::string _texKey, TextureType _type, TilingType _tilingType = TilingType::Repeat)
{
	textures.push_back(TexturePass(_location, _texKey, _type, _tilingType));
}

void Renderer::SetMesh(Mesh* _mesh)
{
	mesh = _mesh;

	InitVBO();
}

void Renderer::SetShader(std::string _shaderKey)
{
	shaderKey = _shaderKey;
}

void Renderer::SetTextureTiling(glm::vec2 _tiling)
{
	textureTiling = _tiling;
}

void Renderer::SetRenderType(RenderType _type)
{
	if (_type == RenderBoth) 
	{
		renderType = RenderFront;
		doubleSided = true;
	}
	else 
	{
		doubleSided = false;
		renderType = _type;
	}
}

void Renderer::SetShadowRendering(bool _on)
{
	renderShadows = _on;
}
