#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::Bind()
{



}

void Material::Unbind()
{
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
