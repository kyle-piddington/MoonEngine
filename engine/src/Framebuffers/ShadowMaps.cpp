#include "ShadowMaps.h"
#include "thirdparty/imgui/imgui.h"
#include <iostream>
using namespace MoonEngine;

ShadowMaps::ShadowMaps(int width, int height):
    GLFramebuffer(width, height)
{
   
    glBindFramebuffer(GL_FRAMEBUFFER, _handle);
   
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    // status();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindTexture(GL_TEXTURE_2D,0);


}

ShadowMaps::~ShadowMaps()
{
    LOG(INFO, "Deleting ShadowMaps " + std::to_string(_handle));
    glDeleteFramebuffers(1, &_handle);
}

void ShadowMaps::bindForWriting(unsigned shadowLevel)
{
    texture_unit id = getTextureUnit(SHADOW_TEXTURE + to_string(shadowLevel));

    glBindFramebuffer(GL_FRAMEBUFFER, _handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id.gl_texture->getTextureId(), 0);
    status();
}

void ShadowMaps::bindForReading()
{
    for (unsigned i = 0; i < NUM_SHADOWS; i++) {
        texture_unit id = getTextureUnit(SHADOW_TEXTURE + to_string(i));
        glActiveTexture(GL_TEXTURE5 + i);
        glBindTexture(GL_TEXTURE_2D, id.gl_texture->getTextureId());
    }
}

void MoonEngine::ShadowMaps::calculateShadowLevels(Scene * scene)
{

    static float edge = 5.0f;
    ImGui::DragFloat("Shadow Edge: ", &edge);
    Camera* cam = scene->getMainCamera()->getComponent<Camera>();

    //glm::mat4 CameraInvView = cam->getView();
    glm::mat4 CameraInvView = scene->getMainCamera()->getTransform().getMatrix();
    glm::vec3 lightDir =  scene->getDirLightObject()->getComponent<DirLight>()->getDirection();
    _lightView = glm::lookAt( glm::vec3(0, 0, 0), lightDir, World::Up);

    float tanHalfHFOV = tanf((cam->getFOV() / 2.0f));
    float tanHalfVFOV = tanf((cam->getFOV() * (1/cam->getAspect())) / 2.0f);
    _shadowZDepth[0] = cam->getNear();
    _shadowZDepth[1] = 10.0f;
    _shadowZDepth[2] = 25.0f;
    _shadowZDepth[3] = 100.0f;
    _orthos.clear();
    for (int i = 0; i < NUM_SHADOWS; i++) {
        float xn = _shadowZDepth[i] * tanHalfHFOV;
        float xf = _shadowZDepth[i + 1] * tanHalfHFOV;
        float yn = _shadowZDepth[i] * tanHalfVFOV;
        float yf = _shadowZDepth[i + 1] * tanHalfVFOV;

        glm::vec4 frustumCorners[NUM_CORNERS] = {
            glm::vec4(xn,   yn, -_shadowZDepth[i], 1.0),
            glm::vec4(-xn,  yn, -_shadowZDepth[i], 1.0),
            glm::vec4(xn,  -yn, -_shadowZDepth[i], 1.0),
            glm::vec4(-xn, -yn, -_shadowZDepth[i], 1.0),

            // far face
            glm::vec4(xf,   yf, -_shadowZDepth[i + 1], 1.0),
            glm::vec4(-xf,  yf, -_shadowZDepth[i + 1], 1.0),
            glm::vec4(xf,  -yf, -_shadowZDepth[i + 1], 1.0),
            glm::vec4(-xf, -yf, -_shadowZDepth[i + 1], 1.0)
        };

        glm::vec4 frustumCornersLight[NUM_CORNERS];

        float minX, minY, minZ;
        minX = minY = minZ = std::numeric_limits<float>::max();
        float maxX, maxY, maxZ;
        maxX = maxY = maxZ = -std::numeric_limits<float>::max();
        glm::vec4 vW;
        for (int j = 0; j < NUM_CORNERS; j++) {
            vW = CameraInvView * frustumCorners[j];
            frustumCornersLight[j] = _lightView * vW;

            minX = std::min(minX, frustumCornersLight[j].x);
            maxX = std::max(maxX, frustumCornersLight[j].x);
            minY = std::min(minY, frustumCornersLight[j].y);
            maxY = std::max(maxY, frustumCornersLight[j].y);
            minZ = std::min(minZ, -frustumCornersLight[j].z);
            maxZ = std::max(maxZ, -frustumCornersLight[j].z);
        }
        float thisEdge = edge*(float)pow(2,i);
        _orthos.push_back(glm::ortho(minX - thisEdge, maxX + thisEdge, minY - thisEdge, maxY + thisEdge, minZ - thisEdge, maxZ+ thisEdge));
        //_orthos.push_back(glm::ortho(minX, maxX, minY, maxY, minZ, maxZ));

    }


}

const glm::mat4 MoonEngine::ShadowMaps::getOrtho(int shadowLevel)
{
    return _orthos[shadowLevel];
}

const float ShadowMaps::getShadowZ(int shadowLevel) {
    return _shadowZDepth[shadowLevel + 1];
}

void MoonEngine::ShadowMaps::setMidpoints(float level1, float level2)
{
    _shadowZDepth[1] = level1;
    _shadowZDepth[2] = level2;
}

const glm::mat4 MoonEngine::ShadowMaps::getLightView()
{
    return _lightView;
}

void ShadowMaps::DBG_DrawToImgui()
{
    ImGui::Begin("Shadow Maps");
    for (unsigned i = 0; i < NUM_SHADOWS; i++)
    {
        texture_unit id = getTextureUnit(SHADOW_TEXTURE + to_string(i));
        ImGui::Image((void *)id.gl_texture->getTextureId(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();

}

