#ifndef CROSS_HATCHING_POSTPROCESSING_H
#define CROSS_HATCHING_POSTPROCESSING_H

#include <DragonslayerEngine/PostProcessingCommand.h>
#include <DragonslayerEngine/Texture2D.h>

using namespace DragonslayerEngine;

class CrossHatchingCommand : public PostProcessingCommand {

private:
    Texture2D* crossHatchingLevels[3];

public:
    CrossHatchingCommand();

    bool isValid() const override;
    void sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) override;
};


#endif //CROSS_HATCHING_POSTPROCESSING_H
