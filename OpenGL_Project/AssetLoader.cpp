#pragma once
#include "AssetLoader.h" 
#include <iostream>
#include <filesystem>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Mesh::Mesh()
{
	initialized = false;

	VAO = NULL;
}

Mesh::Mesh(std::vector<VertexStandard> _data)
{
	data = _data;

	VAO = NULL;

	initialized = false;
}


Mesh::~Mesh()
{
}

void Mesh::Init()
{
	if (initialized)
	{
		return;
	}

	initialized = true;

	//Generate The VAO 
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generate The VBO 
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(VertexStandard), data.data(), GL_STATIC_DRAW);

	//Set Vertex Attribute Info, This Is How To Interpret The Vertex Data
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexStandard),
		(GLvoid*)(offsetof(VertexStandard, VertexStandard::position)));

	glEnableVertexAttribArray(0);


	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexStandard),
		(GLvoid*)(offsetof(VertexStandard, VertexStandard::texcoord)));

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexStandard),
		(GLvoid*)(offsetof(VertexStandard, VertexStandard::normal)));

	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}



AssetLoader::AssetLoader(void) 
{
	currentWindow = nullptr;
}

AssetLoader::~AssetLoader(void) {}

void AssetLoader::CreateShaderProgram(const char* _filename, std::string _shaderKey)
{
	// Create the shaders from the filepath
	GLuint vertShader = CreateShader(GL_VERTEX_SHADER, _filename);
	GLuint fragShader = CreateShader(GL_FRAGMENT_SHADER, _filename);

	// Create the program handle, attach the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader); //Attach Vertex Shader To The Program
	glAttachShader(program, fragShader); //Attach Fragment Shader To The Program

	glLinkProgram(program);

	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = _filename;
		PrintErrorDetails(false, program, programName.c_str());
		return;
	}

	AssetLoader::Instance().shaderPrograms.emplace(_shaderKey, program);
}

GLuint AssetLoader::CreateShader(GLenum shaderType, const char* shaderName)
{
	// Read the shader files and save the source code as strings
	std::string shaderTxt = ReadShaderFile(shaderName);
	const char* define = "";

	if (shaderType == GL_VERTEX_SHADER) 
	{
		 define = "#version 460 core\n#define COMPILING_VS\n";
	}
	else if(shaderType == GL_FRAGMENT_SHADER)
	{
		define = "#version 460 core\n#define COMPILING_FS\n";
	}

	const char* sources[] =
	{
		define,
		shaderTxt.c_str()
	};


	// Create the shader ID and create pointers for source code string and length
	GLuint shaderID = glCreateShader(shaderType);


	const char* shaderChars = shaderTxt.c_str();
	int shaderLength = (int)shaderTxt.size();
	
	// Populate the Shader Object (ID) and compile
	glShaderSource(shaderID, 2, sources, nullptr);
	glCompileShader(shaderID);

	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails(true, shaderID, shaderName);
		return 0;
	}

	return shaderID;
}

std::string AssetLoader::ReadShaderFile(const char* filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

void AssetLoader::PrintErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(isShader == true) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);
	std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << name << std::endl;
	std::cout << &log[0] << std::endl;
}

void AssetLoader::CreateTexture(std::string filename, std::string textureKey)
{
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width;
	int height;
	int components;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &components, 0);

	if (data == nullptr) {
		std::cout << "Texture Not Loaded : " << filename << "\n";
	}

	GLuint ret = NULL;

	//Create And Bind New Texture
	glGenTextures(1, &ret);
	glBindTexture(GL_TEXTURE_2D, ret);

	data = stbi_load(filename.c_str(), &width, &height, &components, 0);

	//Check If Is RGBA Or just RGB
	GLint LoadedComponents = (components == 4) ? GL_RGBA : GL_RGB;

	//Populate Texture Wuth IMage Data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, width, height, 0,
				LoadedComponents, GL_UNSIGNED_BYTE, data);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Generate Mipmaps, Free Memory And Unbind texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	AssetLoader::Instance().textures.emplace(textureKey, ret);
}

void AssetLoader::CreateSkybox(std::string _filepaths[6], std::string _skyboxKey)
{
	GLuint skybox = NULL;

	glGenTextures(1, &skybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	stbi_set_flip_vertically_on_load(false);

	int imageWidth;
	int imageHeight;
	int imageComponents;

	for (int i = 0; i < 6; ++i)
	{
		unsigned char* imageData = stbi_load(_filepaths[i].c_str(), &imageWidth, &imageHeight, &imageComponents, 0);

		GLint loadedComponents = (imageComponents == 4) ? GL_RGBA : GL_RGB;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, loadedComponents, imageWidth, imageHeight, 0, loadedComponents, GL_UNSIGNED_BYTE, imageData);
	
		stbi_image_free(imageData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	AssetLoader::Instance().skyboxes.emplace(_skyboxKey, skybox);
}

void AssetLoader::LoadMesh(std::string _filepath, std::string _meshKey)
{
	std::vector<VertexStandard> vertices;
	tinyobj::ObjReaderConfig readerConfig;
	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(_filepath, readerConfig))
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
	for (size_t shapeIndex = 0; shapeIndex < shapes.size(); ++shapeIndex)
	{

		//Loop Through The Faces Of Each Shape
		size_t readIndexOffset = 0;
		for (size_t faceIndex = 0; faceIndex < size_t(shapes[shapeIndex].mesh.num_face_vertices.size()); ++faceIndex)
		{

			//Loop Through The Vertices Of Each Face
			size_t faceVertexCount = size_t(shapes[shapeIndex].mesh.num_face_vertices[faceIndex]);
			for (size_t vertexIndex = 0; vertexIndex < faceVertexCount; ++vertexIndex)
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

	Mesh* mesh = new Mesh(vertices);
	mesh->Init();

	AssetLoader::Instance().meshes.emplace(_meshKey, mesh);
}

GLuint AssetLoader::GetShaderProgram(std::string _key)
{
	return shaderPrograms[_key];
}

GLuint AssetLoader::GetTexture(std::string _key)
{
	return textures[_key];
}

GLuint AssetLoader::GetSkybox(std::string _key)
{
	return skyboxes[_key];
}

Mesh* AssetLoader::GetMesh(std::string _meshKey)
{
	return meshes[_meshKey];
}

void AssetLoader::AddTexture(GLuint _tex, std::string _key)
{
	textures.emplace(_key, _tex);
}

void AssetLoader::LoadAssets(const char* folderPath)
{
	std::string path = folderPath;

	for (const auto& file : std::filesystem::recursive_directory_iterator(path)) 
	{
		std::string extension = std::filesystem::path(file).extension().string();
		std::filesystem::path name = std::filesystem::path(file).filename();

		extension.erase(std::remove_if(extension.begin(), extension.end(), ::isspace), extension.end());

		for (int i = 0; i < std::size(supportedImageFileExtensions); i++)
		{
			//std::cout << supportedImageFileExtensions[i] << " : " << extension << "\n";

			if (supportedImageFileExtensions[i] == extension)
			{
				name.replace_extension("");
				CreateTexture(std::filesystem::path(file).string(), name.string());
				std::cout << "Created Texture : " << name << "\n";
			}
		}

		for (int i = 0; i < std::size(supportedModelFileExtensions); i++)
		{
			if (supportedModelFileExtensions[i] == extension)
			{
				name.replace_extension("");
				LoadMesh(std::filesystem::path(file).string(), name.string());
				std::cout << "Created Model : " << name << "\n";
			}
		}

		for (int i = 0; i < std::size(supportedShaderFileExtensions); i++)
		{
			if (supportedShaderFileExtensions[i] == extension)
			{
				std::filesystem::path fullPath = std::filesystem::path(file);
				name.replace_extension("");


				std::cout << "Loading Shader File: " << fullPath << ", Key: " << name.string() << "\n";


				CreateShaderProgram(std::filesystem::path(file).string().c_str(), name.string());
				std::cout << "Created Shader : " << name << "\n";
			}
		}

	}



}
