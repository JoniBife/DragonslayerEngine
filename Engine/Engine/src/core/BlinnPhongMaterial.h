#ifndef BLINN_PHONG_MATERIAL_H
#define BLINN_PHONG_MATERIAL_H

#include "Material.h"
#include "../math/Vec3.h"

namespace core {

	class BlinnPhongMaterial : public Material {

	private:
		float ambientStrength = 0.1f; GLint ambientLocation;
		float diffuseStrength = 1.0f; GLint diffuseLocation;
		float specularStrength = 0.7f; GLint specularLocation;
		unsigned int shininess = 32; GLint shininessLocation;

		Vec3 color; GLint colorLocation;

	public:
		void setAmbientStrength(float ambientStrength);
		void setDiffuseStrength(float diffuseStrength);
		void setSpecularStrength(float specularStrength);
		void setShininess(float shininess);

		float getAmbientStrength() const;
		float getDiffuseStrength() const;
		float getSpecularStrength() const;
		unsigned int getShininess() const;

		BlinnPhongMaterial();
		~BlinnPhongMaterial();

		void sendParametersToShader(const Vec3& viewPosition, const Mat4& modelMatrix, const Mat3& normalMatrix = Mat3::IDENTITY) override;

	};

}

#endif
