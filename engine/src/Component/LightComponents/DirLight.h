#pragma once

#include "Light.h"
#include "GLWrapper/GLProgram.h"
#include "Libraries/Library.h"

namespace MoonEngine {

    class DirLight : public Light {
    public:
        DirLight(glm::vec3 color, glm::vec3 direction);
        void update(glm::vec3 direction);
        const glm::vec3& getDirection();
        std::shared_ptr<Component> DirLight::clone() const;
    private:
        glm::vec3 _direction;

    };
}