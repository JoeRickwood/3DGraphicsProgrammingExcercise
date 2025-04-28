#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"
#include "MeshLoader.h"
#include <iostream>

void MeshLoader::Init()
{
	LoadMesh("Resources/Tent.obj");
}

void MeshLoader::LinkMeshes()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Init();
	}
}

Mesh* MeshLoader::GetMesh(int _type)
{
	return &meshes[(int)_type]; //Returns Renderable Based On The RenderableType Enum
}

void MeshLoader::LoadMesh(std::string filepath)
{
	std::vector<VertexStandard> vertices;
	tinyobj::ObjReaderConfig readerConfig;
	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(filepath, readerConfig)) 
	{
		if (!reader.Error().empty()) 
		{
			std::cout << "TINYOBJ :" << reader.Error();
		}

		exit(1);
	}

	if (!reader.Warning().empty()) 
	{
		std::cout << "TINYOBJ :" << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();


	//Loop Through The Shapes Of The Object
	for (size_t shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++)
	{

		//Loop Through The Faces Of Each Shape
		size_t readIndexOffset = 0;
		for (size_t faceIndex = 0; faceIndex < size_t(shapes[shapeIndex].mesh.num_face_vertices.size()); faceIndex++)
		{

			//Loop Through The Vertices Of Each Face
			size_t faceVertexCount = size_t(shapes[shapeIndex].mesh.num_face_vertices[faceIndex]);
			for (size_t vertexIndex = 0; vertexIndex < faceVertexCount; vertexIndex++)
			{
				VertexStandard vertex{};

				//Load Data Into Vertex
				tinyobj::index_t tinyobjVertex = shapes[shapeIndex].mesh.indices[readIndexOffset + vertexIndex];
				vertex.position = {
					attrib.vertices[3 * size_t(tinyobjVertex.vertex_index) + 0],
					attrib.vertices[3 * size_t(tinyobjVertex.vertex_index) + 1],
					attrib.vertices[3 * size_t(tinyobjVertex.vertex_index) + 2],
				};

				if (tinyobjVertex.texcoord_index >= 0)  //Negative Means No texture Coord Data
				{
					vertex.texcoord = {
						attrib.texcoords[2 * size_t(tinyobjVertex.texcoord_index) + 0],
						attrib.texcoords[2 * size_t(tinyobjVertex.texcoord_index) + 1]
					};
				}

				if (tinyobjVertex.normal_index >= 0) //Negative means No Normal Data
				{
					vertex.normal = {
						attrib.normals[3 * size_t(tinyobjVertex.normal_index) + 0],
						attrib.normals[3 * size_t(tinyobjVertex.normal_index) + 1],
						attrib.normals[3 * size_t(tinyobjVertex.normal_index) + 2]
					};
				}

				//Push vertex into list once filled with data
				vertices.push_back(vertex);
			}
			readIndexOffset += faceVertexCount; //Increment the readIndexOffset
		}
	}

	meshes.push_back(Mesh(vertices));
}

MeshLoader::MeshLoader() 
{

}

MeshLoader::~MeshLoader()
{

}
