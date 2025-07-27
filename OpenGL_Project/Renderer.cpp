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
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

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


void Renderer::InitializeRenderingInfo(GLuint _program)
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


	glm::mat4 VP = projection == ProjectionType::Screen_Orthographic ? Camera::Instance().GetProjectionMatrix(projection) : Camera::Instance().GetProjectionMatrix(projection) * Camera::Instance().GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(_program, "VP"), 1, GL_FALSE, glm::value_ptr(VP));

	//Pass In Uniforms
	glm::vec3 position = Camera::Instance().GetCameraPosition();
	glUniform3f(glGetUniformLocation(_program, "CameraPos"), position.x, position.y, position.z);
	glUniform1f(glGetUniformLocation(_program, "Time"), Time::Instance().time);
	glUniform2f(glGetUniformLocation(_program, "Tiling"), textureTiling.x, textureTiling.y);

	//Color Uniform
	glUniform4f(glGetUniformLocation(_program, "Color"), color.x, color.y, color.z, color.w);

	//Pass In Ambient Light
	glUniform3fv(glGetUniformLocation(_program, "Ambient"), 1, glm::value_ptr(Scene::Current().GetAmbientLight()));

	//Pass In Textures
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		GLuint tex = AssetLoader::Instance().GetTexture(textures[i].textureKey);
		glBindTexture(textures[i].type, tex);

		//Updating Tiling Type Based On The Texture pass Tiling Type
		switch (textures[i].tilingType)
		{
		case TilingType::ClampEdges: //Stops The Color At The Edge Of The Texture
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_T, GL_CLAMP);
			break;
		case TilingType::Repeat: //Repeating Textures Wrap Around When Tiled
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TilingType::ClampBorder: //Clamps Color To Texture Border Color IF Set
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(textures[i].type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;
		}
		
		GLint loc = glGetUniformLocation(_program, textures[i].locationName.c_str());

		glUniform1i(loc, i);

		glActiveTexture(0);
	}

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


//Getters And Setters For The Renderers Color Tint Value
void Renderer::SetColor(glm::vec4 _color)
{
	color = _color;
}

glm::vec4 Renderer::GetColor() const
{
	return color;
}
