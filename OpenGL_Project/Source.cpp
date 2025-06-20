#include <iostream>

#include "Scene.h"
#include "Time.h"
#include "PerlinNoise.h"

#include "RenderingPipeline.h"

//On Window Resized Callback Links To The glfwWindowSizefun
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height)
{
	glViewport(0, 0, _Width, _Height);

	AssetLoader::Instance().windowSize.x = (float)_Width;
	AssetLoader::Instance().windowSize.y = (float)_Height;
}

//Sets Up Objects + Other GLFW Parameters
void InitialSetup() 
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
void Update() 
{
	Scene::Current().Update();

	glfwPollEvents();
}

//Creates All Objects In The Scene
void LoadScene()
{
	ObjectInstance* skybox = new ObjectInstance("Skybox", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f));
	skybox->AddComponent<Skybox>("Skybox", "MainSkybox");
	skybox->AddComponent<DirectionalLight>(glm::vec3(0.8f, -1.0f, 0.4f), glm::vec3(1.5f, 1.5f, 1.5f), 1.0f); 

	ObjectInstance* cam = new ObjectInstance("Camera", glm::vec3(0.f, 0.f, 0.f));
	cam->AddComponent<PlayerController>(0.003f, 15);

	//Object Which Shows What A Regular Object With Animation Looks Like
	/*ObjectInstance* ground = new ObjectInstance("Ground", glm::vec3(0.0f, -10.f, 0.0f), glm::vec3(0.f), glm::vec3(250.f, 10.f, 250.f));
	auto groundRenderer = ground->AddComponent<DefaultRenderer>("Default", ProjectionType::Perspective);
	groundRenderer->SetMesh(AssetLoader::Instance().GetMesh("Cube"));
	groundRenderer->AddTexturePass("Texture0", "Prototype", Texture2D, TilingType::Repeat);
	groundRenderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, TilingType::ClampEdges);
	groundRenderer->SetTextureTiling(glm::vec2(25, 25));  */

	/* ObjectInstance* instancedTrees = new ObjectInstance("Trees");
	auto treesRenderer = instancedTrees->AddComponent<InstancedRenderer>("Default", ProjectionType::Perspective);
	treesRenderer->SetMesh(AssetLoader::Instance().GetMesh("Tree"));
	treesRenderer->AddTexturePass("Texture0", "Tree", Texture2D, TilingType::Repeat);
	treesRenderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, TilingType::ClampEdges);
	treesRenderer->SetRenderType(RenderBoth); */

	/* ObjectInstance* instancedGrass = new ObjectInstance("Grass");
	auto grassRenderer = instancedGrass->AddComponent<InstancedRenderer>("Grass", ProjectionType::Perspective);
	grassRenderer->SetMesh(AssetLoader::Instance().GetMesh("Grass"));
	grassRenderer->AddTexturePass("Texture0", "Grass", Texture2D, TilingType::Repeat);
	grassRenderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, TilingType::ClampEdges);
	grassRenderer->SetShadowRendering(false);
	grassRenderer->SetRenderType(RenderFront); */

	ObjectInstance* cube1 = new ObjectInstance("Cube1", glm::vec3(-2.f, -1.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	auto cube1Renderer = cube1->AddComponent<DefaultRenderer>("Default", ProjectionType::Perspective);
	cube1Renderer->SetMesh(AssetLoader::Instance().GetMesh("Cube"));
	cube1Renderer->AddTexturePass("Texture0", "Prototype", Texture2D, TilingType::ClampEdges);
	cube1Renderer->SetShadowRendering(true);
	cube1Renderer->SetRenderType(RenderFront);

	ObjectInstance* cube2 = new ObjectInstance("Cube2", glm::vec3(2.f, -1.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	auto cube2Renderer = cube2->AddComponent<DefaultRenderer>("Default", ProjectionType::Perspective);
	cube2Renderer->SetMesh(AssetLoader::Instance().GetMesh("Cube"));
	cube2Renderer->AddTexturePass("Texture0", "Prototype", Texture2D, TilingType::ClampEdges);
	cube2Renderer->SetShadowRendering(true);
	cube2Renderer->SetRenderType(RenderFront);

	ObjectInstance* cube3 = new ObjectInstance("Cube3", glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(10.f, 0.1f, 10.f));
	auto cube3Renderer = cube3->AddComponent<DefaultRenderer>("Default", ProjectionType::Perspective);
	cube3Renderer->SetMesh(AssetLoader::Instance().GetMesh("Cube"));
	cube3Renderer->AddTexturePass("Texture0", "Prototype", Texture2D, TilingType::Repeat);
	cube3Renderer->SetShadowRendering(true);
	cube3Renderer->SetTextureTiling(glm::vec2(15, 15));
	cube3Renderer->SetRenderType(RenderFront);

	ObjectInstance* UIObjectTest = new ObjectInstance("UIObjectTest", glm::vec3(0.f, 0.f, -1.f), glm::vec3 (0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	auto UIRenderer = UIObjectTest->AddComponent<DefaultRenderer>("DefaultGlyph", ProjectionType::Perspective);
	UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
	UIRenderer->AddTexturePass("Texture0", "Glyph", Texture2D, TilingType::Repeat);
	UIRenderer->SetShadowRendering(false);
	UIRenderer->SetRenderType(RenderFront);

	Scene::Current().SetAmbientLightStength(0.2f);
	Scene::Current().SetAmbientLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

	/*for (int i = 0; i < 5000; ++i)
	{
		glm::vec3 pos = glm::vec3(ValueNoise_2D(i + 2312, i - 23712) * 500.f, 0.f, ValueNoise_2D(i - 232312, i + 23712) * 500.f);
		glm::vec3 rot = glm::vec3(0.0f, rand() % 360, 0.0f);
		glm::vec3 scale = glm::vec3(0.3f, 0.3f, 0.3f) * (((float)(rand() % 300) / 100.f) + 0.5f);

		treesRenderer->AddInstance(pos, rot, scale);
	} */

	/*for (int i = 0; i < 200000; ++i)
	{
		glm::vec3 pos = glm::vec3(ValueNoise_2D(i + 2312, i - 23712) * 500.f, 0.f, ValueNoise_2D(i - 232312, i + 23712) * 500.f);
		glm::vec3 rot = glm::vec3(0.0f, rand() % 360, 0.0f);
		glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.5f) * (((float)(rand() % 100) / 100.f) + 0.5f);

		grassRenderer->AddInstance(pos, rot, scale);
	} */

	//treesRenderer->InitVBO();
	//grassRenderer->InitVBO();
	//groundRenderer->InitVBO();
	//UIRenderer->InitVBO();

	//cube1Renderer->InitVBO();
	//cube2Renderer->InitVBO();

	std::string skyboxPaths[6] =
	{
		"Resources/Skybox/Front.png",
		"Resources/Skybox/Back.png",
		"Resources/Skybox/Top.png",
		"Resources/Skybox/Bottom.png",
		"Resources/Skybox/Right.png",
		"Resources/Skybox/Left.png"
	};

	AssetLoader::CreateSkybox(skyboxPaths, "MainSkybox"); 
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


	RenderingPipeline::InitShadowRendering();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(AssetLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();

		Update();

		RenderingPipeline::ShadowPass();

		RenderingPipeline::Render("DepthTesting");
	}

	glfwTerminate();
	return 0;
}