#include "Material.h"

using namespace renderer;

renderer::Material::Material(GLMaterial* glMaterial) : glMaterial(glMaterial)
{

}

GLMaterial& renderer::Material::getOpenGLMaterial() const
{
	return *glMaterial;
}