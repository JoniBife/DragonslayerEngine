
#ifndef GL_PBR_MATERIAL_H
#define GL_PBR_MATERIAL_H

#include "../../math/Vec3.h"
#include "../../textures/Texture2D.h"
#include "../../shaders/shaderProgram.h"
#include "GLMaterial.h"

namespace renderer {

	class GLPBRMaterial {

	private:
		Vec3  albedoTint = { 0.0f, 0.0f, 0.0f }; 
		float normalStrength = 1.0f; 
		float metallic = 1.0f; 
		float roughness = 1.0f; 
		float ao = 1.0f; 
		Texture2D& albedoMap;
		Texture2D& normalMap; 
		Texture2D& metallicMap; 
		Texture2D& roughnessMap; 
		Texture2D& aoMap; 

	public:
		GLPBRMaterial(Texture2D& defaultAlbedoMap, 
			Texture2D& defaultNormalMap,
			Texture2D& defaultMetallicMap,
			Texture2D& defaultRoughnessMap,
			Texture2D& defaultAOMap);

		void setAlbedoTint(const Vec3& albedoTint);
		void setNormalStrength(float normalStrenght);
		void setMetallic(float metallic);
		void setRoughness(float roughness);
		void setAO(float ao);

		void setAlbedoMap(Texture2D& texture2D);
		void setNormalMap(Texture2D& texture2D);
		void setMetallicMap(Texture2D& texture2D);
		void setRoughnessMap(Texture2D& texture2D);
		void setAOMap(Texture2D& texture2D);

		Texture2D& getAlbedoMap() const;
		Texture2D& getNormalMap() const;
		Texture2D& getMetallicMap() const;
		Texture2D& getRoughnessMap() const;
		Texture2D& getAOMap() const;

		Vec3 getAlbedoTint() const;
		float getNormalStrength() const;
		float getMetallic() const;
		float getRoughness() const;
		float getAO() const;
	};
}


#endif
