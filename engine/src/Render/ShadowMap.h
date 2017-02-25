#pragma once
#include "glm/glm.h"

namespace Moonengine {

	class ShadowMaps
	{
	public:
		ShadowMaps();
		~ShadowMaps();
		glm::mat4 getLightSpaceMatrix();
	private:

	};
}