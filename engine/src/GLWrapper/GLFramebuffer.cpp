#include "GLFramebuffer.h"
#include <cassert>
using namespace MoonEngine;


int GLFramebuffer::_unitCount = 0;

GLFramebuffer::GLFramebuffer(int width, int height):
    _width(width),
    _height(height),
    _framebufferStatus(GL_FRAMEBUFFER_UNDEFINED),
    _colorCount(0),
    _textureHandles(std::unordered_map<std::string, texture_unit>())
{
    glGenFramebuffers(1, &_handle);
}

GLFramebuffer::~GLFramebuffer()
{
    LOG(INFO, "Deleting Framebuffer " + std::to_string(_handle));
    glDeleteFramebuffers(1, &_handle);
}

GLFramebuffer::GLFramebuffer(GLFramebuffer && other):
    _width(other._width),
    _height(other._height),
    _handle(other.release()),
    _framebufferStatus(other._framebufferStatus),
    _textureHandles(other._textureHandles)
{
}

GLFramebuffer & GLFramebuffer::operator=(GLFramebuffer && other)
{
    _width = other._width;
    _height = other._height;
    _textureHandles = other._textureHandles;
    reset(other.release());
    return *this;
}


void GLFramebuffer::addTexture(const std::string & textureName, GLTexture & texture, GLenum attachmentInfo)
{
    assert(texture.getWidth() == _width && texture.getHeight() == _height);
    bind(GL_DRAW_FRAMEBUFFER);
    texture.bindRaw();
    LOG_GL(__FILE__, __LINE__);

    if (attachmentInfo >= GL_COLOR_ATTACHMENT0 && attachmentInfo < GL_COLOR_ATTACHMENT10) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        _colorCount += 1;
    }
    LOG_GL(__FILE__, __LINE__);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentInfo, GL_TEXTURE_2D, texture.getTextureId(), 0);
    texture_unit txUnit;
    
    txUnit.gl_texture = &texture;
    txUnit.unit = _unitCount++;
    txUnit.attachment = attachmentInfo;
    _textureHandles[textureName] = txUnit;
    _framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
    Unbind();
}

void GLFramebuffer::startFrame() {
    bind(GL_DRAW_FRAMEBUFFER);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLFramebuffer::bindForGeomPass(){
    drawColorAttachments(_colorCount);
}

void GLFramebuffer::bindForStencilPass() {
    glDrawBuffer(GL_NONE);
}

void GLFramebuffer::bindForLightPass() {
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    for (auto &tex : _textureHandles) {
        if (tex.first != COMPOSITE_TEXTURE && tex.first != "depth") {
            glActiveTexture(GL_TEXTURE0 + tex.second.unit);
            glBindTexture(GL_TEXTURE_2D, tex.second.gl_texture->getTextureId());
        }
    }
}

void GLFramebuffer::bindForOutput() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    bind(GL_READ_FRAMEBUFFER);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
}


void GLFramebuffer::status()
{
    _framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (_framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        switch (_framebufferStatus)
        {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LOG(ERROR, "Framebuffer not complete, incomplete attachment");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LOG(ERROR, "Framebuffer not complete, No textures attached");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            LOG(ERROR, "Framebuffer not complete, not supported by openGL version");
            break;
        default:
            LOG(ERROR, "FrameBuffer not complete... " + std::to_string(_framebufferStatus));
        }
    }
    assert(_framebufferStatus == GL_FRAMEBUFFER_COMPLETE);
}

void GLFramebuffer::bind(GLuint mode) {
	glBindFramebuffer(mode, _handle);
}

void GLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


GLuint MoonEngine::GLFramebuffer::getHandle() const
{
    return _handle;
}

void GLFramebuffer::UniformTexture(GLProgram * prog, std::string uniformName, std::string textureName)
{
    texture_unit id = this->getTextureUnit(textureName);
    glUniform1i(prog->getUniformLocation(uniformName), id.unit);
}


void GLFramebuffer::drawColorAttachments(int size) {
	vector<GLuint> colors;
	for (int i = 0; i < size; i++)
		colors.push_back(GL_COLOR_ATTACHMENT0 + i);
    bind(GL_DRAW_FRAMEBUFFER);
    glDrawBuffers(size, &colors[0]);
}

void MoonEngine::GLFramebuffer::DBG_DrawToImgui(string guiName)
{
        ImGui::Begin(guiName.c_str());
        for (auto texHandlePair : _textureHandles)
        {
            ImGui::Image((void *)texHandlePair.second.gl_texture->getTextureId(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
}

GLuint GLFramebuffer::release()
{
    GLuint tmp = _handle;
    _handle = 0;
    return tmp;
}

GLuint GLFramebuffer::reset(GLuint newObject)
{
    glDeleteFramebuffers(1, &_handle);
    _handle = newObject;
    return _handle;
}

GLFramebuffer::texture_unit GLFramebuffer::getTextureUnit(std::string name) const
{
    auto it = _textureHandles.find(name);
    if (it == _textureHandles.end())
    {
        assert(!"Could not find texture in framebuffer");
    }
    return it->second;
}

