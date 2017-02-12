#pragma once

#include "Geometry/Transform.h"
#include "Scene/Scene.h"
#include <stdlib.h>

namespace MoonEngine
{

    class LevelBuildingVisual
    {
    public:
        LevelBuildingVisual(Scene * scene);

    private:
        void setCurrentLevelMaterial(bool);
        void initBuildingVisual();
        void updateGui();
        void transformCurrentObject();

        GameObject * _mainCamera;
        std::shared_ptr<GameObject> _currentObject;

        Scene * _scene;

        std::vector<std::string> levelMaterials;

        bool _moveWithCamera;

        int currentLevelMaterial;

        int previousLevelMaterial;

        glm::vec3 _position;
        glm::vec3 _rotation;
        glm::vec3 _scale;

    };
}