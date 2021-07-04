#include "BlinnPhongMaterial.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

using namespace core;

unsigned int BlinnPhongMaterial::getShininess() const
{
    return shininess;
}

BlinnPhongMaterial::BlinnPhongMaterial() : Material( "Blinn-Phong Material") {
    
    Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/blinnPhong/blinnPhongVS.glsl");
    Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/blinnPhong/blinnPhongFS.glsl");
    shaderProgram = new ShaderProgram(vs, fs);

    ambientLocation = shaderProgram->getUniformLocation("ambientStrength");
    diffuseLocation = shaderProgram->getUniformLocation("diffuseStrength");
    specularLocation = shaderProgram->getUniformLocation("specularStrength");
    shininessLocation = shaderProgram->getUniformLocation("shininess");

    modelMatrixLocation = shaderProgram->getUniformLocation("model");
    normalMatrixLocation = shaderProgram->getUniformLocation("normal");
    viewPositionLocation = shaderProgram->getUniformLocation("viewPosition");

    shaderProgram->bindUniformBlock(
        shaderProgram->getUniformBlockIndex("sharedMatrices"), 
        0 // TODO Should not be hardcoded
    );
}

BlinnPhongMaterial::~BlinnPhongMaterial()
{
}

void BlinnPhongMaterial::sendParametersToShader(const Vec3& viewPosition, const Mat4& modelMatrix, const Mat3& normalMatrix)
{
    shaderProgram->setUniform(ambientLocation, ambientStrength);
    shaderProgram->setUniform(diffuseLocation, diffuseStrength);
    shaderProgram->setUniform(specularLocation, specularStrength);
    shaderProgram->setUniform(shininessLocation, shininess);

    shaderProgram->setUniform(modelMatrixLocation, modelMatrix);
    shaderProgram->setUniform(normalMatrixLocation, normalMatrix);
    shaderProgram->setUniform(viewPositionLocation, viewPosition);
}

void core::BlinnPhongMaterial::onGUI()
{
    ImGui::DragFloat("Ambient strength", &ambientStrength, 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat("Diffuse strength", &diffuseStrength, 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat("Specular strength", &specularStrength, 0.05f, 0.0f, 1.0f);
    unsigned int min = 0;
    unsigned int max = 1000;
    ImGui::DragScalar("Shininess", ImGuiDataType_U32,&shininess, 2.0f, &min, &max);
}

void BlinnPhongMaterial::setAmbientStrength(float ambientStrength)
{
    this->ambientStrength = ambientStrength;
}

void BlinnPhongMaterial::setDiffuseStrength(float diffuseStrength)
{
    this->diffuseStrength = diffuseStrength;
}

void BlinnPhongMaterial::setSpecularStrength(float specularStrength)
{
    this->specularStrength = specularStrength;
}

void BlinnPhongMaterial::setShininess(float shininess)
{
    this->shininess = shininess;
}

float BlinnPhongMaterial::getAmbientStrength() const
{
    return ambientStrength;
}

float BlinnPhongMaterial::getDiffuseStrength() const
{
    return diffuseStrength;
}

float BlinnPhongMaterial::getSpecularStrength() const
{
    return specularStrength;
}
