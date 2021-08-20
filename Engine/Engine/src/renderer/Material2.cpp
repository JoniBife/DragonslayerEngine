#include "Material.h"
#include <assert.h>

using namespace renderer;

renderer::Material::Material(GLMaterial* glMaterial) : glMaterial(glMaterial)
{

}

GLMaterial& renderer::Material::getOpenGLMaterial() const
{
	assert(glMaterial != nullptr);
	return *glMaterial;
}