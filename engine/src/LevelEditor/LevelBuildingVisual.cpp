#include "LevelBuildingVisual.h"
#include "Component/CameraComponents/Camera.h"
#include <Component/MaterialComponents/Material.h>
#include <Component/MeshComponents/StaticMesh.h>
#include "IO/Keyboard.h"
#include <glfw/glfw3.h>
#include <IO/Mouse.h>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "GlobalFuncs/GlobalFuncs.h"
#include "thirdparty/imgui/imgui.h"
#include "Libraries/Library.h"
#include "Geometry/Transform.h"

using namespace MoonEngine;

#define DISTANCE_AHEAD 2.0f

LevelBuildingVisual::LevelBuildingVisual(Scene * scene) :
_moveWithCamera(true)
{
    _scene = scene;
    _scene->addCustomUpdate([&](float dt) {
        initBuildingVisual();
        updateGui();
        transformCurrentObject();
    });
}

shared_ptr<GameObject> LevelBuildingVisual::setCurrentLevelMaterial(bool resetTransform) {
    _previousLevelMaterial = _currentLevelMaterial;

    Level::LevelMaterial * levelMaterial =
            Library::LevelLib->getLevelMaterial(_levelMaterials[_currentLevelMaterial]);

    std::shared_ptr<GameObject> object = _scene->createGameObject();
    object->addComponent(_scene->createComponent<StaticMesh>(levelMaterial->mesh, false));
    object->addComponent(_scene->cloneComponent<Material>(levelMaterial->material));

    _moveWithCamera = true;
    _scene->addGameObject(object);

    if (resetTransform) {
        /* Load the current models transforms */
        Transform * transform = &object->getTransform();

        _position = transform->getPosition();
        _rotation = glm::eulerAngles(transform->getRotation());
        _scale = transform->getScale();
    }

    return object;
}

void LevelBuildingVisual::initBuildingVisual() {
    /* Fetch camera after it is created, fetch the current Level Materials. */
    if (_mainCamera == nullptr)
    {
        _mainCamera = GetWorld()->findGameObjectWithComponent<Camera>();

        _levelMaterials = Library::LevelLib->getAllLevelMaterials();
        _currentLevelMaterial = 1;
        _currentObject = 0;

        _objects.push_back(setCurrentLevelMaterial(true));
    }
}

void LevelBuildingVisual::updateGui() {
    ImGui::Begin("Level Editor");
    ImGui::InputFloat3("Position", glm::value_ptr(_position));
    ImGui::InputFloat3("Rotation", glm::value_ptr(_rotation));
    ImGui::InputFloat3("Scale", glm::value_ptr(_scale));

    /* Allow selection of level material */
    std::vector<const char *> levelMats{};
    for (const auto & levelMat : _levelMaterials)
    {
        levelMats.push_back(levelMat.c_str());
    }
    ImGui::ListBox("Level Material", &_currentLevelMaterial, levelMats.data(), levelMats.size());

    /* We selected a different material */
    if (_currentLevelMaterial != _previousLevelMaterial)
    {
        _scene->deleteGameObject(_objects[_currentObject]);
        _objects[_currentObject] = setCurrentLevelMaterial(true);
    }

    string moving = _moveWithCamera ? "Moving" : "Stationary";
    moving += " object (toggle with right shift)";
    ImGui::Text("%s", moving.c_str());
    ImGui::Text("Press 'U' to undo");
    ImGui::Text("Click to place object");
    ImGui::End();

    /* Place in world and save */
    if (Mouse::clicked(GLFW_MOUSE_BUTTON_LEFT)) {
        /* Save object in world */
        Level::LevelObject levelObject;
        levelObject.levelMaterial = _levelMaterials[_currentLevelMaterial];
        levelObject.transform = &_objects[_currentObject]->getTransform();
        Library::LevelLib->addLevelObject(levelObject);

        /* Make a new object */
        _objects.push_back(setCurrentLevelMaterial(false));
        _currentObject++;
    }

    /* Keybindings */
    if (Keyboard::key(GLFW_KEY_RIGHT_SHIFT))
    {
        _moveWithCamera = !_moveWithCamera;
    }

    /* Undo */
    if (Keyboard::key(GLFW_KEY_U))
    {
        if (_currentObject > 0)
        {
            Library::LevelLib->removeLevelObject();

            _scene->deleteGameObject(_objects[_currentObject]);
            _objects.pop_back();
            _currentObject--;
            _moveWithCamera = true;
        }
    }
}

void LevelBuildingVisual::transformCurrentObject()
{
    /* Allow object to stop moving with camera */
    if (_moveWithCamera)
    {
        glm::vec3 camForward = normalize(_mainCamera->getTransform().forward());
        glm::vec3 camPos = _mainCamera->getTransform().getPosition();

        _position = camPos + camForward * -DISTANCE_AHEAD;
    }

    /* Apply any changes */
    Transform * transform = &_objects[_currentObject]->getTransform();

    transform->setPosition(_position);
    transform->setRotation(_rotation);
    transform->setScale(_scale);
}