#pragma once

#include "Light.h"

namespace MoonEngine {

    class DirLight : public Light {
    public:
        DirLight(glm::vec3 color, glm::vec3 direction);
        void update(glm::vec3 direction);
        glm::vec3 getDirection();
    private:
        glm::vec3 _direction;

    };
}