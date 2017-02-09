#pragma once

#include "Light.h"
#include "GLWrapper/GLProgram.h"
#include "Libraries/Library.h"

namespace MoonEngine {

    class DirLight : public Light {
    public:
        DirLight(glm::vec3 color, glm::vec3 direction, std::string program);
        void update(glm::vec3 direction);
        glm::vec3 getDirection();
        GLProgram* getProgram();
    private:
        glm::vec3 _direction;
        GLProgram* _program;

    };
}