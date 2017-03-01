#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "GLWrapper/GLTexture.h"
#include "GLWrapper/GLConstants.h"
#include "GLUtil/GLTextureConfiguration.h"
#include "Libraries/Library.h"
#include "Util/MathUtil.h"
#include "Scene/Scene.h"
#include "Component/Components.h"
#include <vector>

#define NUM_SHADOWS 3
#define NUM_CORNERS 8

namespace MoonEngine {

	class ShadowMaps
	{
	public:
		ShadowMaps(int width, int height);
		~ShadowMaps();
        void bindForWriting(int shadowLevel);
        void bindForReading();
        void calculateShadowLevels(Scene* scene);
        const glm::mat4 getOrtho(int shadowLevel);
        const glm::mat4 getLightView();
        const float getShadowZ(int shadowLevel);
        void DBG_DrawToImgui();
        int getWidth(){return _width;}
        int getHeight(){return _height;}
    private:
        float _width;
        float _height;
        std::vector<GLTexture*> _depthTexs;
        std::vector<glm::mat4> _orthos;
        glm::mat4 _lightView;
        float _shadowZDepth[4];
        GLuint _handle;
        void status();
	};
}