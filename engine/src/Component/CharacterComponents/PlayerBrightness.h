#pragma once

#include "Component/Components.h"
#include "Component/Component.h"
#include "Geometry/Transform.h"

namespace MoonEngine
{
    class PointLight;
	class PlayerBrightness : public Component
	{
	public:
		PlayerBrightness();
		
		void start();

		void update(float dt);
		
		std::shared_ptr<Component> clone() const;

	private:
        void increaseBrightness(glm::vec3 colorIncrease, float distanceIncrease);

        float distance;
        glm::vec3 color;

        PointLight * _pointLight;
	};
}