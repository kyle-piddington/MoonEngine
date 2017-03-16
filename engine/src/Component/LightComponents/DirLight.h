#pragma once

#include "Light.h"
#include "GLWrapper/GLProgram.h"
#include "Libraries/Library.h"

namespace MoonEngine {

    class DirLight : public Light {
    public:
        /*
        * @param direction
        * @param color
        */
        DirLight(glm::vec3 direction, glm::vec3 color, float ambient = 0.2f, float intensity = 0.8f);
        void update(float dt);
        const glm::vec3& getDirection();
        void setDirection(const glm::vec3 & dir){_direction = dir;}
        std::shared_ptr<Component> clone() const;
    private:
        glm::vec3 _direction;
        float _intensity;
        float oldAmbient;

    };
}