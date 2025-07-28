#include "Terrain.h"
#include "Scene.h"
#include <iostream>
#include "RenderingPipeline.h"
#include "MathFunctions.h"


Terrain::Terrain(std::string _shaderKey, ProjectionType _projection, int _sizeX, int _sizeY, float _cellSpacing) : Renderer(_shaderKey, ProjectionType::Perspective)
{
	terrainID = -1;
	terrainSize = glm::ivec2(_sizeX, _sizeY);
	cellSpacing = _cellSpacing;
	mesh = nullptr;
	modelMat = glm::mat4();
	terrainScale = 0.0f;
}

Terrain::~Terrain()
{
}

void Terrain::Init()
{
	std::vector<ObjectInstance*> objects = Scene::Current().GetAllObjects();


	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetComponent<Terrain>() != nullptr)
		{
			terrainID++;
		}
	}

	GenerateMesh(0.0f);

	RenderingPipeline::AddRenderer(this);
}

void Terrain::InitVBO()
{
	glGenBuffers(1, &VBO);

	BindVBOData();
}

void Terrain::BindVBOData()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &modelMat, GL_DYNAMIC_DRAW);

	glBindVertexArray(mesh->VAO);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float Terrain::SampleHeight(float _x, float _y)
{
	//float height = ValueNoise_2D(_x * 2.f, _y * 2.f) * terrainScale;
	if (_x >= terrainSize.x) 
	{
		_x = terrainSize.x - 1;
	}

	if (_x < 0)
	{
		_x = 0;
	}


	if (_y >= terrainSize.y)
	{
		_y = terrainSize.y - 1;
	}

	if (_y < 0)
	{
		_y = 0;
	}


	float height = heights[_x * terrainSize.y + _y];

	//float multiplier = Smoothstep(-20, 25, height);

	return height;
}

float Terrain::SampleSteepness(float x, float y)
{
	float invCellSpacing = 1.0f / (2.0f * cellSpacing);

	float rowNeg = SampleHeight(x - 1, y);
	float rowPos = SampleHeight(x + 1, y);
	float colNeg = SampleHeight(x, y - 1);
	float colPos = SampleHeight(x, y + 1);

	float X = rowNeg - rowPos;
	if (x == 0 || x == terrainSize.x - 1)
	{
		X *= 2;
	}

	float Y = colPos - colNeg;
	if (y == 0 || y == terrainSize.y - 1)
	{
		Y *= 2;
	}

	glm::vec3 tangentZ = glm::vec3(0.0f, X * invCellSpacing, 1.0f);
	glm::vec3 tangentX = glm::vec3(1.0f, Y * invCellSpacing, 0.0f);

	glm::vec3 normal = glm::cross(tangentZ, tangentX);
	normal = glm::normalize(normal);

	return glm::dot(normal, glm::vec3(0, 1, 0));
}

float Terrain::GetCellSpacing()
{
	return cellSpacing;
}

glm::vec2 Terrain::GetSize()
{
	return terrainSize;
}

float Terrain::Smoothstep(float edge0, float edge1, float x)
{
	// Scale, and clamp x to 0..1 range
	x = (x - edge0) / (edge1 - edge0);

	if (x < 0) 
	{
		x = 0;
	}

	if (x > 1) 
	{
		x = 1;
	}

	return x * x * (3.0 - 2.0 * x);
}


void Terrain::Update()
{
	modelMat = glm::translate(glm::mat4(1.0f), parent->GetPosition());
}

void Terrain::Render()
{
	if (mesh == nullptr)
	{
		std::cerr << "Mesh In Invalid Or Missing" << std::endl;
		return;
	}

	//Draw Renderable
	glBindVertexArray(mesh->VAO);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->data.size());

	glBindVertexArray(0);
}


void Terrain::GenerateMesh(float _scale)
{
	terrainScale = _scale;

	if (mesh)
	{
		delete mesh;
	}

	positions.clear();
	normals.clear();
	indices.clear();
	texCoords.clear();
	
	//LoadHeightmap("Resources/Heightmap0.raw");
	LoadPerlinMap(terrainScale);

	SaveAsHeightmap();

	//SmoothHeights(0); 

	for (int x = 0; x < terrainSize.x; x++)
	{
		for (int y = 0; y < terrainSize.y; y++)
		{
			float height = heights[(x * terrainSize.y) + y];

			float posX = x * cellSpacing;
			float posY = y * cellSpacing;

			positions.push_back(glm::vec3(posX, height, posY));
			normals.push_back(glm::vec3(0, 1, 0));
			texCoords.push_back(glm::vec2(x, y));
		}
	}

	unsigned int faceCount = (terrainSize.x - 1) * (terrainSize.y - 1) * 2;
	int drawCount = faceCount * 3;
	indices = std::vector<int>(drawCount);

	int index = 0;
	for (int i = 0; i < (terrainSize.x - 1); i++)
	{
		for (int j = 0; j < (terrainSize.y - 1); j++)
		{
			indices[index++] = i * terrainSize.y + j;
			indices[index++] = i * terrainSize.y + j + 1;
			indices[index++] = (i + 1) * terrainSize.y + j;

			indices[index++] = (i + 1) * terrainSize.y + j;
			indices[index++] = i * terrainSize.y + j + 1;
			indices[index++] = (i + 1)* terrainSize.y + j + 1;
		}
	}


	//Recalculate Normals
	float invCellSpacing = 1.0f / (2.0f * cellSpacing);
	for (int x = 0; x < terrainSize.x; ++x)
	{
		for (int y = 0; y < terrainSize.y; ++y)
		{
			float rowNeg = SampleHeight(x - 1, y);
			float rowPos = SampleHeight(x + 1, y);
			float colNeg = SampleHeight(x, y - 1);
			float colPos = SampleHeight(x, y + 1);

			float X = rowNeg - rowPos;
			if (x == 0 || x == terrainSize.x - 1) 
			{
				X *= 2;
			}

			float Y = colPos - colNeg;
			if (y == 0 || y == terrainSize.y - 1) 
			{
				Y *= 2;
			}

			glm::vec3 tangentZ = glm::vec3(0.0f, X * invCellSpacing, 1.0f);
			glm::vec3 tangentX = glm::vec3(1.0f, Y * invCellSpacing, 0.0f);

			glm::vec3 normal = glm::cross(tangentZ, tangentX);
			normal = glm::normalize(normal);

			normals[x * terrainSize.y + y] = normal;
		}
	}

	mesh = AssetLoader::CreateMesh(positions, indices, normals, texCoords);

	SetMesh(mesh);
}

void Terrain::LoadHeightmap(std::string _filepath)
{
	int vertexCount = terrainSize.x * terrainSize.y;

	std::vector<unsigned char> heightValues(vertexCount);

	std::ifstream File;
	File.open(_filepath, std::ios_base::binary);

	if (File) 
	{
		File.read((char*)&heightValues[0], (std::streamsize)heightValues.size());
		File.close();
	}

	heights.resize(vertexCount, 0);
	for (unsigned int i = 0; i < vertexCount; i++)
	{
		heights[i] = Lerp(minHeight, maxHeight, (float)heightValues[i] / 255.0f);
	}
}

void Terrain::SaveAsHeightmap()
{
	uint8_t* pixels = new uint8_t[terrainSize.x * terrainSize.y];
	int index = 0;

	for (int x = 0; x < terrainSize.x; x++)
	{
		for (int y = 0; y < terrainSize.y; y++)
		{
			float noise = InverseLerp(minHeight, maxHeight, (float)heights[x * terrainSize.y + y]);
			noise = glm::clamp(noise * 255.0f, 0.0f, 255.0f);
			pixels[index++] = (uint8_t)(noise);
		}
	}

	AssetLoader::SaveImageToPath("NoiseTextures/NoiseTex", pixels, terrainSize.x, terrainSize.y);
}

void Terrain::LoadPerlinMap(float _scale)
{
	int vertexCount = terrainSize.x * terrainSize.y;
	heights.resize(vertexCount, 0);

	for (int x = 0; x < terrainSize.x; x++)
	{
		for (int y = 0; y < terrainSize.y; y++)
		{
			heights[x * terrainSize.y + y] = Clamp(PerlinNoise(x * cellSpacing, y * cellSpacing) * _scale, minHeight, maxHeight);
		}
	}
}

void Terrain::SmoothHeights(int _iterations)
{
	for (int n = 0; n < _iterations; n++)
	{
		std::vector<float> smoothedMap(heights.size());

		for (int i = 0; i < terrainSize.x; i++)
		{
			for (int j = 0; j < terrainSize.y; j++)
			{
				smoothedMap[i * terrainSize.y + j] = Average(i, j);
			}
		}

		for (int i = 0; i < heights.size(); i++)
		{
			heights[i] = smoothedMap[i];
		}
	}
}

float Terrain::Average(unsigned int x, unsigned int y)
{
	float totalHeight = 0.0f;

	int averageCount = 0;
	for (int i = -1; i < 1; i++)
	{
		for (int j = -1; j < 1; j++)
		{
			float val = 0.0f;

			if (x + i > terrainSize.x || x + i < 0) 
			{
				continue;
			}

			if (y + j > terrainSize.x || y + j < 0)
			{
				continue;
			}

			totalHeight += SampleHeight(i + x, j + y);
			averageCount++;
		}
	}

	return totalHeight / averageCount;
}
