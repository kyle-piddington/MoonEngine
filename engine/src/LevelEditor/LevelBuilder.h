#pragma once

#include "Geometry/Transform.h"
#include "Scene/Scene.h"
#include <stdlib.h>
#include <queue>

namespace MoonEngine
{

    class LevelBuildingVisual
    {
    public:
        LevelBuildingVisual(Scene * scene);

    private:
        std::shared_ptr<GameObject> setCurrentLevelMaterial(bool);
        void initBuildingVisual();
        void updateGui();
        void transformCurrentObject();

        GameObject * _mainCamera;
        Scene * _scene;

        int _currentObject;
        std::vector<std::shared_ptr<GameObject>> _objects;

        int _currentLevelMaterial;
        int _previousLevelMaterial;
        std::vector<std::string> _levelMaterials;

        bool _moveWithCamera;

        float _distanceAhead;

        glm::vec3 _position;
        glm::vec3 _rotation;
        glm::vec3 _scale;

    };
}