#pragma once
#include "GraphicsLoader.h" 
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GraphicsLoader::GraphicsLoader(void) 
{
	currentWindow = nullptr;
}

GraphicsLoader::~GraphicsLoader(void) {}

GLuint GraphicsLoader::CreateShaderProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	// Create the shaders from the filepath
	GLuint vertShader = CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fragShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

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
		std::string programName = vertexShaderFilename + *fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}

GLuint GraphicsLoader::CreateShader(GLenum shaderType, const char* shaderName)
{
	// Read the shader files and save the source code as strings
	std::string shaderTxt = ReadShaderFile(shaderName).c_str();


	// Create the shader ID and create pointers for source code string and length
	GLuint shaderID = glCreateShader(shaderType);


	const char* shaderChars = shaderTxt.c_str();
	int shaderLength = (int)shaderTxt.size();
	
	// Populate the Shader Object (ID) and compile
	glShaderSource(shaderID, 1, &shaderChars, &shaderLength);


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

std::string GraphicsLoader::ReadShaderFile(const char* filename)
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

void GraphicsLoader::PrintErrorDetails(bool isShader, GLuint id, const char* name)
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

GLuint GraphicsLoader::CreateTexture(std::string filename)
{
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width;
	int height;
	int components;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &components, 0);

	if (data == nullptr) {
		std::cout << "Texture Not Loaded";
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
	
	//glDisable(GL_BLEND);

	return ret;
}


void GraphicsLoader::InitializeShaderPrograms()
{
	shaderPrograms.push_back(CreateShaderProgram("Resources/Shaders/ClipSpace.vert", "Resources/Shaders/TextureSpace.frag"));
	shaderPrograms.push_back(CreateShaderProgram("Resources/Shaders/ClipSpaceInstanced.vert", "Resources/Shaders/TextureSpace.frag"));
	shaderPrograms.push_back(CreateShaderProgram("Resources/Shaders/GrassSway.vert", "Resources/Shaders/Grass.frag"));
	shaderPrograms.push_back(CreateShaderProgram("Resources/Shaders/Water.vert", "Resources/Shaders/Water.frag"));
}

void GraphicsLoader::InitializeTextures()
{
	textures.push_back(CreateTexture("Resources/Prototype.png"));
	textures.push_back(CreateTexture("Resources/Tree.png"));
	textures.push_back(CreateTexture("Resources/Grass.png"));
}

GLuint GraphicsLoader::GetShaderProgram(int _ID)
{
	return shaderPrograms[_ID];
}

GLuint GraphicsLoader::GetTexture(int _ID)
{
	return textures[_ID];
}