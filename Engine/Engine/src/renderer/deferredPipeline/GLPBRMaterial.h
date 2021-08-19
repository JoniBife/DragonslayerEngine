
#ifndef GL_PBR_MATERIAL_H
#define GL_PBR_MATERIAL_H

#include "../../math/Vec3.h"
#include "../../textures/Texture2D.h"
#include "../../shaders/shaderProgram.h"
#include "GLMaterial.h"

namespace renderer {

	class GLPBRMaterial : public GLMaterial {

	private:
		ShaderProgram* geometryShaderProgram = nullptr;

		Vec3  albedoTint = { 0.0f, 0.0f, 0.0f }; 
		float normalStrength = 1.0f; 
		float metallic = 1.0f; 
		float roughness = 1.0f; 
		float ao = 1.0f; 
		Texture2D* albedoMap = nullptr;
		Texture2D* normalMap = nullptr; 
		Texture2D* metallicMap = nullptr; 
		Texture2D* roughnessMap = nullptr; 
		Texture2D* aoMap = nullptr; 

	public:
		GLPBRMaterial(ShaderProgram* geometryShaderProgram, ShaderProgram* lightingShaderProgram);
		
		void sendParametersToGeometryShaderProgram(const Mat4& model, const Mat3& normal) override;

		ShaderProgram& getGeometryShaderProgram() const override;

	};
}


#endif
