#include "LevelBuilder.h"
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
        _distanceAhead = 2.5;

        _objects.push_back(setCurrentLevelMaterial(true));
    }
}

void LevelBuildingVisual::updateGui() {
    ImGui::Begin("Level Editor");
    ImGui::DragFloat3("Position", glm::value_ptr(_position), 0.05f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(_rotation), 0.01f);
    ImGui::DragFloat3("Scale", glm::value_ptr(_scale), 0.01f, 0.001f, 10.0f);
    ImGui::Spacing();

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

    if (ImGui::Button("Toggle Move With Camera"))
    {
        _moveWithCamera = !_moveWithCamera;
    }
    string moving = _moveWithCamera ? "Enabled" : "Disabled";
    ImGui::SameLine();
    ImGui::Text("%s\n", moving.c_str());

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
     /* Undo */
    if (Keyboard::key(GLFW_KEY_U) || ImGui::Button("Undo"))
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

    ImGui::Text("\nClick to place object");
    ImGui::SliderFloat("Object distance", &_distanceAhead, 0.5, 10, "%.5f");

    if (ImGui::Button("Save")) {
        Library::LevelLib->serializeLevelObjects();
    }
    ImGui::End();
}

void LevelBuildingVisual::transformCurrentObject()
{
    /* Allow object to stop moving with camera */
    if (_moveWithCamera)
    {
        glm::vec3 camForward = normalize(_mainCamera->getTransform().forward());
        glm::vec3 camPos = _mainCamera->getTransform().getPosition();

        _position = camPos + camForward * -_distanceAhead;
    }

    /* Apply any changes */
    Transform * transform = &_objects[_currentObject]->getTransform();

    transform->setPosition(_position);
    transform->setRotation(_rotation);
    transform->setScale(_scale);
}