#include "SkyBox.h"
#include "../view/Camera.h"
#include "../utils/OpenGLUtils.h"

SkyBox::SkyBox(const std::vector<std::string>& facesFilePath, Camera* camera) {

    cube = new Mesh({        
        {-1.0f,  1.0f, -1.0f ,1.0f},
        {-1.0f, -1.0f, -1.0f ,1.0f},
        { 1.0f, -1.0f, -1.0f ,1.0f},
        { 1.0f, -1.0f, -1.0f ,1.0f},
        { 1.0f,  1.0f, -1.0f ,1.0f},
        {-1.0f,  1.0f, -1.0f ,1.0f},
                             
        {-1.0f, -1.0f,  1.0f ,1.0f},
        {-1.0f, -1.0f, -1.0f ,1.0f},
        {-1.0f,  1.0f, -1.0f ,1.0f},
        {-1.0f,  1.0f, -1.0f ,1.0f},
        {-1.0f,  1.0f,  1.0f ,1.0f},
        {-1.0f, -1.0f,  1.0f ,1.0f},
                             
        { 1.0f, -1.0f, -1.0f ,1.0f},
        { 1.0f, -1.0f,  1.0f ,1.0f},
        { 1.0f,  1.0f,  1.0f ,1.0f},
        { 1.0f,  1.0f,  1.0f ,1.0f},
        { 1.0f,  1.0f, -1.0f ,1.0f},
        { 1.0f, -1.0f, -1.0f ,1.0f},
                             
        {-1.0f, -1.0f,  1.0f ,1.0f},
        {-1.0f,  1.0f,  1.0f ,1.0f},
        { 1.0f,  1.0f,  1.0f ,1.0f},
        { 1.0f,  1.0f,  1.0f ,1.0f},
        { 1.0f, -1.0f,  1.0f ,1.0f},
        {-1.0f, -1.0f,  1.0f ,1.0f},
                             
        {-1.0f,  1.0f, -1.0f ,1.0f},
        { 1.0f,  1.0f, -1.0f ,1.0f},
        { 1.0f,  1.0f,  1.0f ,1.0f},
        { 1.0f,  1.0f,  1.0f ,1.0f},
        {-1.0f,  1.0f,  1.0f ,1.0f},
        {-1.0f,  1.0f, -1.0f ,1.0f},
                             
        {-1.0f, -1.0f, -1.0f ,1.0f},
        {-1.0f, -1.0f,  1.0f ,1.0f},
        { 1.0f, -1.0f, -1.0f ,1.0f},
        { 1.0f, -1.0f, -1.0f ,1.0f},
        {-1.0f, -1.0f,  1.0f ,1.0f},
        { 1.0f, -1.0f,  1.0f ,1.0f}
    });
    cube->init();

	cubemap = new CubeMap(facesFilePath);

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/skybox/vertexShaderSkyBox.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/skybox/fragmentShaderSkyBox.glsl");
	skyboxShader = new ShaderProgram(vs, fs);
    // Associating the shared matrix index with the binding point of the camera (0)
    GLuint sharedMatricesIndex = skyboxShader->getUniformBlockIndex("SharedMatrices");
    skyboxShader->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

    skyBoxLocation = skyboxShader->getUniformLocation("skybox");
    skyboxShader->use();
    skyboxShader->setUniform(skyBoxLocation, 0);
    skyboxShader->stopUsing();
}

SkyBox::~SkyBox() {
    delete cube;
    delete cubemap;
    delete skyboxShader;
}

void SkyBox::init() { }
void SkyBox::bind() {   
}
void SkyBox::unBind() {   
}
void SkyBox::draw() {
    skyboxShader->use();
    cube->bind();
    cubemap->bind(GL_TEXTURE0);

    GL_CALL(glDepthMask(GL_FALSE));
    cube->draw();
    GL_CALL(glDepthMask(GL_TRUE));

    cubemap->unBind(GL_TEXTURE0);
    cube->unBind();
    skyboxShader->stopUsing();
}