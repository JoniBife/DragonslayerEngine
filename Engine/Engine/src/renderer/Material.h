
#ifndef MATERIAL_R_H
#define MATERIAL_R_H

#include "../math/Vec3.h"
#include <gl/glew.h>
#include "../textures/Texture2D.h"

namespace renderer {

	class Material {

	public:
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

		Material();
		~Material();

	};

}

#endif
