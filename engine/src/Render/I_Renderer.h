#pragma once

#include "Scene/Scene.h"
#include <GLFW/glfw3.h>
/**
 * The renderer interface is implemented by any renderer
 * who needs to render a scene. The current (naieve) renderer
 * does no more than iterate through a list and phong-shade all objects
 * with a static directional light. More compliceated
 * renderers adapt to this same interface.
 */
namespace MoonEngine
{
    class I_Renderer
    {
    public:
        virtual void setup(Scene * scene, GLFWwindow * window) = 0; //Initial setup for scene. Collect any info neccesary.
        virtual void render(Scene * scene) = 0; //Perform the full render loop for this scene
        virtual void shutdown() = 0; // Any Shutdown tasks neccesary
    };
}
