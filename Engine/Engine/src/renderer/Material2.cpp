#include "Material.h"

renderer::Material::Material()
{
    albedoMap = new Texture2D("../Engine/textures/pbr/rustediron2/albedo.png");
    normalMap = new Texture2D("../Engine/textures/pbr/rustediron2/normal.png");
    metallicMap = new Texture2D("../Engine/textures/pbr/rustediron2/metallic.png");
    roughnessMap = new Texture2D("../Engine/textures/pbr/rustediron2/roughness.png");
    aoMap = new Texture2D("../Engine/textures/pbr/rustediron2/ao.png");
}