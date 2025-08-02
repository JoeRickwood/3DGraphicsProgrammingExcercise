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

	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	//Create Window
	glm::vec2 windowSize = AssetLoader::Instance().GetWindowSize();
	AssetLoader::Instance().currentWindow = glfwCreateWindow((int)windowSize.x, (int)windowSize.y, "OPEN GL EXCERCISE", NULL, NULL);
	AssetLoader::Instance().SetWindowSized(glm::vec2((int)windowSize.x, (int)windowSize.y));

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
		ObjectInstance* objectTest = new ObjectInstance("Test", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f, 1.f, 1.f));
		auto objectTestRenderer = objectTest->AddComponent<SpriteRenderer>("DefaultSprite", ProjectionType::Orthographic);
		objectTestRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		objectTestRenderer->AddTexturePass("Texture0", "Test", Texture2D, Repeat);
		objectTestRenderer->SetRenderType(RenderBoth);
		objectTestRenderer->SetShadowRendering(true);
	}

	{
		ObjectInstance* objectTest = new ObjectInstance("Test", glm::vec3(3.0f, 3.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.f, 5.f, 5.f));
		auto objectTestRenderer = objectTest->AddComponent<SpriteRenderer>("DefaultSprite", ProjectionType::Orthographic);
		objectTestRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		objectTestRenderer->AddTexturePass("Texture0", "ShadowMap", Texture2D, Repeat);
		objectTestRenderer->SetRenderType(RenderBoth);
		objectTestRenderer->SetShadowRendering(false);
		objectTestRenderer->SetTextureTiling(glm::vec2(25, 25));
	}


	/*
	//Regular Sprite Object Set-Up
	{
		ObjectInstance* backgroundTest = new ObjectInstance("Test", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.f, 20.f, 1.f));
		auto backgroundTestRenderer = backgroundTest->AddComponent<SpriteRenderer>("DefaultSprite", ProjectionType::Orthographic);
		backgroundTestRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		backgroundTestRenderer->AddTexturePass("Texture0", "GroundTile", Texture2D, Repeat);
		backgroundTestRenderer->SetRenderType(RenderBoth);
		backgroundTestRenderer->SetTextureTiling(glm::vec2(20.0f, 20.0f));
	} */
	
	//Expand For UI Creation
	/*
	//Play Button Set-Up
	{
		UIObjectInstance* UIButtonObject = new UIObjectInstance("UIButtonObjectTest", glm::vec3(-175.f, 100.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(150.0f, 75.0f, 1.0f));
		auto UIButtonRenderer = UIButtonObject->AddComponent<SpriteRenderer>("DefaultUI", ProjectionType::Screen_Orthographic);
		UIButtonRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIButtonRenderer->AddTexturePass("Texture0", "Button", Texture2D, TilingType::ClampEdges);
		UIButtonRenderer->SetRenderType(RenderBoth);
		auto UIButton = UIButtonObject->AddComponent<Button>();
		auto func = []()
			{
				std::cout << "Working!!!!";
			};
		UIButton->AddListener(func);
		UIButtonObject->SetScreenAlignment(MIDDLE, CENTER);


		ObjectInstance* UIObjectTest = new ObjectInstance("UIObjectTest", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::Screen_Orthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetFont("AldotheApache");
		UIRenderer->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		UIRenderer->SetText("Start New");
		UIRenderer->SetRenderType(RenderBoth);

		UIObjectTest->SetParent(UIButtonObject);
		UIObjectTest->SetPosition(glm::vec3(0, 0, 0));
	}


	//Play Button Set-Up
	{
		UIObjectInstance* UIButtonObject = new UIObjectInstance("UIButtonObjectTest", glm::vec3(175.f, 100.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(150.0f, 75.0f, 1.0f));
		auto UIButtonRenderer = UIButtonObject->AddComponent<SpriteRenderer>("DefaultUI", ProjectionType::Screen_Orthographic);
		UIButtonRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIButtonRenderer->AddTexturePass("Texture0", "Button", Texture2D, TilingType::ClampEdges);
		UIButtonRenderer->SetRenderType(RenderBoth);
		auto UIButton = UIButtonObject->AddComponent<Button>();
		auto func = []()
			{
				std::cout << "Working!!!!";
			};
		UIButton->AddListener(func);
		UIButtonObject->SetScreenAlignment(MIDDLE, CENTER);


		ObjectInstance* UIObjectTest = new ObjectInstance("UIObjectTest", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::Screen_Orthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetFont("AldotheApache");
		UIRenderer->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		UIRenderer->SetText("Continue");
		UIRenderer->SetRenderType(RenderBoth);


		UIObjectTest->SetParent(UIButtonObject);
		UIObjectTest->SetPosition(glm::vec3(0, 0, 0));
	}


	//Play Button Set-Up
	{
		UIObjectInstance* UIButtonObject = new UIObjectInstance("UIButtonObjectTest", glm::vec3(0.f, -100.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(325.0f, 75.0f, 1.0f));
		auto UIButtonRenderer = UIButtonObject->AddComponent<SpriteRenderer>("DefaultUI", ProjectionType::Screen_Orthographic);
		UIButtonRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIButtonRenderer->AddTexturePass("Texture0", "Button", Texture2D, TilingType::ClampEdges);
		UIButtonRenderer->SetRenderType(RenderBoth);
		auto UIButton = UIButtonObject->AddComponent<Button>();
		auto func = []()
			{
				std::cout << "Working!!!!";
			};
		UIButton->AddListener(func);
		UIButtonObject->SetScreenAlignment(MIDDLE, CENTER);


		ObjectInstance* UIObjectTest = new ObjectInstance("UIObjectTest", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::Screen_Orthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetFont("AldotheApache");
		UIRenderer->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		UIRenderer->SetText("Settings");
		UIRenderer->SetRenderType(RenderBoth);


		UIObjectTest->SetParent(UIButtonObject);
		UIObjectTest->SetPosition(glm::vec3(0, 0, 0));
	}

	{
		UIObjectInstance* UIObjectTest = new UIObjectInstance("UIObjectTest", glm::vec3(0.f, 350.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(3.f, 3.f, 3.f));
		auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::Screen_Orthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetFont("AldotheApache");
		UIRenderer->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		UIRenderer->SetText("Game Name");
		UIRenderer->SetRenderType(RenderBoth);
		UIObjectTest->SetScreenAlignment(MIDDLE, CENTER);
	}

	*/

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


	RenderingPipeline::InitializeShadowMapping();

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

		RenderingPipeline::ShadowPass();

		//Render All Objects In The Scene
		RenderingPipeline::Render();
	}

	glfwTerminate();
	return 0;
}