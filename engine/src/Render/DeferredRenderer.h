#pragma once

#include "GLWrapper/GLProgram.h"
#include "I_Renderer.h"
#include "Component/Components.h"
#include "Framebuffers/GBuffer.h"
#include "Util/Logger.h"
#include "GLWrapper/OpenGL.h"
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "Component/Components.h"
#include "GameObject/GameObject.h"
#include "Geometry/MeshCreator.h"
#include <iostream>
#include "GLUtil/GL_LOG.h"
#include "PostProcess/PostProcessStep.h"
#include "Libraries/Library.h"
#include "GLWrapper/GLConstants.h"
#include "Framebuffers/ShadowMaps.h"



/**
 * The Deferred renderer performs a phong rendering
 * of the entire scene, but with a two stage defferred
 * render pass. Light volumes are used to cull fragments.
 * Some binning is done in ensureing only defferred objects
 * get drawn, then forward objects after. 
 */
namespace MoonEngine
{

    class DeferredRenderer: public I_Renderer
    {
    public:
        DeferredRenderer(int width, int height, string shadowMapsProgramName, string stencilProgramName, string pointLightProgramName, string dirLightProgramName);

        virtual ~DeferredRenderer()
        {}

        /**
         * Pre-setup for renderer. Gather variables
         * @param scene Scene to render.
         */
        virtual void setup(Scene * scene, GLFWwindow * window);

        /**
         * Render the current state of the scene
         * @param scene the scene to render.
         */
        virtual void render(Scene * scene);

        /**
         * Perform cleanup.
         */
        virtual void shutdown();

        void addPostProcessStep(std::shared_ptr<PostProcessStep> step);


    private:

        void shadowMapPass(Scene* scene);
		void geometryPass(Scene* scene);

        void stencilPass(std::shared_ptr<GameObject> light);
        void pointLightPass(std::shared_ptr<GameObject> light);

        void dirLightPass(Scene* scene);
        void outputPass(Scene* scene);
        void forwardPass(Scene* scene);

        //Setup Uniforms shared across both light passes
        void setupShadowMapUniforms(GLProgram* prog);
        void setupPointLightUniforms(GLProgram* prog, std::shared_ptr<GameObject> light);
        void setupDirLightUniforms(GLProgram* prog);
        
        Camera* _mainCamera;
        glm::vec3 _mainCameraPosition;
        MeshInfo* _renderQuad;
		GLuint _width, _height;
        int _deferredWidth, _deferredHeight;
        GBuffer _gBuffer;
		GLTexture* _colorTex, * _positionTex, * _normalTex, * _textureTex;
        GLTexture* _depthTex;
        GLTexture* _outputTex;
        
        ShadowMaps _shadowMaps;
        bool _debugShadows;

        GLProgram* _shadowMapsProgram;
        GLProgram* _stencilProgram;
        GLProgram* _pointLightProgram;
        GLProgram* _dirLightProgram;

        std::vector<std::shared_ptr<PostProcessStep>> postprocessPipeline;
    };

}