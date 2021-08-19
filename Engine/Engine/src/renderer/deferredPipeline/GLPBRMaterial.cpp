#include "GLPBRMaterial.h"

renderer::GLPBRMaterial::GLPBRMaterial(ShaderProgram* geometryShaderProgram, ShaderProgram* lightingShaderProgram) : 
	geometryShaderProgram(geometryShaderProgram)
{
	
}

void renderer::GLPBRMaterial::sendParametersToGeometryShaderProgram(const Mat4& model, const Mat3& normal)
{
	albedoMap->bind(GL_TEXTURE0);
	normalMap->bind(GL_TEXTURE1);
	metallicMap->bind(GL_TEXTURE2);
	roughnessMap->bind(GL_TEXTURE3);
	aoMap->bind(GL_TEXTURE4);

	geometryShaderProgram->setUniform("modelMatrix", model);
	geometryShaderProgram->setUniform("normalMatrix", normal);
	geometryShaderProgram->setUniform("albedoTint", albedoTint);
	geometryShaderProgram->setUniform("normalStrength", normalStrength);
	geometryShaderProgram->setUniform("metallicFactor", metallic);
	geometryShaderProgram->setUniform("roughnessFactor", roughness);
	geometryShaderProgram->setUniform("aoFactor", ao);
}

ShaderProgram& renderer::GLPBRMaterial::getGeometryShaderProgram() const
{
	return *geometryShaderProgram;
}
