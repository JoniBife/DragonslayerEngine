#include "Material.h"

WarriorRenderer::Material::Material(Texture2D* defaultAlbedoMap,
	Texture2D* defaultNormalMap,
	Texture2D* defaultMetallicMap,
	Texture2D* defaultRoughnessMap,
	Texture2D* defaultAOMap) :
	albedoMap(defaultAlbedoMap), 
	normalMap(defaultNormalMap), 
	metallicMap(defaultMetallicMap),
	roughnessMap(defaultRoughnessMap),
	aoMap(defaultAOMap) {
	
}

void WarriorRenderer::Material::setAlbedoTint(const Vec3& albedoTint)
{
	this->albedoTint = albedoTint;
}

void WarriorRenderer::Material::setNormalStrength(float normalStrength)
{
	this->normalStrength = normalStrength;
}

void WarriorRenderer::Material::setMetallic(float metallic)
{
	this->metallic = metallic;
}

void WarriorRenderer::Material::setRoughness(float roughness)
{
	this->roughness = roughness;
}

void WarriorRenderer::Material::setAO(float ao)
{
	this->ao = ao;
}

void WarriorRenderer::Material::setAlbedoMap(Texture2D* texture2D)
{
	this->albedoMap= texture2D;
}

void WarriorRenderer::Material::setNormalMap(Texture2D* texture2D)
{
	this->normalMap = texture2D;
}

void WarriorRenderer::Material::setMetallicMap(Texture2D* texture2D)
{
	this->metallicMap = texture2D;
}

void WarriorRenderer::Material::setRoughnessMap(Texture2D* texture2D)
{
	this->roughnessMap = texture2D;
}

void WarriorRenderer::Material::setAOMap(Texture2D* texture2D)
{
	this->aoMap = texture2D;
}

Texture2D& WarriorRenderer::Material::getAlbedoMap() const
{
	return *albedoMap;
}

Texture2D& WarriorRenderer::Material::getNormalMap() const
{
	return *normalMap;
}

Texture2D& WarriorRenderer::Material::getMetallicMap() const
{
	return *metallicMap;
}

Texture2D& WarriorRenderer::Material::getRoughnessMap() const
{
	return *roughnessMap;
}

Texture2D& WarriorRenderer::Material::getAOMap() const
{
	return *aoMap;
}

Vec3 WarriorRenderer::Material::getAlbedoTint() const
{
	return albedoTint;
}

float WarriorRenderer::Material::getNormalStrength() const
{
	return normalStrength;
}

float WarriorRenderer::Material::getMetallic() const
{
	return metallic;
}

float WarriorRenderer::Material::getRoughness() const
{
	return roughness;
}

float WarriorRenderer::Material::getAO() const
{
	return ao;
}
