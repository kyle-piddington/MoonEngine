#pragma once
#include "glm/glm.hpp"
#include "GLWrapper/GLTexture.h"
#include <vector>

namespace Moonengine {

	class ShadowMap
	{
	public:
		ShadowMap();
		~ShadowMap();
		glm::mat4 getLightSpaceMatrix();
	private:
        std::vector<GLTexture*> depthTexs;
	};
}