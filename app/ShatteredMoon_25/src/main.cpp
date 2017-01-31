#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MoonEngine.h"


using namespace MoonEngine;


int main(int argc, char **argv) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	Logger::SetLogLevel(GAME);


	MoonEngineCfg cfg;
	cfg.assetPath = "resources";
	
	std::shared_ptr<EngineApp> app = std::make_shared<EngineApp>(window, cfg);
	Scene * scene = new Scene();

	//Game Objects
	std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>();

	std::shared_ptr<GameObject> playerObj = std::make_shared<GameObject>();
	playerObj->addComponent(scene->createComponent<ThirdPersonCharacterController>(2.1));

	
	playerObj->addComponent(scene->createComponent<StaticMesh>("suzanne.obj",false));
	playerObj->addComponent(scene->createComponent<Material>(glm::vec3(0.2,0.2,0.2), "phong.program"));
	playerObj->addComponent(scene->createComponent<BoxCollider>());
	
	playerObj->getTransform().setPosition(glm::vec3(0,0.5,0));
	playerObj->getTransform().setScale(glm::vec3(0.2,0.2,0.2));
	playerObj->addTag(T_Player);
	
	scene->addGameObject(playerObj);

	//Camera setup
	Camera * cam = scene->createComponent<Camera>( 3.1415/3, 800.0/600.0, 0.1, 50);
	cameraObj->addComponent(cam);
	cameraObj->addComponent(scene->createComponent<ThirdPersonOrbitalController>());
	cameraObj->getTransform().translate(glm::vec3(0,5,5));
	//cameraObj->getTransform().rotate(glm::vec3(-M_PI/6,0,0));
	scene->addGameObject(cameraObj);

	
	//Ground
	Transform groundTransform;
	groundTransform.setScale(glm::vec3(5,1,5));
	std::shared_ptr<GameObject> groundObject = std::make_shared<GameObject>(groundTransform);
	groundObject->addComponent(scene->createComponent<StaticMesh>("quad.obj",true));
	groundObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2,0.8,0.2), "default.program"));
	scene->addGameObject(groundObject);

	//Upper Platforms
	Transform levelTransform;
	levelTransform.setScale(glm::vec3(1, 0.5, 1));
	levelTransform.setPosition(glm::vec3(-1, 2, 2));
	std::shared_ptr<GameObject> boxObject = std::make_shared<GameObject>(levelTransform);
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	scene->addGameObject(boxObject);



	levelTransform.setPosition(glm::vec3(-1, 2.5, 0.5));
	boxObject = std::make_shared<GameObject>(levelTransform);
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.3, 0.5, 0.8), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	scene->addGameObject(boxObject);

	levelTransform.setPosition(glm::vec3(-1, 3, -1));
	boxObject = std::make_shared<GameObject>(levelTransform);
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	scene->addGameObject(boxObject);

	levelTransform.setPosition(glm::vec3(-1, 2.5, -3.5));
	boxObject = std::make_shared<GameObject>(levelTransform);
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.9, 0.5), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	scene->addGameObject(boxObject);

	//Boxes
	Transform boxTransform;
	boxTransform.setPosition(glm::vec3(1,0,0));
	boxObject = std::make_shared<GameObject>(boxTransform);
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj",false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8,0.8,0.8), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	scene->addGameObject(boxObject);

	boxTransform.setPosition(glm::vec3(2.5,0.5,0));
	boxObject = std::make_shared<GameObject>(boxTransform);
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj",false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8,0.8,0.8), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	scene->addGameObject(boxObject);


	boxTransform.setPosition(glm::vec3(3,1,1.5));
	boxObject = std::make_shared<GameObject>(boxTransform);
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj",false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.4,0.9,0.8), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	scene->addGameObject(boxObject);

	boxTransform.setPosition(glm::vec3(1.5, 1.5, 2));
	boxObject = std::make_shared<GameObject>(boxTransform);
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8, 0.3, 0.8), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	scene->addGameObject(boxObject);

	
	
	float accumTime;
	int lastUpdateTime;
	scene->addCustomUpdate([&](float dt){
		//ImGui::ShowTestWindow();
		// accumTime += dt;
		// if((int)accumTime > lastUpdateTime)
		// {
		// 	LOG(GAME, "FPS: " + std::to_string(1.0/dt));
		// 	LOG(GAME, "Active Objects: " + std::to_string(scene->getGameObjects().size()));
			
		// 	lastUpdateTime = (int)accumTime;
		// }
				
	});

	

	ProgramRenderer * renderer = new ProgramRenderer();
	app->run(scene, renderer);

	delete scene;
	delete renderer;
	
	
	return 0;

}