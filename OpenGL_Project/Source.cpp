#include "Scene.h"
#include "Physics.h"

//Window Width And Height Of The Application (Pixels)
static int windowWidth = 800;
static int windowHeight = 800;

//Global Variables To Use In Main, Update + Render Functions
GLFWwindow* Window;

Scene scene;

//Forward Declare Functions For Later
void InitialSetup();
void Update();
void Render();
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height);

int main() 
{
	//We Initialize The Renderable Loader BEFORE We Set Our Rendereable Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (Quads)
	RenderableLoader::Instance().Init();

	ObjectInstance* player = new ObjectInstance("Player", glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.f), glm::vec3(0.5f, 0.5f, 0.5f));
	player->AddComponent<Renderer>(RenderableType::Quad, ShaderType::Texture, 0);
	Animator* anim = player->AddComponent<Animator>(4.f);
	anim->AddAnimation(Animation(0, 8, 1, 1));
	anim->AddAnimation(Animation(0, 8, 1, 8));
	player->AddComponent<Collider>(glm::vec3(0.5f, 1.0f, 1.0f));
	player->AddComponent<PhysicsObject>();
	player->AddComponent<Player>();


	ObjectInstance* floor = new ObjectInstance("Floor", glm::vec3(-50.f, -11.f, 0.0f));
	floor->AddComponent<MapGenerator>(glm::vec2(0.4f, 0.35f), glm::vec2(100, 10));

	scene.AddObject(player);
	scene.AddObject(floor);

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

		Physics::Instance().ResolveCollisions();

		Render();
	}

	glfwTerminate();
	return 0;
}

//Sets Up Objects + Other GLFW Parameters
void InitialSetup() 
{
	glClearColor(0.6f, 0.6f, 1.0f, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);

	GraphicsLoader::Instance().InitializeShaderPrograms(); // Generates The Shader Programs To Be Used By Renderables
	GraphicsLoader::Instance().InitializeTextures(); //Generates The Textures Used

	Time::Instance().Init();
	Input::Instance().Init();

	glfwSetWindowSizeCallback(Window, (GLFWwindowsizefun)OnWindowResized);
}

//Update Is Called Once Every Frame BEFORE Render
void Update() 
{
	scene.Update();

	glfwPollEvents();
}

//Render Is Called After Update And Draws Objects To The Screen
void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	scene.Render();

	glfwSwapBuffers(Window);
}

//On Window Resized Callback Links To The glfwWindowSizefun
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height) 
{
	glViewport(0, 0, _Width, _Height);	
}