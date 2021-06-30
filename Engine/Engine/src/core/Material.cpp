#include <assert.h>
#include "Material.h"

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

std::string Material::getName() const
{
	return name;
}
