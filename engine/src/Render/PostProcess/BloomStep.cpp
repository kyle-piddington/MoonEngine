#include "BloomStep.h"
#include "Libraries/Library.h"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

using namespace MoonEngine;

BloomStep::BloomStep()
{

}

void BloomStep::setup(GLFWwindow * window, Scene * scene)
{
    string path = "postprocess/bloom/post_bloom_";
    _glowProgram = Library::ProgramLib->getProgramForName(path + "glow.program");
    _blurProgram = Library::ProgramLib->getProgramForName(path + "blur.program");
    _combineProgram = Library::ProgramLib->getProgramForName(path + "combine.program");

	_inputTexture = Library::TextureLib->getTexture("output");

    int _width, _height;
    glfwGetWindowSize(window, &_width, &_height);

    GLTextureConfiguration colorCFG(_width, _height, GL_RGBA, GL_RGBA, GL_FLOAT);
    _glowTexture = Library::TextureLib->createTexture("_bloomGlowTexture", colorCFG);

    fbo = std::make_shared(_width, _height);
    fbo->addTexture("glow", _glowTexture);
}

void BloomStep::extractGlow() {
    _glowProgram->enable();
    _inputTexture->bind(0);
    glUniform1i(_glowProgram->getUniformLocation("_inputTexture"), 0);
}


void BloomStep::render(Scene * scene)
{
	fbo->bind(GL_FRAMEBUFFER);

    extractGlow();

    if (_glowProgram->hasUniform("iGlobalTime")) {
        glUniform1f(_glowProgram->getUniformLocation("iGlobalTime"), scene->getGlobalTime());
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderToScreen();
}