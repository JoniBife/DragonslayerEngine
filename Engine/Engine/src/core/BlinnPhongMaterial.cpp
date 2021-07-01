#include "BlinnPhongMaterial.h"

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
}

BlinnPhongMaterial::~BlinnPhongMaterial()
{
}

void BlinnPhongMaterial::sendParametersToShader(const Mat4& modelMatrix, const Mat3& normalMatrix)
{
    shaderProgram->setUniform(ambientLocation, ambientStrength);
    shaderProgram->setUniform(diffuseLocation, diffuseStrength);
    shaderProgram->setUniform(specularLocation, specularStrength);
    shaderProgram->setUniform(shininessLocation, shininess);

    shaderProgram->setUniform(modelMatrixLocation, modelMatrix);
    shaderProgram->setUniform(normalMatrixLocation, normalMatrix);
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
