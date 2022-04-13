#include "DragonslayerEngine/PostProcessingCommand.h"
#include <random>
#include <string>
#include "DragonslayerEngine/utils/OpenGLUtils.h"
#include <LMath/MathAux.hpp>

using namespace LMath;

DragonslayerEngine::PostProcessingCommand::~PostProcessingCommand()
{
	shaderProgram.deleteObject();
}

ShaderProgram& DragonslayerEngine::PostProcessingCommand::getShader()
{
	return shaderProgram;
}

DragonslayerEngine::FxaaCommand::FxaaCommand()
{
	Shader postProcessingVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/postProcessingVS.glsl");
	Shader fxaaFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/fxaaFS.glsl");
	shaderProgram = ShaderProgram(postProcessingVS, fxaaFS);
	postProcessingVS.deleteObject();
	fxaaFS.deleteObject();
}

bool DragonslayerEngine::FxaaCommand::isValid() const
{
	return minEdgeThreshold > 0.0f && maxEdgeThreshold > 0.0f;
}

void DragonslayerEngine::FxaaCommand::sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture)
{
	shaderProgram.setUniform("minEdgeThreshold", minEdgeThreshold);
	shaderProgram.setUniform("maxEdgeThreshold", maxEdgeThreshold);
	shaderProgram.setUniform("previousRenderTexture", 0);
	previousRenderTexture.bind(GL_TEXTURE0);
}

DragonslayerEngine::ACESToneMappingCommand::ACESToneMappingCommand()
{
	Shader postProcessingVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/postProcessingVS.glsl");
	Shader toneMappingFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/acesToneMappingFS.glsl");
	shaderProgram = ShaderProgram(postProcessingVS, toneMappingFS);
	postProcessingVS.deleteObject();
	toneMappingFS.deleteObject();
}

bool DragonslayerEngine::ACESToneMappingCommand::isValid() const
{
	return true;
}

void DragonslayerEngine::ACESToneMappingCommand::sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture)
{
	shaderProgram.setUniform("previousRenderTexture", 0);
	previousRenderTexture.bind(GL_TEXTURE0);
}

DragonslayerEngine::ReinhardToneMappingCommand::ReinhardToneMappingCommand()
{
	Shader postProcessingVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/postProcessingVS.glsl");
	Shader toneMappingFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/reinhardToneMappingFS.glsl");
	shaderProgram = ShaderProgram(postProcessingVS, toneMappingFS);
	postProcessingVS.deleteObject();
	toneMappingFS.deleteObject();
}

bool DragonslayerEngine::ReinhardToneMappingCommand::isValid() const
{
	return true;
}

void DragonslayerEngine::ReinhardToneMappingCommand::sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture)
{
	shaderProgram.setUniform("previousRenderTexture", 0);
	previousRenderTexture.bind(GL_TEXTURE0);
}

DragonslayerEngine::SSAOCommand::SSAOCommand(unsigned int sampleSize, unsigned int noiseSize) : sampleSize(sampleSize), noiseSize(noiseSize)
{
	Shader postProcessingVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/postProcessingVS.glsl");
	Shader ssaoFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/ssaoFS.glsl");
	shaderProgram = ShaderProgram(postProcessingVS, ssaoFS);
	postProcessingVS.deleteObject();
	ssaoFS.deleteObject();

	// Not using rand because it's result is biased when using the mod operator
	// https://stackoverflow.com/questions/7114043/random-number-generation-in-c11-how-to-generate-how-does-it-work
	std::uniform_real_distribution<float> dist(0.0, 1.0); // random floats between [0.0, 1.0]
	std::default_random_engine generator;

	// Generating samples in Tangent space around the normal oriented hemisphere (i.e. the z sample component is within the [0,1] interval)
	for (unsigned int i = 0; i < sampleSize; ++i)
	{
		Vec3 sample(
			dist(generator) * 2.0 - 1.0,
			dist(generator) * 2.0 - 1.0,
			dist(generator)
		);
		sample = sample.normalize();
		sample *= dist(generator);

		float scale = (float)i / sampleSize;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		samples.push_back(sample);
	}

	std::vector<Vec2> noise;
	// Generating a noise texture
	for (unsigned int i = 0; i < noiseSize * noiseSize; ++i)
	{
		Vec2 randomVec(
			dist(generator) * 2.0 - 1.0,
			dist(generator) * 2.0 - 1.0
		);
		noise.push_back(randomVec);
	}

	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, noiseSize, noiseSize, 0, GL_RG, GL_FLOAT, &noise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // We are tiling the texture over the whole screen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

bool DragonslayerEngine::SSAOCommand::isValid() const
{
	return sampleSize > 0u && sampleSize <= 64u;
}

void DragonslayerEngine::SSAOCommand::sendParametersToShader(const Camera& camera, const FrameBuffer& gBuffer, Texture2D& previousRenderTexture)
{
	shaderProgram.setUniform("previousRenderTexture", 0);
	previousRenderTexture.bind(GL_TEXTURE0);

	shaderProgram.setUniform("gBufferNormalRoughness", 1);
	gBuffer.getColorAttachment(1).bind(GL_TEXTURE1);

	shaderProgram.setUniform("gBufferDepth", 2);
	gBuffer.getDepthAttachment().bind(GL_TEXTURE2);

	shaderProgram.setUniform("noise", 3);
	GL_CALL(glActiveTexture(GL_TEXTURE3));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, noiseTexture));

	for (unsigned int i = 0u; i < sampleSize; ++i) {
		std::string uniformName = "samples[" + std::to_string(i) + "]";
		shaderProgram.setUniform(uniformName, samples[i]);
	}
	shaderProgram.setUniform("sampleSize", sampleSize);

	shaderProgram.setUniform("projectionMatrix", camera.getProjection());
	//shaderProgram.setUniform("viewMatrix", camera.getView());
	Mat4 inverseProjection;
	camera.getProjection().inverse(inverseProjection);
	Mat4 inverseView;
	camera.getView().inverse(inverseView);
	shaderProgram.setUniform("inverseProjectionMatrix", inverseProjection);
}

