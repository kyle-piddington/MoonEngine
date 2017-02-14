#pragma once
#include "Component/Component.h"
#include "glm/glm.hpp"

namespace MoonEngine 
{

    class Light : public Component
    {
    public:
        Light(glm::vec3 color, float ambient, float intensity);
        virtual std::shared_ptr<Component> clone() const = 0;

       const  glm::vec3& getColor();

        float getAmbient();

        glm::vec3 _color;
        float _ambient;
        float _intensity;
    };

    inline Light::Light(glm::vec3 color, float ambient, float intensity) :
        _color(color), _ambient(ambient), _intensity(intensity){

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