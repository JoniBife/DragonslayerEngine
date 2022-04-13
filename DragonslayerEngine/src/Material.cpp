#include "DragonslayerEngine/Material.h"

DragonslayerEngine::Material::Material(Texture2D* defaultAlbedoMap,
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

void DragonslayerEngine::Material::setAlbedoTint(const Vec3& albedoTint)
{
	this->albedoTint = albedoTint;
}

void DragonslayerEngine::Material::setNormalStrength(float normalStrength)
{
	this->normalStrength = normalStrength;
}

void DragonslayerEngine::Material::setMetallic(float metallic)
{
	this->metallic = metallic;
}

void DragonslayerEngine::Material::setRoughness(float roughness)
{
	this->roughness = roughness;
}

void DragonslayerEngine::Material::setAO(float ao)
{
	this->ao = ao;
}

void DragonslayerEngine::Material::setAlbedoMap(Texture2D* texture2D)
{
	this->albedoMap= texture2D;
}

void DragonslayerEngine::Material::setNormalMap(Texture2D* texture2D)
{
	this->normalMap = texture2D;
}

void DragonslayerEngine::Material::setMetallicMap(Texture2D* texture2D)
{
	this->metallicMap = texture2D;
}

void DragonslayerEngine::Material::setRoughnessMap(Texture2D* texture2D)
{
	this->roughnessMap = texture2D;
}

void DragonslayerEngine::Material::setAOMap(Texture2D* texture2D)
{
	this->aoMap = texture2D;
}

Texture2D& DragonslayerEngine::Material::getAlbedoMap() const
{
	return *albedoMap;
}

Texture2D& DragonslayerEngine::Material::getNormalMap() const
{
	return *normalMap;
}

Texture2D& DragonslayerEngine::Material::getMetallicMap() const
{
	return *metallicMap;
}

Texture2D& DragonslayerEngine::Material::getRoughnessMap() const
{
	return *roughnessMap;
}

Texture2D& DragonslayerEngine::Material::getAOMap() const
{
	return *aoMap;
}

Vec3 DragonslayerEngine::Material::getAlbedoTint() const
{
	return albedoTint;
}

float DragonslayerEngine::Material::getNormalStrength() const
{
	return normalStrength;
}

float DragonslayerEngine::Material::getMetallic() const
{
	return metallic;
}

float DragonslayerEngine::Material::getRoughness() const
{
	return roughness;
}

float DragonslayerEngine::Material::getAO() const
{
	return ao;
}
