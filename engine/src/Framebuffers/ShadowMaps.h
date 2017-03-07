#pragma once
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include "GLWrapper/GLFramebuffer.h"
#include "GLUtil/GLTextureConfiguration.h"
#include "Libraries/Library.h"
#include "Util/MathUtil.h"
#include "Scene/Scene.h"
#include "Component/Components.h"
#include <vector>

#define NUM_SHADOWS 3
#define NUM_CORNERS 8

namespace MoonEngine {

	class ShadowMaps :public GLFramebuffer
	{
	public:
		ShadowMaps(int width, int height);
		~ShadowMaps();
        void bindForWriting(unsigned shadowLevel);
        void bindForReading();
        void calculateShadowLevels(Scene* scene);
        const glm::mat4 getOrtho(int shadowLevel);
        const glm::mat4 getLightView();
        const float getShadowZ(int shadowLevel);
        void setMidpoints(float level1, float level2);
        void DBG_DrawToImgui();
    private:
        std::vector<glm::mat4> _orthos;
        glm::mat4 _lightView;
        float _shadowZDepth[4];
	};
}