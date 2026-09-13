#include <iostream>

#include "Scene.h"
#include "Time.h"
#include "MathFunctions.h"

#include "RenderingPipeline.h"

//On Window Resized Callback Links To The glfwWindowSizefun
static void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height)
{
	glViewport(0, 0, _Width, _Height);

	AssetLoader::Instance().windowSize.x = (float)_Width;
	AssetLoader::Instance().windowSize.y = (float)_Height;


	RenderingPipeline::UpdateFramebufferTexture();
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

	//For Particle Systems
	glEnable(GL_PROGRAM_POINT_SIZE);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glfwSwapInterval(0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, (GLsizei)AssetLoader::Instance().windowSize.x, (GLsizei)AssetLoader::Instance().windowSize.y);
	glfwSetWindowSizeCallback(AssetLoader::Instance().currentWindow, (GLFWwindowsizefun)OnWindowResized);
}


//Creates All Objects In The Scene1
static void LoadScene1()
{
	Scene::Current().ChangeScene(1);

	Scene::Current().SetAmbientLightStength(0.4f);
	Scene::Current().SetAmbientLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

	//Create Skybox For Rendering
	ObjectInstance* skybox = new ObjectInstance("Skybox", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f));
	skybox->AddComponent<Skybox>("Skybox", "MainSkybox");
	skybox->AddComponent<DirectionalLight>(glm::vec3(0.8f, -0.4f, 0.4f), glm::vec3(1.5f, 1.5f, 1.5f), 1.0f); 

	//Create Player Move-Around Camera
	ObjectInstance* cam = new ObjectInstance("Camera", glm::vec3(0.f, 0.f, 0.f));
	cam->AddComponent<PlayerController>(0.003f, 100);

	//Create Terrain Object
	ObjectInstance* ground = new ObjectInstance("Terrain", glm::vec3(200.f, 0.f, 200.f), glm::vec3(-90, 0, 0), glm::vec3(400, 400, 400));
	auto groundRenderer = ground->AddComponent<DefaultRenderer>("Default", ProjectionType::Perspective);
	//Send All Used Texture Passes Into Terrain Shader For Height / Normal Blending
	groundRenderer->AddTexturePass("Texture0", "Grass", Texture2D, TilingType::Repeat);
	groundRenderer->SetTextureTiling(glm::vec2(40, 40));
	groundRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
	groundRenderer->SetRenderType(RenderBoth);

	ObjectInstance* pumpjacks = new ObjectInstance("Pumpjacks", glm::vec3(0.f, 0.f, 0.f), glm::vec3(00, 0, 0), glm::vec3(1, 1, 1));
	auto instancedPumpjacks = pumpjacks->AddComponent<InstancedRenderer>("Default", ProjectionType::Perspective);
	instancedPumpjacks->SetMesh(AssetLoader::Instance().GetMesh("PumpJack"));
	instancedPumpjacks->AddTexturePass("Texture0", "PumpJack", Texture2D, TilingType::ClampEdges);

	float spacing = 8.0f;
	for (int x = 0; x < 50; x++)
	{
		for (int y = 0; y < 50; y++)
		{
			glm::vec3 pos = glm::vec3(x * spacing, 0, y * spacing);

			instancedPumpjacks->AddInstance(pos, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
		}
	}

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		glm::vec3 pos = glm::vec3(rand() % 400, 2, rand() % 400);
		glm::vec3 col = glm::vec3((rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f);

		ObjectInstance* light = new ObjectInstance("Light", pos, glm::vec3(), glm::vec3(4, 4, 4));
		light->AddComponent<PointLight>(col, 0.5);
		auto lightRenderer = light->AddComponent<DefaultRenderer>("LightUnlit", ProjectionType::Perspective);
		lightRenderer->SetColor(glm::vec4(col, 1));
		lightRenderer->SetMesh(AssetLoader::Instance().GetMesh("Sphere"));
	}

	//Create A Little Text UI To Show What Scene The User Is Currently Viewing
	UIObjectInstance* SceneText = new UIObjectInstance("Scene Indicator", glm::vec3(50.f, 50.f, -1.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(2.f, 2.f, 2.f));
	auto SceneTextRenderer = SceneText->AddComponent<TextRenderer>("DefaultText", ProjectionType::ScreenOrthographic);
	SceneTextRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
	SceneTextRenderer->SetFont("AldotheApache");
	SceneTextRenderer->SetColor(glm::vec3(1.f, 1.f, 1.f));
	SceneTextRenderer->SetText("Terrain Scene Test 1");
	SceneTextRenderer->SetRenderType(RenderBoth);
	SceneText->SetScreenAlignment(MIDDLE, TOP);
	SceneText->SetPosition(glm::vec3(0, -100, 0));
}

int main()
{
	//Setup All Objects In Project
	InitialSetup();

	//We Initialize The Renderable Loader BEFORE We Set Our Object Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (None)
	AssetLoader::Instance().LoadAssets("Resources");

	//Skyboxes cant be loaded without a supplymentary help or a seperate file not yet implemented, so we manually create it
	//On Loading Scene 2, Create The Used Skybox
	std::string skyboxPaths[6] =
	{
		"Resources/Skybox/Front.png",
		"Resources/Skybox/Back.png",
		"Resources/Skybox/Top.png",
		"Resources/Skybox/Down.png",
		"Resources/Skybox/Right.png",
		"Resources/Skybox/Left.png"
	};

	AssetLoader::CreateSkybox(skyboxPaths, "MainSkybox");

	Time::Instance().Init();

	//Load Each Scene Into The Scene Manager
	LoadScene1();
	Scene::Current().ChangeScene(1);

	//Framebuffer Screen Quad
	ObjectInstance* screenQuad = new ObjectInstance("Screen Quad", glm::vec3(1280 / 2, 720 / 2, 0), glm::vec3(0, 0, 0), glm::vec3(1280, 720, 100), true);
	auto renderer = screenQuad->AddComponent<ScreenQuadRenderer>("ScreenSpaceRender", ProjectionType::ScreenOrthographic);
	renderer->AddTexturePass("Texture_Position", "Texture_Position", TextureType::Texture2D, TilingType::ClampEdges);
	renderer->AddTexturePass("Texture_Normal", "Texture_Normal", TextureType::Texture2D, TilingType::ClampEdges);
	renderer->AddTexturePass("Texture_AlbedoShininess", "Texture_AlbedoShininess", TextureType::Texture2D, TilingType::ClampEdges);
	renderer->AddTexturePass("ShadowMap", "DepthMap", TextureType::Texture2D, TilingType::ClampBorder);
	renderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));

	RenderingPipeline::SetScreenQuadRenderer(renderer);

	RenderingPipeline::InitShadowRendering();
	RenderingPipeline::InitFrameBuffer();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(AssetLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();

		Scene::Current().Update();

		screenQuad->Update();

		glfwPollEvents();

		RenderingPipeline::ShadowPass();

		RenderingPipeline::GeometryPass();
		//RenderingPipeline::FrameBufferPass();

		RenderingPipeline::RenderToScreen();

		RenderingPipeline::WriteDepth();
	}

	glfwTerminate();
	return 0;
}