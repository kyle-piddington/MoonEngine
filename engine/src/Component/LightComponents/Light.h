#pragma once
#include "Component/Component.h"
#include "glm/glm.hpp"

namespace MoonEngine 
{

    class Light : public Component
    {
    public:
        Light(glm::vec3 color, float ambient);
        virtual std::shared_ptr<Component> clone() const = 0;

       const  glm::vec3& getColor();

        float getAmbient();

        glm::vec3 _color;
        float _ambient;
    };

    inline Light::Light(glm::vec3 color, float ambient) :
        _color(color), _ambient(ambient){

    }

    inline const glm::vec3& Light::getColor()
    {
        return this->_color;
    }

    inline float Light::getAmbient()
    {
        return _ambient;
    }

}