#include "SSAOBuffers.h"
#include <random>

SSAOBuffers::SSAOBuffers(int width, int height, int samples) :
_ssaoBuffer(width, height), _ssaoBlurBuffer(width, height)
{
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    for(auto i =0; i < samples; i++) {
        glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f,
            randomFloats(generator) * 2.0f - 1.0f,
            randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        auto scale = static_cast<float>(i) / static_cast<float>(samples);
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        _kernel.push_back(sample);
    }
    GLTextureConfiguration ssaoCFG(width, height, GL_RED, GL_RGB, GL_FLOAT);
    Library::TextureLib->createTexture(SSAO_COLOR_TEXTURE, ssaoCFG);
    Library::TextureLib->createTexture(SSAO_BLUR_TEXTURE, ssaoCFG);
    vector<TexParameter> texParams;
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    texParams.push_back(std::bind(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    _ssaoBuffer.addTexture(SSAO_COLOR_TEXTURE, SSAO_ATTACHMENT, texParams);
    _ssaoBlurBuffer.addTexture(SSAO_BLUR_TEXTURE, SSAO_ATTACHMENT, texParams);
    _ssaoBuffer.status();
    _ssaoBlurBuffer.status();

    genNoiseTex();
}

void SSAOBuffers::bindForSSAO()
{
    _ssaoBuffer.bind(GL_FRAMEBUFFER);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
   /* auto posTex = Library::TextureLib->getTexture(POSITION_TEXTURE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, posTex->getTextureId());
    auto normTex = Library::TextureLib->getTexture(NORMAL_TEXTURE);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normTex->getTextureId());*/
    auto noiseTex = Library::TextureLib->getTexture(SSAO_NOISE_TEXTURE);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, noiseTex->getTextureId());

}

void SSAOBuffers::bindForBlur()
{
    _ssaoBlurBuffer.bind(GL_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT);
    auto id = _ssaoBuffer.getTexture(SSAO_COLOR_TEXTURE);
    auto unit = _ssaoBuffer.getTexUnit(SSAO_COLOR_TEXTURE);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id->getTextureId());

}

void SSAOBuffers::DBG_DrawToImgui(string guiName)
{
    ImGui::Begin(guiName.c_str());
    {
        auto tex = Library::TextureLib->getTexture(SSAO_COLOR_TEXTURE);
        ImGui::Image((void *)tex->getTextureId(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
        tex = Library::TextureLib->getTexture(SSAO_BLUR_TEXTURE);
        ImGui::Image((void *)tex->getTextureId(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
        tex = Library::TextureLib->getTexture(SSAO_NOISE_TEXTURE);
        ImGui::Image((void *)tex->getTextureId(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}

float SSAOBuffers::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

void SSAOBuffers::genNoiseTex()
{
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    for (auto i = 0; i < 16; i++)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f,
            randomFloats(generator) * 2.0f - 1.0f,
            0.0f);
        _noise.push_back(sample);
    }
    GLTextureConfiguration noiseCFG(4, 4, GL_RGB16F, GL_RGB, GL_FLOAT);
    auto tex = Library::TextureLib->createTexture(SSAO_NOISE_TEXTURE, noiseCFG);
    tex->bindRaw();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
