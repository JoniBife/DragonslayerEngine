#include "PMaterial.h"

WarriorRenderer::PMaterial::PMaterial(Texture2D* defaultAlbedoMap,
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

void WarriorRenderer::PMaterial::setAlbedoTint(const Vec3& albedoTint)
{
	this->albedoTint = albedoTint;
}

void WarriorRenderer::PMaterial::setNormalStrength(float normalStrength)
{
	this->normalStrength = normalStrength;
}

void WarriorRenderer::PMaterial::setMetallic(float metallic)
{
	this->metallic = metallic;
}

void WarriorRenderer::PMaterial::setRoughness(float roughness)
{
	this->roughness = roughness;
}

void WarriorRenderer::PMaterial::setAO(float ao)
{
	this->ao = ao;
}

void WarriorRenderer::PMaterial::setAlbedoMap(Texture2D* texture2D)
{
	this->albedoMap= texture2D;
}

void WarriorRenderer::PMaterial::setNormalMap(Texture2D* texture2D)
{
	this->normalMap = texture2D;
}

void WarriorRenderer::PMaterial::setMetallicMap(Texture2D* texture2D)
{
	this->metallicMap = texture2D;
}

void WarriorRenderer::PMaterial::setRoughnessMap(Texture2D* texture2D)
{
	this->roughnessMap = texture2D;
}

void WarriorRenderer::PMaterial::setAOMap(Texture2D* texture2D)
{
	this->aoMap = texture2D;
}

Texture2D& WarriorRenderer::PMaterial::getAlbedoMap() const
{
	return *albedoMap;
}

Texture2D& WarriorRenderer::PMaterial::getNormalMap() const
{
	return *normalMap;
}

Texture2D& WarriorRenderer::PMaterial::getMetallicMap() const
{
	return *metallicMap;
}

Texture2D& WarriorRenderer::PMaterial::getRoughnessMap() const
{
	return *roughnessMap;
}

Texture2D& WarriorRenderer::PMaterial::getAOMap() const
{
	return *aoMap;
}

Vec3 WarriorRenderer::PMaterial::getAlbedoTint() const
{
	return albedoTint;
}

float WarriorRenderer::PMaterial::getNormalStrength() const
{
	return normalStrength;
}

float WarriorRenderer::PMaterial::getMetallic() const
{
	return metallic;
}

float WarriorRenderer::PMaterial::getRoughness() const
{
	return roughness;
}

float WarriorRenderer::PMaterial::getAO() const
{
	return ao;
}
