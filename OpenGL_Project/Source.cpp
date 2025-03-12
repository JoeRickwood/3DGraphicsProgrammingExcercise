#include "ObjectInstance.h"
#include "Components.h"


//Window Width And Height Of The Application (Pixels)
static int windowWidth = 800;
static int windowHeight = 800;

//Global Variables To Use In Main, Update + Render Functions
GLFWwindow* Window;

std::vector<ObjectInstance*> objects;

//Forward Declare Functions For Later
void InitialSetup();
void Update();
void Render();
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height);

void TestButton() {
	std::cout << "Hello";
}

int main() 
{
	//We Initialize The Renderable Loader BEFORE We Set Our Rendereable Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (Quads)
	RenderableLoader::Instance().Init();

	objects =
	{
		new ObjectInstance("Hexagon 1")
	};

	objects[0]->AddComponent<Renderer>(RenderableType::Quad, ShaderType::Texture);
	Animator* anim = objects[0]->AddComponent<Animator>(4.f);
	anim->AddAnimation(Animation(0, 8, 1, 1));
	anim->AddAnimation(Animation(0, 8, 1, 8));
	objects[0]->AddComponent<Player>();

	//Initialize GLFW And setting the version to 4.6
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create Window
	Window = glfwCreateWindow(windowWidth, windowHeight, "OPEN GL EXCERCISE", NULL, NULL);

	if(Window == NULL) 
	{
		std::cout << "Error Creating Window, GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	//Make The Window The Current Context Of GLFW
	glfwMakeContextCurrent(Window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	//Setup All Objects In Project
	InitialSetup();
	RenderableLoader::Instance().LinkRenderables();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(Window) == false) 
	{
		Time::Instance().Update();

		Update();

		Render();
	}

	glfwTerminate();
	return 0;
}

//Sets Up Objects + Other GLFW Parameters
void InitialSetup() 
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);

	GraphicsLoader::Instance().InitializeShaderPrograms(); // Generates The Shader Programs To Be Used By Renderables
	GraphicsLoader::Instance().InitializeTextures(); //Generates The Textures Used

	Input::Instance().Init();

	glfwSetWindowSizeCallback(Window, (GLFWwindowsizefun)OnWindowResized);
}

//Update Is Called Once Every Frame BEFORE Render
void Update() 
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}

	glfwPollEvents();
}

//Render Is Called After Update And Draws Objects To The Screen
void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	glfwSwapBuffers(Window);
}

//On Window Resized Callback Links To The glfwWindowSizefun
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height) 
{
	glViewport(0, 0, _Width, _Height);	
}