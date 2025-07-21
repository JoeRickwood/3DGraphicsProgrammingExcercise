#include <iostream>

#include "Scene.h"
#include "Time.h"

#include "RenderingPipeline.h"

//On Window Resized Callback Links To The glfwWindowSizefun
static void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height)
{
	glViewport(0, 0, _Width, _Height);

	AssetLoader::Instance().windowSize.x = (float)_Width;
	AssetLoader::Instance().windowSize.y = (float)_Height;
}

//Sets Up Objects + Other GLFW Parameters
static void InitialSetup() 
{
	//Initialize GLFW And setting the version to 4.6
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_SAMPLES, 4);

	//Create Window
	AssetLoader::Instance().currentWindow = glfwCreateWindow((int)AssetLoader::Instance().windowSize.x, (int)AssetLoader::Instance().windowSize.y, "OPEN GL EXCERCISE", NULL, NULL);

	if (AssetLoader::Instance().currentWindow == NULL)
	{
		std::cout << "Error Creating AssetLoader::Instance().currentWindow, GLFW Failed To Initialize, Terminating Program" << std::endl;

		glfwTerminate();
		return;
	}

	//Make The Window The Current Context Of GLFW
	glfwMakeContextCurrent(AssetLoader::Instance().currentWindow);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glfwSwapInterval(0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, (GLsizei)AssetLoader::Instance().windowSize.x, (GLsizei)AssetLoader::Instance().windowSize.y);
	glfwSetWindowSizeCallback(AssetLoader::Instance().currentWindow, (GLFWwindowsizefun)OnWindowResized);
}

//Update Is Called Once Every Frame BEFORE Render
static void Update() 
{
	Scene::Current().Update();

	glfwPollEvents();
}

//Creates All Objects In The Scene
static void LoadScene()
{
	Scene::Current().SetAmbientLightStength(0.2f);
	Scene::Current().SetAmbientLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

	{
		ObjectInstance* backgroundTest = new ObjectInstance("Test", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f, 1.f, 1.f));
		auto backgroundTestRenderer = backgroundTest->AddComponent<DefaultRenderer>("DefaultSprite", ProjectionType::Orthographic);
		backgroundTestRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		backgroundTestRenderer->AddTexturePass("Texture0", "Joe", Texture2D, Repeat);
		backgroundTestRenderer->SetRenderType(RenderBoth);
	}

	{
		ObjectInstance* backgroundTest = new ObjectInstance("Test", glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(25.f, 25.f, 25.f));
		auto backgroundTestRenderer = backgroundTest->AddComponent<DefaultRenderer>("DefaultSprite", ProjectionType::Orthographic);
		backgroundTestRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		backgroundTestRenderer->AddTexturePass("Texture0", "GroundTile", Texture2D, Repeat);
		backgroundTestRenderer->SetTextureTiling(glm::vec2(25, 25));
		backgroundTestRenderer->SetRenderType(RenderBoth);
	}

	ObjectInstance* UIObjectTest = new ObjectInstance("UIObjectTest", glm::vec3(50.f, 50.f, -1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::Orthographic);
	UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
	UIRenderer->SetFont("AldotheApache");
	UIRenderer->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	UIRenderer->SetText("Font Text Rendering Test");
	UIRenderer->SetRenderType(RenderBoth);


	Camera::Instance().SetCameraPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	Camera::Instance().SetCameraLookDirection(glm::vec3(0.0f, 0.0f, 1.0f));
	Camera::Instance().SetCameraUpDirection(glm::vec3(0.0f, 1.0f, 0.0f));
	Camera::Instance().SetOrthographicSize(10.0f);
}

int main()
{
	//Setup All Objects In Project
	InitialSetup();

	//We Initialize The Renderable Loader BEFORE We Set Our Object Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (None)
	AssetLoader::Instance().LoadAssets("Resources");

	Time::Instance().Init();

	LoadScene();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(AssetLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();

		Update();

		RenderingPipeline::Render();
	}

	glfwTerminate();
	return 0;
}