#include "GLPBRMaterial.h"

renderer::GLPBRMaterial::GLPBRMaterial(Texture2D& defaultAlbedoMap,
	Texture2D& defaultNormalMap,
	Texture2D& defaultMetallicMap,
	Texture2D& defaultRoughnessMap,
	Texture2D& defaultAOMap) :
	albedoMap(defaultAlbedoMap), 
	normalMap(defaultNormalMap), 
	metallicMap(defaultMetallicMap),
	roughnessMap(defaultRoughnessMap),
	aoMap(defaultAOMap) {
	
}

void renderer::GLPBRMaterial::setAlbedoTint(const Vec3& albedoTint)
{
	this->albedoTint = albedoTint;
}

void renderer::GLPBRMaterial::setNormalStrength(float normalStrength)
{
	this->normalStrength = normalStrength;
}

void renderer::GLPBRMaterial::setMetallic(float metallic)
{
	this->metallic = metallic;
}

void renderer::GLPBRMaterial::setRoughness(float roughness)
{
	this->roughness = roughness;
}

void renderer::GLPBRMaterial::setAO(float ao)
{
	this->ao = ao;
}

void renderer::GLPBRMaterial::setAlbedoMap(Texture2D& texture2D)
{
	this->albedoMap= texture2D;
}

void renderer::GLPBRMaterial::setNormalMap(Texture2D& texture2D)
{
	this->normalMap = texture2D;
}

void renderer::GLPBRMaterial::setMetallicMap(Texture2D& texture2D)
{
	this->metallicMap = texture2D;
}

void renderer::GLPBRMaterial::setRoughnessMap(Texture2D& texture2D)
{
	this->roughnessMap = texture2D;
}

void renderer::GLPBRMaterial::setAOMap(Texture2D& texture2D)
{
	this->aoMap = texture2D;
}

Texture2D& renderer::GLPBRMaterial::getAlbedoMap() const
{
	return albedoMap;
}

Texture2D& renderer::GLPBRMaterial::getNormalMap() const
{
	return normalMap;
}

Texture2D& renderer::GLPBRMaterial::getMetallicMap() const
{
	return metallicMap;
}

Texture2D& renderer::GLPBRMaterial::getRoughnessMap() const
{
	return roughnessMap;
}

Texture2D& renderer::GLPBRMaterial::getAOMap() const
{
	return aoMap;
}

Vec3 renderer::GLPBRMaterial::getAlbedoTint() const
{
	return albedoTint;
}

float renderer::GLPBRMaterial::getNormalStrength() const
{
	return normalStrength;
}

float renderer::GLPBRMaterial::getMetallic() const
{
	return metallic;
}

float renderer::GLPBRMaterial::getRoughness() const
{
	return roughness;
}

float renderer::GLPBRMaterial::getAO() const
{
	return ao;
}
