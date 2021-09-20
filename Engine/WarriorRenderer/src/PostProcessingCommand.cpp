#include "PostProcessingCommand.h"

WarriorRenderer::PostProcessingCommand::~PostProcessingCommand()
{
	shaderProgram.deleteObject();
}

ShaderProgram& WarriorRenderer::PostProcessingCommand::getShader()
{
	return shaderProgram;
}

WarriorRenderer::FxaaCommand::FxaaCommand()
{
	Shader postProcessingVS(GL_VERTEX_SHADER, "../WarriorRenderer/shaders/deferred/postProcessingVS.glsl");
	Shader fxaaFS(GL_FRAGMENT_SHADER, "../WarriorRenderer/shaders/deferred/fxaaFS.glsl");
	shaderProgram = ShaderProgram(postProcessingVS, fxaaFS);
	postProcessingVS.deleteObject();
	fxaaFS.deleteObject();
}

bool WarriorRenderer::FxaaCommand::isValid() const
{
	return minEdgeThreshold > 0.0f && maxEdgeThreshold > 0.0f;
}

void WarriorRenderer::FxaaCommand::sendParametersToShader(const FrameBuffer& gBuffer, Texture2D& previousRenderTexture)
{
	shaderProgram.setUniform("minEdgeThreshold", minEdgeThreshold);
	shaderProgram.setUniform("maxEdgeThreshold", maxEdgeThreshold);
	shaderProgram.setUniform("previousRenderTexture", 0);
	previousRenderTexture.bind(GL_TEXTURE0);
}

WarriorRenderer::ACESToneMappingCommand::ACESToneMappingCommand()
{
	Shader postProcessingVS(GL_VERTEX_SHADER, "../WarriorRenderer/shaders/deferred/postProcessingVS.glsl");
	Shader toneMappingFS(GL_FRAGMENT_SHADER, "../WarriorRenderer/shaders/deferred/acesToneMappingFS.glsl");
	shaderProgram = ShaderProgram(postProcessingVS, toneMappingFS);
	postProcessingVS.deleteObject();
	toneMappingFS.deleteObject();
}

bool WarriorRenderer::ACESToneMappingCommand::isValid() const
{
	return true;
}

void WarriorRenderer::ACESToneMappingCommand::sendParametersToShader(const FrameBuffer& gBuffer, Texture2D& previousRenderTexture)
{
	shaderProgram.setUniform("previousRenderTexture", 0);
	previousRenderTexture.bind(GL_TEXTURE0);
}

