#pragma once
#include "glm/glm.hpp"

namespace Moonengine {

	class ShadowMap
	{
	public:
		ShadowMap();
		~ShadowMap();
		glm::mat4 getLightSpaceMatrix();
	private:

	};
}