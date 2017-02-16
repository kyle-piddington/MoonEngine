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
/**
 * The Default renderer performs a phong rendering
 * of the entire scene, using MaterialInstance.tint's 
 * property to color objects. The renderer
 * does not switch programs, or perform
 * any binning.
 */
namespace MoonEngine
{

    class ProgramRenderer: public I_Renderer
    {
    public:
        ProgramRenderer();

        virtual ~ProgramRenderer()
        {}

        /**
         * Pre-setup for renderer. Gather variables
         * @param scene Scene to render.
         */
        virtual void setup(Scene * scene);

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
        Camera * mainCamera;
        //MeshInfo * renderQuad;

        //GLFramebuffer renderToFB;
        //GLTexture framebufferColorTexture;
        //GLTexture framebufferDepthStencilTexture;
    };
}