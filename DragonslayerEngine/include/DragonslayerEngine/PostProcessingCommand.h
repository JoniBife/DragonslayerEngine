#ifndef POST_PROCESSING_COMMAND_H
#define POST_PROCESSING_COMMAND_H

#include "shaders/ShaderProgram.h"
#include "textures/FrameBuffer.h"
#include "DragonslayerEngine/Camera.h"
#include <string>

namespace DragonslayerEngine {

	// TODO Add support for custom framebuffer, for example to apply post processing with a lower resolution
	class PostProcessingCommand {

	protected:
		ShaderProgram shaderProgram;

	public:
		virtual bool isValid() const = 0;
		virtual void sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) = 0;
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
		void sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) override;
	};

	class ACESToneMappingCommand : public PostProcessingCommand {

	public:
		ACESToneMappingCommand();

		bool isValid() const override;
		void sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) override;
	};

	class ReinhardToneMappingCommand : public PostProcessingCommand {

	public:
		ReinhardToneMappingCommand();

		bool isValid() const override;
		void sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) override;
	};

	class SSAOCommand : public PostProcessingCommand {

	private:
		std::vector<Vec3> samples;
		unsigned int sampleSize;
		unsigned int noiseSize;
		GLuint noiseTexture;

	public:
		SSAOCommand(unsigned int sampleSize = 64u, unsigned int noiseSize = 4u);

		bool isValid() const override;
		void sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture) override;
	};
}

#endif
