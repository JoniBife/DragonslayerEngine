#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include "Material.h"
#include "../textures/Texture2D.h"

namespace core {

	class PBRMaterial : public Material {

	private:
		Vec3  albedoTint = { 0.0f, 0.0f, 0.0f }; GLint albedoTintLocation = -1;
		float normalStrength = 1.0f; GLint normalStrengthLocation = -1;
		float metallic = 1.0f; GLint metallicLocation = -1;
		float roughness = 1.0f; GLint roughnessLocation = -1;
		float ao = 1.0f; GLint aoLocation = -1; // ambient occlusion
		Texture2D* albedoMap = nullptr; GLint albedoMapLocation = -1;
		Texture2D* normalMap = nullptr; GLint normalMapLocation = -1;
		Texture2D* metallicMap = nullptr; GLint metallicMapLocation = -1;
		Texture2D* roughnessMap = nullptr; GLint roughnessMapLocation = -1;
		Texture2D* aoMap = nullptr; GLint aoMapLocation = -1;

	public:
		PBRMaterial();
		~PBRMaterial();

		void sendParametersToShader(const Vec3& viewPosition, const Mat4& modelMatrix, const Mat3& normalMatrix = Mat3::IDENTITY) override;

		void onGUI() override;

		void bindTextures() override;
		void unBindTextures() override;
	};

}

#endif
