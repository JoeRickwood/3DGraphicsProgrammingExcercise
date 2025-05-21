#include "Renderer.h"
#include "Time.h"
#include "Scene.h"

Renderer::Renderer(int _type, ShaderType _shader, int _textureID, ProjectionType _projectionType, int _reflectionTexID)
{
	mesh = MeshLoader::Instance().GetMesh(_type);

	shader = _shader;

	textureID = _textureID;
	reflectionTexID = _reflectionTexID;

	projection = _projectionType;

	cameraLoc = NULL;
	timeLoc = NULL;
	mainTexLoc = NULL;
	mainTextureTilingLoc = NULL;
	skyboxLoc = NULL;
}

Renderer::~Renderer()
{

}


//Uses This Function To Store The Uniform Locations Of The Shader
void Renderer::ShaderInit()
{
	GLuint prgm = GraphicsLoader::Instance().GetShaderProgram(shader);

	cameraLoc = glGetUniformLocation(prgm, "CameraPos");
	timeLoc = glGetUniformLocation(prgm, "Time");
	mainTexLoc = glGetUniformLocation(prgm, "Texture0");
	mainTextureTilingLoc = glGetUniformLocation(prgm, "Tiling");
	skyboxLoc = glGetUniformLocation(prgm, "SkyboxTex");
	reflectionTexLoc = glGetUniformLocation(prgm, "ReflectionTex");
}

void Renderer::InitializeRenderingInfo()
{
	GLuint prgm = GraphicsLoader::Instance().GetShaderProgram(shader);

	//Set The New Shader Program
	glUseProgram(prgm);

	//Pass In Uniforms
	glUniform3f(cameraLoc, Camera::Instance().cameraPosition.x, Camera::Instance().cameraPosition.y, Camera::Instance().cameraPosition.z);
	glUniform1f(timeLoc, Time::Instance().time);
	glUniform2f(mainTextureTilingLoc, textureTiling.x, textureTiling.y);

	//Set The Active Textrure Slot
	glActiveTexture(GL_TEXTURE0);
	///Set The Texture Mode To Repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Bind Texture To Opengl
	glBindTexture(GL_TEXTURE_2D, GraphicsLoader::Instance().GetTexture(textureID));
	//Pass It Into The Uniform
	glUniform1i(mainTexLoc, 0);

	//Now The Cubemap
	glActiveTexture(GL_TEXTURE1);
	Skybox* skybox = Scene::Current().FindObject("Skybox")->GetComponent<Skybox>();
	//Bind Texture To Opengl
	glBindTexture(GL_TEXTURE_CUBE_MAP, GraphicsLoader::Instance().GetSkybox(skybox->skyboxTexID));
	//Pass It Into Uniform
	glUniform1i(skyboxLoc, 1);


	//Only Send In Reflection Map If It Exists On The Renderer
	if (reflectionTexID != -1) 
	{
		//Set The Reflection Map Now
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GraphicsLoader::Instance().GetTexture(reflectionTexID));
		glUniform1i(reflectionTexLoc, 2);
	}

	parent->ShaderUpdate();
}

void Renderer::Render()
{

}

void Renderer::Update()
{
}


const Bounds Renderer::GetWorldBounds()
{
	float x = parent->position.x;
	float y = -parent->position.y;

	Bounds ret = Bounds((-0.5f * parent->scale.x) + x, (-0.5f * parent->scale.y) + y, (0.5f * parent->scale.x) + x, (0.5f * parent->scale.y) + y);
	return ret;
}
