#pragma once

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
		glm::vec3 getColor();
		float getDistance();
		std::shared_ptr<Component> clone() const;

	private:
        void increaseBrightness(glm::vec3 colorIncrease, float distanceIncrease);

		float distance;
        float _linear;
		float _exp;

        glm::vec3 color;

        PointLight * _pointLight;
	};
}