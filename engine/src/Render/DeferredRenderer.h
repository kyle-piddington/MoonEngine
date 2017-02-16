#pragma once

#include "GLWrapper/GLProgram.h"
#include "I_Renderer.h"
#include "Component/Components.h"
#include "GLWrapper/GLFramebuffer.h"
#include "Util/Logger.h"
#include "GLWrapper/OpenGL.h"
#include <glm/gtc/type_ptr.hpp>
#include "Component/Components.h"
#include "GameObject/GameObject.h"
#include "Geometry/MeshCreator.h"
#include "thirdparty/imgui/imgui.h"
#include <iostream>
#include "GLUtil/GL_LOG.h"

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
        DeferredRenderer(int width, int height, string pointLightProgramName, string dirLightProgramName);

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


    private:

		vector<std::shared_ptr<GameObject>> geometryPass(Scene* scene);
        void lightingSetup();
        void pointLightPass(Scene* scene);
        void dirLightPass(Scene* scene);
        void forwardPass(Scene* scene, vector<std::shared_ptr<GameObject>> forwardObjects);

        //Setup Uniforms shared across both light passes
        void setupLightUniforms(GLProgram* prog);
        Camera* _mainCamera;
        glm::vec3 _mainCameraPosition;
        MeshInfo* _renderQuad;
		GLuint _width, _height;
        int _deferredWidth, _deferredHeight;
        GLFramebuffer _gBuffer;
		GLTexture _colorTex, _positionTex, _normalTex, _textureTex;
        GLTexture _depthTex;

        GLProgram * _pointLightProgram;
        GLProgram * _dirLightProgram;
    };

    void drawBufferToImgui(std::string guiName, const GLFramebuffer * bfr);
}