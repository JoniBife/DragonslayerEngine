#include <assert.h>
#include "Material.h"
#include "..\renderer\Material.h"

using namespace core;

Material::Material(const std::string& name) : name(name) {}

Material::~Material()
{
	if (shaderProgram != nullptr)
		delete shaderProgram;
}

ShaderProgram& Material::getShaderProgram() const
{
	assert(shaderProgram != nullptr);
	return *shaderProgram;
}

void core::Material::bindTextures()
{
}

void core::Material::unBindTextures()
{
}

void Material::use()
{
	assert(shaderProgram != nullptr);
	shaderProgram->use();
}

void Material::stopUsing()
{
	assert(shaderProgram != nullptr);
	shaderProgram->stopUsing();
}

std::string Material::getName() const
{
	return name;
}
