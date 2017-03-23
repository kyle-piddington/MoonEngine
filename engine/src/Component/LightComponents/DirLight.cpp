#include "DirLight.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Util/MathUtil.h"
using namespace MoonEngine;

DirLight::DirLight(glm::vec3 direction, glm::vec3 color, float ambient, float intensity) :
    Light(color, ambient), _direction(direction), _intensity(intensity)
{
	oldAmbient = ambient;
}

void DirLight::update(float dt)
{
	const float horizonTime = 0.48f;
    float time = GetWorld()->getGlobalTime();
    _direction = glm::vec3(-1, -1 + 1.5 * time, -1);
    _color = glm::mix(glm::vec3(1,1,1), glm::vec3(0.8,0.6,0.6), time/horizonTime);
    if(time >horizonTime)
    {
    	float mixTime = time - horizonTime;
    	_ambient = std::max(0.0f,MathUtil::lerp(oldAmbient,0,mixTime));
    	_color = glm::vec3(0.8,0.6,0.6) * std::max(0.0f,MathUtil::lerp(1,0,mixTime));
    }

}

const glm::vec3& DirLight::getDirection()
{
    return _direction;
}

std::shared_ptr<Component> DirLight::clone() const
{
    return std::make_shared<DirLight>(*this);
}
