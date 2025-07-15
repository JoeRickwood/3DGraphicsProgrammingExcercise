#include "Terrain.h"
#include "Scene.h"
#include <iostream>
#include "RenderingPipeline.h"
#include "PerlinNoise.h"


Terrain::Terrain(ProjectionType _projection, int _sizeX, int _sizeY, float _cellSpacing) : Renderer("Default", ProjectionType::Perspective)
{
	terrainID = -1;
	terrainSize = glm::ivec2(_sizeX, _sizeY);
	cellSpacing = _cellSpacing;
	mesh = nullptr;
	modelMat = glm::mat4();
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

	GenerateMesh();

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
	if (_x < 0) 
	{
		_x = 0;
	}

	if (_x >= terrainSize.x) 
	{
		_x = terrainSize.x;
	}

	if (_y < 0)
	{
		_y = 0;
	}

	if (_y >= terrainSize.x)
	{
		_y = terrainSize.x;
	}

	int index = _x * terrainSize.y + _y;

	if (index >= heights.size()) 
	{
		return 0.f;
	}

	return heights[index];
}

glm::vec2 Terrain::GetSize()
{
	return terrainSize;
}

void Terrain::LoadHeightMap(std::string filePath)
{
	heights.clear();

	unsigned int vertexCount = terrainSize.x * terrainSize.y;

	std::vector<unsigned char> heightValues(vertexCount);

	std::ifstream File;
	File.open(filePath, std::ios_base::binary);
	if (File) 
	{
		File.read((char*)&heightValues[0], (std::streamsize)heightValues.size());
		File.close();
	}
	else 
	{
		std:cout << "Heightmap Failed To Load";
	}

	heights.resize(vertexCount, 0);
	for (int i = 0; i < positions.size(); i++)
	{
		if (heightValues.size() <= i)
		{
			break;
		}

		heights[i] = (float)heightValues[i];
		positions[i].y = heights[i];
	}

	GenerateMesh();
}

void Terrain::LoadPerlinMap()
{
	heights.clear();

	for (int x = 0; x < terrainSize.x; x++)
	{
		for (int y = 0;  y < terrainSize.y;  y++)
		{
			float height = ValueNoise_2D(x, y) * 50.f;

			heights.push_back(height);
		}
	}

	GenerateMesh();
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

void Terrain::GenerateMesh()
{
	if (mesh)
	{
		delete mesh;
	}

	positions.clear();
	normals.clear();
	indices.clear();
	texCoords.clear();

	for (int x = 0; x < terrainSize.x; x++)
	{
		for (int y = 0; y < terrainSize.y; y++)
		{
			float height = SampleHeight(x, y);

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
