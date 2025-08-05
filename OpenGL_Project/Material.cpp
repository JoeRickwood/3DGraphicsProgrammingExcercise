#include "AssetLoader.h"

Material::Material()
{
	renderType = RENDER_FRONT;
}

Material::~Material()
{
}

GLuint Material::Bind()
{
	GLuint program = AssetLoader::Instance().GetShaderProgram(shaderKey);
	glUseProgram(program);
	return program;
}

void Material::SetUniforms(GLuint _program)
{
	glCullFace(renderType);

	//Pass In Textures
	for (int i = 0; i < texturePasses.size(); i++)
	{
		TexturePass pass = texturePasses[i];

		glActiveTexture(GL_TEXTURE0 + i);

		GLuint tex = AssetLoader::Instance().GetTexture(pass.textureKey);
		glBindTexture(pass.type, tex);

		//Updating Tiling Type Based On The Texture pass Tiling Type
		switch (pass.tilingType)
		{
		case TextureTilingType::CLAMP_EDGES: //Stops The Color At The Edge Of The Texture
			glTexParameteri(pass.type, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(pass.type, GL_TEXTURE_WRAP_T, GL_CLAMP);
			break;
		case TextureTilingType::REPEAT: //Repeating Textures Wrap Around When Tiled
			glTexParameteri(pass.type, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(pass.type, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TextureTilingType::CLAMP_BORDER: //Clamps Color To Texture Border Color If Set
			glTexParameteri(pass.type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(pass.type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;
		}

		GLint loc = glGetUniformLocation(_program, pass.locationName.c_str());

		glUniform1i(loc, i);

		glActiveTexture(0);
	}


	//Value Pass-Ins
	for (int i = 0; i < valuePasses.size(); i++)
	{
		ValuePass pass = valuePasses[i];

		switch (pass.type)
		{
		case ValuePassInType::FLOAT:
			glUniform1f(glGetUniformLocation(_program, pass.name.c_str()), pass.value.x);
			break;
		case ValuePassInType::VECTOR2:
			glUniform2fv(glGetUniformLocation(_program, pass.name.c_str()), 1, glm::value_ptr(glm::vec2(pass.value.x, pass.value.y)));
			break;
		case ValuePassInType::VECTOR3:
			glUniform3fv(glGetUniformLocation(_program, pass.name.c_str()), 1, glm::value_ptr(glm::vec3(pass.value.x, pass.value.y, pass.value.z)));
			break;
		case ValuePassInType::VECTOR4:
			glUniform3fv(glGetUniformLocation(_program, pass.name.c_str()), 1, glm::value_ptr(pass.value));
			break;
		}
	}

	//Matrix Pass-Ins
	for (int i = 0; i < matrixPasses.size(); i++)
	{
		MatrixPass pass = matrixPasses[i];

		switch (pass.type)
		{
		case ValuePassInType::MATRIX2X2:
			glUniformMatrix2fv(glGetUniformLocation(_program, pass.name.c_str()), 1,  GL_FALSE, glm::value_ptr(glm::mat2(pass.value)));
			break;
		case ValuePassInType::MATRIX3X3:
			glUniformMatrix3fv(glGetUniformLocation(_program, pass.name.c_str()), 1, GL_FALSE, glm::value_ptr(glm::mat3(pass.value)));
			break;
		case ValuePassInType::MATRIX4X4:
			glUniformMatrix4fv(glGetUniformLocation(_program, pass.name.c_str()), 1, GL_FALSE, glm::value_ptr(glm::mat4(pass.value)));
			break;
		}
	}
}

void Material::Unbind()
{
	glUseProgram(0);
}

void Material::AddTexturePass(TexturePass _value)
{
	texturePasses.emplace_back(_value);
}

TexturePass* Material::GetTexturePass(std::string _key)
{
	for (int i = 0; i < texturePasses.size(); ++i)
	{
		if (texturePasses[i].textureKey == _key)
		{
			return &texturePasses[i];
		}
	}

	return nullptr;
}

void Material::AddValuePass(ValuePass _value)
{
	valuePasses.emplace_back(_value);
}

ValuePass* Material::GetValuePass(std::string _key)
{
	for (int i = 0; i < valuePasses.size(); ++i)
	{
		if (valuePasses[i].name == _key)
		{
			return &valuePasses[i];
		}
	}

	return nullptr;
}

void Material::AddMatrixPass(MatrixPass _value)
{
	matrixPasses.push_back(_value);
}

MatrixPass* Material::GetMatrixPass(std::string _key)
{
	for (int i = 0; i < matrixPasses.size(); i++)
	{
		if (matrixPasses[i].name == _key)
		{
			return &matrixPasses[i];
		}
	}

	return nullptr;
}

void Material::SetShader(std::string _shader) 
{
	shaderKey = _shader;
}


std::string Material::GetShader() const
{
	return shaderKey;
}

void Material::SetRenderType(RenderType _type) 
{
	renderType = _type;
}

RenderType Material::GetRenderType() const
{
	return renderType;
}