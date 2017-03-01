#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MoonEngine.h"
#include "LevelEditor/LevelLoader.h"

using namespace MoonEngine;


int main(int argc, char ** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    float windowWidth = 640.0f, windowHeight = 480.0f;
    GLFWwindow * window = glfwCreateWindow(windowWidth, windowHeight, "ShatteredMoon", nullptr, nullptr);
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
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    Logger::SetLogLevel(WARN);
    std::shared_ptr<EngineApp> app = std::make_shared<EngineApp>(window);
    Scene * scene = new Scene();



    //Player Setup
    Transform playerTransform = Transform();
    std::shared_ptr<GameObject> playerObj = std::make_shared<GameObject>(playerTransform);
    playerObj->addComponent(scene->createComponent<ThirdPersonCharacterController>(4.1));

    stringmap textures({ { "diffuse", "wolf.tga" } });

    playerObj->addComponent(scene->createComponent<StaticMesh>("wolf.obj", false));
    playerObj->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.2, 0.2), "geom.program", textures));
    playerObj->addComponent(scene->createComponent<BoxCollider>());

    playerObj->getTransform().setScale(glm::vec3(0.2, 0.2, 0.2));
    playerObj->addTag(T_Player);
    scene->addGameObject(playerObj);

    //Camera setup
    std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>(playerTransform);
    Camera * cam = scene->createComponent<Camera>(3.1415 / 3, windowWidth / windowHeight, 0.1, 300);
    cameraObj->addComponent(cam);
    cameraObj->addComponent(scene->createComponent<ThirdPersonOrbitalController>());
    cameraObj->getTransform().translate(glm::vec3(0, 5, 5));
    scene->addGameObject(cameraObj);


    
    stringmap boxTextures({ { "diffuse", "cube.png" } });
    stringmap groundTextures({ { "diffuse", "stone2.png" } });
    stringmap ligthIndicatorTextures({ { "diffuse", "cactus2.png" } });

    //Ground
    Transform groundTransform;
    groundTransform.setScale(glm::vec3(5, 1, 5));
    std::shared_ptr<GameObject> groundObject = std::make_shared<GameObject>(groundTransform);
    groundObject->addComponent(scene->createComponent<StaticMesh>("quad.obj", true));
    groundObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.8, 0.2), "geom.program"));
    scene->addGameObject(groundObject);

    //Upper Platforms
    Transform levelTransform;
    levelTransform.setScale(glm::vec3(1, 0.5, 1));
    levelTransform.setPosition(glm::vec3(-1, 2, 2));
    std::shared_ptr<GameObject> boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "geom.program", boxTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);


    levelTransform.setPosition(glm::vec3(-1, 2.5, 0.5));
    boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.3, 0.5, 0.8), "geom.program", boxTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    levelTransform.setPosition(glm::vec3(-1, 3, -1));
    boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "geom.program", boxTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    levelTransform.setPosition(glm::vec3(-1, 2.5, -3.5));
    boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.9, 0.5), "geom.program", boxTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    //Boxes
    Transform boxTransform;
    boxTransform.setPosition(glm::vec3(1, 0, 0));
    boxObject = std::make_shared<GameObject>(boxTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8, 0.8, 0.8), "geom.program", boxTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    //boxObject->addComponent(scene->createComponent<CollectableComponent>()); Twas a test
    scene->addGameObject(boxObject);

    boxTransform.setPosition(glm::vec3(2.5, 0.5, 0));
    boxObject = std::make_shared<GameObject>(boxTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8, 0.8, 0.8), "geom.program", boxTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);


    boxTransform.setPosition(glm::vec3(3, 1, 1.5));
    boxObject = std::make_shared<GameObject>(boxTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.4, 0.9, 0.8), "geom.program", boxTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    boxTransform.setPosition(glm::vec3(1.5, 1.5, 2));
    boxObject = std::make_shared<GameObject>(boxTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8, 0.3, 0.8), "geom.program", boxTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);


    


    ////Skydome
    //Transform skydomeTransform;
    //skydomeTransform.setPosition(glm::vec3(0, 0, 0));
    //skydomeTransform.setScale(glm::vec3(1000, 1000, 1000));
    //std::shared_ptr<GameObject> sphereObject = std::make_shared<GameObject>(skydomeTransform);
    //sphereObject->addComponent(scene->createComponent<StaticMesh>("sphere.obj", false));
    //stringmap sky_textures({{"skycolor", "skycolor"}});
    //sphereObject->addComponent(scene->createComponent<Material>(glm::vec3(1.0, 1.0, 1.0), "skydome.program", sky_textures, true));
    //scene->addGameObject(sphereObject);

    //Lights
    Transform lightTransform;
    lightTransform.setPosition(glm::vec3(2, 3, 1));
    std::shared_ptr<GameObject> pointLight = make_shared<GameObject>(lightTransform);
    pointLight->addComponent(scene->createComponent<PointLight>(pointLight->getTransform().getPosition(), COLOR_MAGENTA, 0.9f));
    pointLight->getComponent<PointLight>()->setRange(10);
    scene->addGameObject(pointLight);

    boxTransform.setPosition(glm::vec3(2, 2.9, 1));
    boxTransform.setScale(glm::vec3(.2,.2,.2));
    boxObject = std::make_shared<GameObject>(boxTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8, 0.3, 0.8), "geom.program", ligthIndicatorTextures));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);


    //Lights + boxes
    boxTransform.setPosition(glm::vec3(2, 3, 1));
    boxTransform.setScale(glm::vec3(.2, .2, .2));
    boxObject = std::make_shared<GameObject>(boxTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8, 0.3, 0.8), "geom.program", ligthIndicatorTextures));
    boxObject->addComponent(scene->createComponent<PointLight>(COLOR_MAGENTA, 10.0f));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    boxTransform.setPosition(glm::vec3(0, 2.9, -5));
    boxTransform.setScale(glm::vec3(.2, .2, .2));
    boxObject = std::make_shared<GameObject>(boxTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cubev2.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8, 0.3, 0.8), "geom.program", ligthIndicatorTextures));
    boxObject->addComponent(scene->createComponent<PointLight>(COLOR_RED, 10.0f));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    std::shared_ptr<GameObject> dirLight = make_shared<GameObject>();
    dirLight->addComponent(scene->createComponent<DirLight>(glm::vec3(-1, -1, -1), COLOR_WHITE));
    scene->addGameObject(dirLight);


    
    
    float accumTime;
    int lastUpdateTime;
    scene->addCustomUpdate([&](float dt) {
        //ImGui::ShowTestWindow();
        // accumTime += dt;
        // if((int)accumTime > lastUpdateTime)
        // {
        // 	LOG(GAME, "FPS: " + std::to_string(1.0/dt));
        // 	LOG(GAME, "Active Objects: " + std::to_string(scene->getGameObjects().size()));

        // 	lastUpdateTime = (int)accumTime;
        // }

    });

	 DeferredRenderer * renderer = new DeferredRenderer(width, height, 
        "shadow_maps.program", "deferred_stencil.program", "deferred_pointL.program", "deferred_dirL.program");
    app->run(scene, renderer);

    delete scene;
    delete renderer;

    return 0;

}