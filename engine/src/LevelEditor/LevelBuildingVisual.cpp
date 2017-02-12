#include "LevelBuildingVisual.h"
#include "Component/CameraComponents/Camera.h"
#include <Component/MaterialComponents/Material.h>
#include <Component/MeshComponents/StaticMesh.h>
#include "IO/Keyboard.h"
#include <glfw/glfw3.h>
#include "glm/gtc/type_ptr.hpp"
#include "GlobalFuncs/GlobalFuncs.h"
#include "thirdparty/imgui/imgui.h"
#include "Libraries/Library.h"
#include "Geometry/Transform.h"

using namespace MoonEngine;

LevelBuildingVisual::LevelBuildingVisual(Scene * scene) :
_moveWithCamera(true)
{
    _scene = scene;
    _scene->addCustomUpdate([&](float dt) {
        handleMove(dt);
    });
}

void LevelBuildingVisual::handleMove(float dt)
{
    if (_mainCamera == nullptr)
    {
        _mainCamera = GetWorld()->findGameObjectWithComponent<Camera>();

        levelMaterials = Library::LevelLib->getAllLevelMaterials();
        currentLevelMaterial = previousLevelMaterial = 1;

        Level::LevelMaterial * levelMaterial =
                Library::LevelLib->getLevelMaterial(levelMaterials[currentLevelMaterial]);

        std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
        object->addComponent(_scene->createComponent<StaticMesh>(levelMaterial->mesh, false));
        object->addComponent(_scene->cloneComponent<Material>(levelMaterial->material));

        _currentObject = object;
        _scene->addGameObject(_currentObject);
    }

    Transform * transform = &_currentObject->getTransform();

    glm::vec3 position = transform->getPosition();
    glm::vec3 rotation = glm::eulerAngles(transform->getRotation());
    glm::vec3 scale = transform->getScale();

    if (_moveWithCamera)
    {
        glm::vec3 camForward = normalize(_mainCamera->getTransform().forward());
        glm::vec3 camPos = _mainCamera->getTransform().getPosition();

        position = camPos + camForward * -2.0f;
    }

    ImGui::Begin("Level Editor");
    ImGui::InputFloat3("Position", glm::value_ptr(position));
    ImGui::InputFloat3("Rotation", glm::value_ptr(rotation));
    ImGui::InputFloat3("Scale", glm::value_ptr(scale));


    std::vector<const char *> levelMats{};

    for (const auto & levelMat : levelMaterials)
    {
        levelMats.push_back(levelMat.c_str());
    }

    ImGui::ListBox("Level Material", &currentLevelMaterial, levelMats.data(), levelMats.size());

    if (currentLevelMaterial != previousLevelMaterial)
    {
        previousLevelMaterial = currentLevelMaterial;
        _scene->deleteGameObject(_currentObject);

        Level::LevelMaterial * levelMaterial =
                Library::LevelLib->getLevelMaterial(levelMaterials[currentLevelMaterial]);

        std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
        object->addComponent(_scene->createComponent<StaticMesh>(levelMaterial->mesh, false));
        object->addComponent(_scene->cloneComponent<Material>(levelMaterial->material));

        _currentObject = object;
        _scene->addGameObject(_currentObject);
    }

    if (!_moveWithCamera) {
        ImGui::Text("Not moving with camera");
    }

    ImGui::End();

    transform->setPosition(position);
    transform->setRotation(rotation);
    transform->setScale(scale);

    if(Keyboard::key(GLFW_KEY_X))
    {
        if (Keyboard::isKeyToggled(GLFW_KEY_X))
        {
            _moveWithCamera = !_moveWithCamera;
        }
    }

}