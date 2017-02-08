#include "PointLight.h"

using namespace MoonEngine;

std::shared_ptr<Component> PointLight::clone() const
{
    return std::make_shared<PointLight>(*this);
}
