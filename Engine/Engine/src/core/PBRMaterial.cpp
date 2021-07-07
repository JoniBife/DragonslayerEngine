#include "PBRMaterial.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

using namespace core;

PBRMaterial::PBRMaterial() : Material("PBR Material")
{
    Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/pbr/pbrVS.glsl");
    Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/pbr/pbrFS.glsl");
    shaderProgram = new ShaderProgram(vs, fs);

    albedoMap = new Texture2D("../Engine/textures/pbr/rustediron2/albedo.png");
    normalMap = new Texture2D("../Engine/textures/pbr/rustediron2/normal.png");
    metallicMap = new Texture2D("../Engine/textures/pbr/rustediron2/metallic.png");
    roughnessMap = new Texture2D("../Engine/textures/pbr/rustediron2/roughness.png");
    aoMap = new Texture2D("../Engine/textures/pbr/rustediron2/ao.png");

    albedoTintLocation = shaderProgram->getUniformLocation("albedoTint");
    normalStrengthLocation = shaderProgram->getUniformLocation("normalStrength");
    metallicLocation = shaderProgram->getUniformLocation("metallicFactor");
    roughnessLocation = shaderProgram->getUniformLocation("roughnessFactor");
    aoLocation = shaderProgram->getUniformLocation("aoFactor");

    albedoMapLocation = shaderProgram->getUniformLocation("albedoMap");
    normalMapLocation = shaderProgram->getUniformLocation("normalMap");
    metallicMapLocation = shaderProgram->getUniformLocation("metallicMap");
    roughnessMapLocation = shaderProgram->getUniformLocation("roughnessMap");
    aoMapLocation = shaderProgram->getUniformLocation("aoMap");
  

    modelMatrixLocation = shaderProgram->getUniformLocation("model");
    normalMatrixLocation = shaderProgram->getUniformLocation("normal");
    viewPositionLocation = shaderProgram->getUniformLocation("viewPosition");

    shaderProgram->bindUniformBlock(
        shaderProgram->getUniformBlockIndex("sharedMatrices"),
        0 // TODO Should not be hardcoded
    );
}

PBRMaterial::~PBRMaterial()
{
}

void PBRMaterial::sendParametersToShader(const Vec3& viewPosition, const Mat4& modelMatrix, const Mat3& normalMatrix)
{
    shaderProgram->setUniform(albedoTintLocation, albedoTint);
    shaderProgram->setUniform(normalStrengthLocation, normalStrength);
    shaderProgram->setUniform(metallicLocation, metallic);
    shaderProgram->setUniform(roughnessLocation, roughness);
    shaderProgram->setUniform(aoLocation, ao);

    shaderProgram->setUniform(albedoMapLocation, 0);
    shaderProgram->setUniform(normalMapLocation, 1);
    shaderProgram->setUniform(metallicMapLocation, 2);
    shaderProgram->setUniform(roughnessMapLocation, 3);
    shaderProgram->setUniform(aoMapLocation, 4);

    shaderProgram->setUniform(modelMatrixLocation, modelMatrix);
    shaderProgram->setUniform(normalMatrixLocation, normalMatrix);

    // TODO Move this to the uniform block that contains the shared matrices
    shaderProgram->setUniform(viewPositionLocation, viewPosition);
}

void PBRMaterial::onGUI()
{
    float color[3];

    albedoTint.toOpenGLFormat(color);

    ImGui::ColorEdit3("Albedo", color);

    albedoTint.x = color[0];
    albedoTint.y = color[1];
    albedoTint.z = color[2];

    ImGui::DragFloat("Metallic", &metallic, 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat("Roughness", &roughness, 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat("AO", &ao, 0.05f, 0.0f, 1.0f);
}

void core::PBRMaterial::bindTextures()
{
    unsigned int textureNumber = GL_TEXTURE0;
    if (albedoMap != nullptr) {
        albedoMap->bind(textureNumber);
        ++textureNumber;
    } 

    if (normalMap != nullptr) {
        normalMap->bind(textureNumber);
        ++textureNumber;
    }

    if (metallicMap != nullptr) {
        metallicMap->bind(textureNumber);
        ++textureNumber;
    }

    if (roughnessMap != nullptr) {
        roughnessMap->bind(textureNumber);
        ++textureNumber;
    }

    if (aoMap != nullptr) {
        aoMap->bind(textureNumber);
    }
}

void core::PBRMaterial::unBindTextures()
{
    unsigned int textureNumber = GL_TEXTURE0;
    if (albedoMap != nullptr) {
        albedoMap->unBind(textureNumber);
        ++textureNumber;
    }

    if (normalMap != nullptr) {
        normalMap->unBind(textureNumber);
        ++textureNumber;
    }

    if (metallicMap != nullptr) {
        metallicMap->unBind(textureNumber);
        ++textureNumber;
    }

    if (roughnessMap != nullptr) {
        roughnessMap->unBind(textureNumber);
        ++textureNumber;
    }

    if (aoMap != nullptr) {
        aoMap->unBind(textureNumber);
    }
}
