#include <string>
#include <vector>
#include <map>
#include<fstream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

enum ValuePassInType 
{
	FLOAT,
	VECTOR2,
	VECTOR3,
	VECTOR4,

	MATRIX2X2,
	MATRIX3X3,
	MATRIX4X4
};

enum TextureType
{
	TEXTURE_2D = GL_TEXTURE_2D,
	CUBEMAP = GL_TEXTURE_CUBE_MAP
};

enum TextureTilingType
{
	CLAMP_EDGES,
	REPEAT,
	CLAMP_BORDER
};

enum RenderType
{
	RENDER_FRONT = GL_BACK,
	RENDER_BACK = GL_FRONT,
	RENDER_NONE = GL_FRONT_AND_BACK,
	RENDER_BOTH = GL_CULL_FACE
};

//Structs Used To Pass Uniforms Into Shader
struct TexturePass
{
public:
	std::string locationName;
	std::string textureKey;
	TextureType type;
	TextureTilingType tilingType;

	TexturePass(std::string _location, std::string _texKey, TextureType _type, TextureTilingType _tilingType)
	{
		locationName = _location;

		textureKey = _texKey;
		type = _type;
		tilingType = _tilingType;
	}

	~TexturePass()
	{

	}
};

struct ValuePass
{
public:
	std::string name;
	glm::vec4 value;
	ValuePassInType type;

	//Constructors For All Value Types
	ValuePass(std::string _name, glm::vec4 _value)
	{
		name = _name;
		value = _value;
		type = VECTOR4;
	}

	ValuePass(std::string _name, glm::vec3 _value)
	{
		name = _name;
		value = glm::vec4(_value.x, _value.y, _value.z, 0.0f);
		type = VECTOR3;
	}

	ValuePass(std::string _name, glm::vec2 _value)
	{
		name = _name;
		value = glm::vec4(_value.x, _value.y, 0.0f, 0.0f);
		type = VECTOR2;
	}

	ValuePass(std::string _name, float _value)
	{
		name = _name;
		value = glm::vec4(_value, 0.0f, 0.0f, 0.0f);
		type = FLOAT;
	}

	~ValuePass()
	{

	}
};

struct MatrixPass 
{
public:
	std::string name;
	glm::mat4 value;
	ValuePassInType type;

	MatrixPass(std::string _name, glm::mat4 _value) 
	{
		name = _name;
		value = _value;
		type = MATRIX4X4;
	}

	MatrixPass(std::string _name, glm::mat3 _value)
	{
		name = _name;
		value = glm::mat4(_value);
		type = MATRIX3X3;
	}

	MatrixPass(std::string _name, glm::mat2 _value)
	{
		name = _name;
		value = glm::mat4(_value);
		type = MATRIX2X2;
	}

	~MatrixPass() 
	{ 
	
	}

};


/// <summary>
/// Material Class Is Used By Renderer To Send Information To The GPU For Shaders
/// The Purpose For The Material Class Is To Ensure There Is Not A Significant Amount Of Data Being Held In Each Renderer
/// The Materials Have Certain Specific Values Which Other Components In Code Look Through To Update Automatically
/// </summary>

class Material
{
protected:
	//Name Used For Identification
	std::string name;
	//Shader Key To Receive GLuint Shader Program From The Asset Loader
	std::string shaderKey;

	RenderType renderType;

	//List Of Texture Passes To Send To The Shader Program
	std::vector<TexturePass> texturePasses;
	std::vector<ValuePass> valuePasses;
	std::vector<MatrixPass> matrixPasses;



	void SetUniforms(GLuint _program);

public:
	Material();
	~Material();

	//Binds Shader Program And Passes In Textures, Values And Matrices Into Shader
	//Returns GLuint Shader ID Held In The AssetLoader
	GLuint Bind();
	void Unbind();

	//Emplaces Texture Onto Texture Pass Stack
	void AddTexturePass(TexturePass _value);
	//Returns A Texture Pass With The Corresponding Texture Key, If None Exist In This Material, Returns Nullptr
	TexturePass* GetTexturePass(std::string _key);

	//Emplaces A Value Onto The Value Pass Stack
	void AddValuePass(ValuePass _value);
	//Returns A Value Pass With The Corresponding NameKey, If None Exist In This Material, Returns Nullptr
	ValuePass* GetValuePass(std::string _key);

	//Emplaces A Matrix Onto The Matric Pass Stack
	void AddMatrixPass(MatrixPass _value);
	//Returns A Matrix Pass With The Corresponding Name Key, if None Exist In This Material, Returns Nullptr
	MatrixPass* GetMatrixPass(std::string _key);


	void SetName(std::string _name);
	std::string GetName() const;

	void SetShader(std::string _shader);
	std::string GetShader() const;

	void SetRenderType(RenderType _renderType);
	RenderType GetRenderType() const;

};