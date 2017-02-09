#pragma once
#include "Component/Component.h"
#include "glm/glm.hpp"

namespace MoonEngine 
{

    class Light : public Component
    {
    public:
        Light(glm::vec3 color): 
            _color(color),
            _ambient(1.0f),
            _intensity(1.0f)
            {};
        virtual std::shared_ptr<Component> clone() const = 0;

        glm::vec3 getColor(){
            return _color;
        }

        GLfloat getAmbient() {
            return _ambient;
        }

        GLfloat getIntensity() {
            return _intensity;
        }

    private:
        glm::vec3 _color;
        GLfloat _ambient;
        GLfloat _intensity;
    };



}