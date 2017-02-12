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

        void start();

        void update(float dt);

    private:
        GameObject * _mainCamera;
        std::shared_ptr<GameObject> _currentObject;

        Scene * _scene;

        void handleMove(float dt);

        bool _moveWithCamera;

    };
}