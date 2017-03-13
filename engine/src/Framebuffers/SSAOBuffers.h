#pragma once
#include "GLWrapper/GLFramebuffer.h"
#include "GBuffer.h"
class SSAOBuffers
{
public:
    SSAOBuffers(int width, int height, int samples);
    void bindForSSAO();
    void bindForBlur();
    auto getKernel() {
        return _kernel;
    }
    auto getNoise()
    {
        return _noise;
    }
    void DBG_DrawToImgui(string guiName);
private:
    GLFramebuffer _ssaoBuffer;
    GLFramebuffer _ssaoBlurBuffer;
    vector<glm::vec3> _kernel;
    vector<glm::vec3> _noise;
    static float lerp(float a, float b, float f);
    void genNoiseTex();
    GLTexture* _noiseTex;
};