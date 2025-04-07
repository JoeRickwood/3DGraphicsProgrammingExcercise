#include "Scene.h"
#include "Physics.h"

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

	//Object Which Shows What A Regular Object With Animation Looks Like
	ObjectInstance* test = new ObjectInstance("Test", glm::vec3(-100.0f, 50.0f, 0.0f), glm::vec3(0.f), glm::vec3(100.f, 100.f, 100.f));
	test->AddComponent<Renderer>(RenderableType::Quad, ShaderType::Texture, 0, ProjectionType::Orthographic);
	Animator* anim = test->AddComponent<Animator>(3.f);
	anim->AddAnimation(Animation(0, 4, 2, 8));
	test->AddComponent<TestCamera>(2.0f, 150.f);

	//Object That Interpolates Between Two Textures
	ObjectInstance* testLerp = new ObjectInstance("TestLerp", glm::vec3(-200.0f, 50.0f, 0.0f), glm::vec3(0.f), glm::vec3(100.f, 100.f, 100.f));
	testLerp->AddComponent<Renderer>(RenderableType::Quad, ShaderType::LerpTest, 0, ProjectionType::Orthographic);
	Animator* anim2 = testLerp->AddComponent<Animator>(3.f);
	anim2->AddAnimation(Animation(0, 4, 2, 8));
	testLerp->AddComponent<TestLerp>(1.f, 0, 1);

	//Repeating Texture Shows 2x2 Textures Mapped Correctly
	ObjectInstance* testRepeat = new ObjectInstance("TestRepeat", glm::vec3(150.0f, -25.0f, 0.0f), glm::vec3(0.f), glm::vec3(250.f, 250.f, 250.f));
	testRepeat->AddComponent<Renderer>(RenderableType::Quad, ShaderType::Texture, 2, ProjectionType::Orthographic);
	testRepeat->AddComponent<TestRepeat>(2);


	//Object That Is Flipped Left
	ObjectInstance* testFlipLeft = new ObjectInstance("TestFlipLeft", glm::vec3(-200.0f, -100.0f, 0.0f), glm::vec3(0.f), glm::vec3(100.f, 100.f, 100.f));
	testFlipLeft->AddComponent<Renderer>(RenderableType::Quad, ShaderType::Texture, 2, ProjectionType::Orthographic);
	testFlipLeft->GetComponent<Renderer>()->FlipX(true); //Flip The Renderer X Value, To Make Object Face Left

	//Object That Is Flipped Right
	ObjectInstance* testFlipRight = new ObjectInstance("TestFlipRight", glm::vec3(-100.0f, -100.0f, 0.0f), glm::vec3(0.f), glm::vec3(100.f, 100.f, 100.f));
	testFlipRight->AddComponent<Renderer>(RenderableType::Quad, ShaderType::Texture, 2, ProjectionType::Orthographic);


	Scene::Current().AddObject(testRepeat);
	Scene::Current().AddObject(test);
	Scene::Current().AddObject(testLerp);
	Scene::Current().AddObject(testFlipLeft);
	Scene::Current().AddObject(testFlipRight);


	//Initialize GLFW And setting the version to 4.6
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create Window
	GraphicsLoader::Instance().currentWindow = glfwCreateWindow(800, 800, "OPEN GL EXCERCISE", NULL, NULL);

	if(GraphicsLoader::Instance().currentWindow == NULL)
	{
		std::cout << "Error Creating GraphicsLoader::Instance().currentWindow, GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	//Make The Window The Current Context Of GLFW
	glfwMakeContextCurrent(GraphicsLoader::Instance().currentWindow);

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
	while (glfwWindowShouldClose(GraphicsLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		Update();

		Physics::Instance().ResolveCollisions();

		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();

		Render();
	}

	glfwTerminate();
	return 0;
}

//Sets Up Objects + Other GLFW Parameters
void InitialSetup() 
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_CULL_FACE);

	glClearColor(0.6f, 0.6f, 1.0f, 1.0f);
	glViewport(0, 0, (GLsizei)GraphicsLoader::Instance().windowSize.x, (GLsizei)GraphicsLoader::Instance().windowSize.y);

	GraphicsLoader::Instance().InitializeShaderPrograms(); // Generates The Shader Programs To Be Used By Renderables
	GraphicsLoader::Instance().InitializeTextures(); //Generates The Textures Used

	Time::Instance().Init();

	glfwSetWindowSizeCallback(GraphicsLoader::Instance().currentWindow, (GLFWwindowsizefun)OnWindowResized);
}

//Update Is Called Once Every Frame BEFORE Render
void Update() 
{
	Scene::Current().Update();

	glfwPollEvents();
}

//Render Is Called After Update And Draws Objects To The Screen
void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::Current().Render();

	glfwSwapBuffers(GraphicsLoader::Instance().currentWindow);
}

//On Window Resized Callback Links To The glfwWindowSizefun
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height) 
{
	glViewport(0, 0, _Width, _Height);

	GraphicsLoader::Instance().windowSize.x = (float)_Width;
	GraphicsLoader::Instance().windowSize.y = (float)_Height;
}