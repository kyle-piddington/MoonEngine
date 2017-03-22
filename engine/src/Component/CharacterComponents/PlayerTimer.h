#pragma once

#include "Component/Components.h"
#include "Component/Component.h"
#include "Geometry/Transform.h"

namespace MoonEngine
{
	class PlayerTimer : public Component
	{
	public:
		PlayerTimer();

		void start();

		void update(float dt);
		
		std::shared_ptr<Component> clone() const;

	private:
		vector<float> _times;

        int _currentTime;

        bool _timing;
	};
}