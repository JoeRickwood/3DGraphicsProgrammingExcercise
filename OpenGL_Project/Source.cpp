#include <iostream>

#include "Scene.h"
#include "Time.h"

#include "RenderingPipeline.h"

//On Window Resized Callback Links To The glfwWindowSizefun
static void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height)
{
	glViewport(0, 0, _Width, _Height);

	AssetLoader::Instance().SetWindowSized(glm::vec2((float)_Width, (float)_Height));
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
	glm::vec2 windowSize = AssetLoader::Instance().GetWindowSize();
	AssetLoader::Instance().currentWindow = glfwCreateWindow((int)windowSize.x, (int)windowSize.y, "OPEN GL EXCERCISE", NULL, NULL);

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

	glViewport(0, 0, (GLsizei)windowSize.x, (GLsizei)windowSize.y);
	glfwSetWindowSizeCallback(AssetLoader::Instance().currentWindow, (GLFWwindowsizefun)OnWindowResized);


	Time::Instance().Init();


	//We Initialize The Renderable Loader BEFORE We Set Our Object Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (None)
	AssetLoader::Instance().LoadAssets("Resources");
}

//Creates All Objects In The Scene
static void LoadScene()
{
	//Set The Scenes Current Ambient Light Color And Strength
	Scene::Current().SetAmbientLightStength(1.0f);
	Scene::Current().SetAmbientLightColor(glm::vec3(1.0f, 1.0f, 1.0f));


	//Regular Sprite Object Set-Up
	{
		ObjectInstance* backgroundTest = new ObjectInstance("Test", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f, 1.f, 1.f));
		auto backgroundTestRenderer = backgroundTest->AddComponent<SpriteRenderer>("DefaultSprite", ProjectionType::Orthographic);
		backgroundTestRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		backgroundTestRenderer->AddTexturePass("Texture0", "Joe", Texture2D, Repeat);
		backgroundTestRenderer->SetRenderType(RenderBoth);
	}

	//Button Set-Up
	{
		ObjectInstance* UIButtonObject = new ObjectInstance("UIButtonObjectTest", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(200.0f, 100.0f, 1.0f));
		auto UIButtonRenderer = UIButtonObject->AddComponent<SpriteRenderer>("DefaultUI", ProjectionType::Screen_Orthographic);
		UIButtonRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIButtonRenderer->SetRenderType(RenderBoth);
		auto UIButton = UIButtonObject->AddComponent<Button>();
		auto func = []()
			{
				std::cout << "Working!!!!";
			};
		UIButton->AddListener(func);


		ObjectInstance* UIObjectTest = new ObjectInstance("UIObjectTest", glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::Screen_Orthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetFont("AldotheApache");
		UIRenderer->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		UIRenderer->SetText("Font Text Rendering Test");
		UIRenderer->SetRenderType(RenderBoth);
		UIObjectTest->SetParent(UIButtonObject);
	}


	Camera::Instance().SetCameraPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	Camera::Instance().SetCameraLookDirection(glm::vec3(0.0f, 0.0f, 1.0f));
	Camera::Instance().SetCameraUpDirection(glm::vec3(0.0f, 1.0f, 0.0f));
	Camera::Instance().SetOrthographicSize(10.0f);
}

int main()
{
	//Setup All Objects In Project
	InitialSetup();

	//Load The Scenes Objects
	LoadScene();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(AssetLoader::Instance().currentWindow) == false)
	{
		//Calculate The Camera View And Projection Matrices WHich Objects Will Use Later To Render Properly
		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();


		//Update All ObjectInstance Logic In The Scene
		Time::Instance().Update();
		Scene::Current().Update();

		//Poll To See If Any glfw Window Events Have Occured
		glfwPollEvents();

		//Render All Objects In The Scene
		RenderingPipeline::Render();
	}

	glfwTerminate();
	return 0;
}