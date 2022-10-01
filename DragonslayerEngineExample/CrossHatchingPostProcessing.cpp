#include "CrossHatchingPostProcessing.h"

CrossHatchingCommand::CrossHatchingCommand() {
    Shader postProcessingVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/postProcessingVS.glsl");
    Shader crossHatchingFS(GL_FRAGMENT_SHADER, "../DragonslayerEngineExample/shaders/crossHatchingFS.glsl");
    shaderProgram = ShaderProgram(postProcessingVS, crossHatchingFS);
    postProcessingVS.deleteObject();
    crossHatchingFS.deleteObject();

    crossHatchingLevels[0] = new Texture2D("../DragonslayerEngineExample/textures/CrossHatchingLevel0.png");
    crossHatchingLevels[1] = new Texture2D("../DragonslayerEngineExample/textures/CrossHatchingLevel1.jpg");
    crossHatchingLevels[2] = new Texture2D("../DragonslayerEngineExample/textures/CrossHatchingLevel2.jpg");
}

bool CrossHatchingCommand::isValid() const {
    return true;
}

void CrossHatchingCommand::sendParametersToShader(const Camera &camera, const FrameBuffer &gBuffer,
                                                         Texture2D &previousRenderTexture) {
    shaderProgram.setUniform("previousRenderTexture", 0);
    previousRenderTexture.bind(GL_TEXTURE0);
    shaderProgram.setUniform("gBufferLocalPositionMetallic", 1);
    gBuffer.getColorAttachment(0).bind(GL_TEXTURE1);
    shaderProgram.setUniform("gBufferNormalRoughness", 2);
    gBuffer.getColorAttachment(1).bind(GL_TEXTURE2);

    for (int i = 0; i < 3; ++i) {
        shaderProgram.setUniform("crossHatchingLevels[" + std::to_string(i) + "]", 3 + i);
        crossHatchingLevels[i]->bind(GL_TEXTURE3 + i);
    }
}
