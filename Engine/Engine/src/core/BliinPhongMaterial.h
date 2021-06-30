#ifndef BLIIN_PHONG_MATERIAL_H
#define BLIIN_PHONG_MATERIAL_H

#include "Material.h"
#include "../math/Vec3.h"

class BliinPhongMaterial : public Material {

private: 
	float ambientStrength = 0.1f;
	float diffuseStrength = 1.0f;
	float specularStrength = 0.7f;
	unsigned int shininess = 32;

	Vec3 color;

public:
	void setAmbientStrength(float ambientStrength);
	void setDiffuseStrength(float diffuseStrength);
	void setSpecularStrength(float specularStrength);
	void setShininess(float shininess);

	float getAmbientStrength() const;
	float getDiffuseStrength() const;
	float getSpecularStrength() const;
	unsigned int getShininess() const;

	BliinPhongMaterial();

};

#endif
