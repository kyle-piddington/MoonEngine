#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MoonEngine.h"
#include "CustomComponents/CharacterMoveComponent.h"
#include "CustomComponents/ObjectSpawner.h"


using namespace MoonEngine;


int main(int argc, char **argv) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow * window = glfwCreateWindow(800,600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	Logger::SetLogLevel(VERBOSE);


	MoonEngineCfg cfg;
	cfg.assetPath = "resources";
	
	std::shared_ptr<EngineApp> app = std::make_shared<EngineApp>(window, cfg);
	Scene * scene = new Scene();

	//Game Objects
	std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>();

	
	//Camera setup
	Camera * cam = scene->createComponent<Camera>( M_PI/3, 800.0/600.0, 0.1, 50);
	FirstPersonController * ctrl = scene->createComponent<FirstPersonController>();
	cameraObj->addComponent(ctrl);
	cameraObj->addComponent(cam);
	scene->addGameObject(cameraObj);

	
	//Monkey prefab setup
	Transform monkeyTransform;
	monkeyTransform.setScale(glm::vec3(0.5,0.5,0.5));
	std::shared_ptr<GameObject> monkeyObj = std::make_shared<GameObject>(monkeyTransform);
	StaticMesh * monkeyMesh = scene->createComponent<StaticMesh>("suzanne.obj", true);
	Material * monkeyMat = scene->createComponent<Material>(glm::vec3(0.6,0.5,0.5));
	CharacterMoveComponent * monkeyMove = scene->createComponent<CharacterMoveComponent>(rand()%30 / 10.f);
	BoxCollider * monkeyCollider = scene->createComponent<BoxCollider>();
	monkeyObj->addComponent(monkeyCollider);
	monkeyObj->addComponent(monkeyMesh);
	monkeyObj->addComponent(monkeyMat);
	monkeyObj->addComponent(monkeyMove);
		

	Transform dogTransform;
	dogTransform.setScale(glm::vec3(0.2,0.2,0.2));
	std::shared_ptr<GameObject> dogObj = std::make_shared<GameObject>(dogTransform);
	StaticMesh * dogMesh = scene->createComponent<StaticMesh>("dog.obj", true);
	Material * dogMaterial = scene->createComponent<Material>(glm::vec3(0.6,0.5,0.5));
	CharacterMoveComponent * dogMove = scene->createComponent<CharacterMoveComponent>(rand()%30 / 10.f);
	BoxCollider * dogCollider = scene->createComponent<BoxCollider>();
	dogObj->addComponent(dogCollider);
	dogObj->addComponent(dogMesh);
	dogObj->addComponent(dogMaterial);
	dogObj->addComponent(dogMove);
	
	std::vector<GameObject *> prefabPtrs;
	prefabPtrs.push_back(dogObj.get());
	prefabPtrs.push_back(monkeyObj.get());

	std::shared_ptr<GameObject> spawnerObj = std::make_shared<GameObject>(Transform());
	spawnerObj->addComponent(scene->createComponent<ObjectSpawner>(1.0f,prefabPtrs));
	scene->addGameObject(spawnerObj);



	

	DefaultRenderer * renderer = new DefaultRenderer();
	app->run(scene, renderer);

	// global setup...
	// GLProgram program = createProgram();
	// float data[] = 
	// {
	// 	-0.5,-0.5,0,
	// 	0.5,-0.5,0,
	// 	0,0.5,0
	// };

	// GLBuffer buffer(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	// GLVertexArrayObject obj;
	// obj.bindVertexBuffer(0,buffer);
	// glClearColor(0.2,0.2,0.6,1.0);
	// program.enable();
	// while(!glfwWindowShouldClose(window))
	// {
	//     glfwPollEvents();
	    
	//     glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//     obj.bind();
	//     glDrawArrays(GL_TRIANGLES,0,3);
	//     GLVertexArrayObject::Unbind();
	//     glfwSwapBuffers(window);
	// }
	delete scene;
	delete renderer;
	
	
	return 0;

}