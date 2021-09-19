#ifndef POST_PROCESSING_COMMAND_H
#define POST_PROCESSING_COMMAND_H

#include "../shaders/shaderProgram.h"
#include "../textures/FrameBuffer.h"

namespace WarriorRenderer {

	class PostProcessingCommand {

	protected:
		ShaderProgram shaderProgram;

	public:
		virtual bool isValid() const = 0;
		virtual void sendParametersToShader(const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) = 0;
		virtual ~PostProcessingCommand();
		ShaderProgram& getShader();
	};

	class FxaaCommand : public PostProcessingCommand {
	
	private:
		float minEdgeThreshold = 0.0312f;
		float maxEdgeThreshold = 0.125f;

	public:
		FxaaCommand();

		bool isValid() const override;
		void sendParametersToShader(const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) override;
	};

	class ACESToneMappingCommand : public PostProcessingCommand {

	public:
		ACESToneMappingCommand();

		bool isValid() const override;
		void sendParametersToShader(const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) override;
	};



}

#endif
