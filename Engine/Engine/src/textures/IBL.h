#ifndef IBL_H
#define IBL_H

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <GL/glew.h>
#include "stb_image.h"
#include "std_image_write.h"
#include "../math/Mat4.h"
#include "../view/Transformations.h"
#include "../shaders/ShaderProgram.h"
#include "../meshes/Mesh.h"

static void screenShotFrame(float width, float height, const std::string& outputPath) {

    GLubyte* data = new GLubyte[3 * width * height];
    memset(data, 255, 3 * width * height);
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); // Read the on-screen pixels into the space allocated above
    //const std::string path = "../Engine/textures/frame.png";

    //stbi_flip_vertically_on_write(true);
    stbi_write_png(outputPath.c_str(), width, height, 3, data, 0);

    free(data);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


/* Namespace containg function used to precompute any irradiance maps
required for PBR, any function call should be done after the creation
of the OpenGL context */

namespace IBL {

	/* Creates a cubemap from an HDRE file (a.k.a Radiance HDR file), each of the faces
	is output to a new file named as: front, back, left, right, top, bottom. Each of the faces
	file is places in the outputPath. Returns false if the hdrFilePath is wrong or if the internal
	format of the input file is not HDR */
	static bool ComputeIrradianceCubeMap(const char* hdrFilePath, const char* outputPath) {

        // Loading HDR file
        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        float* data = stbi_loadf(hdrFilePath, &width, &height, &channels, 0);

        if (data == nullptr)
            return false;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        /*bool cullFacingEnabled = glIsEnabled(GL_CULL_FACE);
        glGet
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);*/

        // Creating OpenGL texture with 16 bit float precision per color channel
        GLuint hdrTexture;
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        // TODO Check if this should be 32F instead
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        // Creating temporary framebuffer to render a single frame 
        // converting from cylindrical map to cube map
        GLuint captureFBO;
        GLuint captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        // Creating cubemap texture to store each of the faces
        unsigned int envCubemap;
        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Setup each of the view matrices to look at the cube faces and projection matrix
        Mat4 viewMatrices[6] = {
            lookAt(Vec3(0.0f,0.0f,0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)),
            lookAt(Vec3(0.0f,0.0f,0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)),
            lookAt(Vec3(0.0f,0.0f,0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)),
            lookAt(Vec3(0.0f,0.0f,0.0f), Vec3(0.0f,-1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f)),
            lookAt(Vec3(0.0f,0.0f,0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f)),
            lookAt(Vec3(0.0f,0.0f,0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, -1.0f, 0.0f))
        };
        Mat4 projection = perspective(PI/2.0f, 1.0f, 0.1f, 10.f); // TODO Maybe reduce far plane

        // Setup shaders used to convert from equirectangular map to cubemap
        Shader convertVS(GL_VERTEX_SHADER, "../Engine/shaders/deferred/eqrtToCubemapVS.glsl");
        Shader convertFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/eqrtToCubemapFS.glsl");
        ShaderProgram convertSp(convertVS, convertFS);

        // Load cube to render each of the faces
        Mesh* cube = Mesh::loadFromFile("../Engine/objs/cube.obj");
        cube->init();

        convertSp.use();
        convertSp.setUniform("projectionMatrix", projection);
        convertSp.setUniform("equirectangularMap", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glViewport(0, 0, 512, 512);

        for (unsigned int i = 0; i < 6; ++i)
        {
            
            convertSp.setUniform("viewMatrix", viewMatrices[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);

            glClearColor(1, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            cube->bind();
            cube->draw();
            cube->unBind();

            // Allocate enough data for 3 channels: RGB
            /*int width = 512;
            int height = 512;
            GLubyte* data = new GLubyte[12 * 512 * 512];
            memset(data, 255, 12 * width * height);

            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, 512, 512, GL_RGB, GL_FLOAT, data); // Read the on-screen pixels into the space allocated above
            const std::string path = std::string(outputPath) + "cubeMap" + std::to_string(i) + ".png";

            // glReadPixels reads pixels, bottom > up so we have to flip in order for the image to look right
            stbi_flip_vertically_on_write(true);
            stbi_write_png(path.c_str(), 512, 512, 3, data, 0);*/
            screenShotFrame(512, 512, std::string(outputPath) + "cubeMap" + std::to_string(i) + ".png");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        convertSp.stopUsing();

        // Creating texture to render the irradiance maps, one at the time
        unsigned int irradianceMapFace;
        glGenTextures(1, &irradianceMapFace);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMapFace);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Setup shaders used to convert from equirectangular map to cubemap
        Shader convolutionVS(GL_VERTEX_SHADER, "../Engine/shaders/deferred/convolutionVS.glsl");
        Shader convolutionFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/convolutionFS.glsl");
        ShaderProgram convolutionSp(convolutionVS, convolutionFS);
        
        // Finally convolute cubemap and save each face to file
        convolutionSp.use();
        convolutionSp.setUniform("projectionMatrix", projection);
        convolutionSp.setUniform("environmentMap", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32); // Resizing RBO, check if this is safe
        
        // Using the same texture for each of the faces. but saving the pixels each iteration
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, irradianceMapFace, 0);

        glViewport(0, 0, 32, 32);

        for (unsigned int i = 0; i < 6; ++i)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            convolutionSp.setUniform("viewMatrix", viewMatrices[i]);
            cube->bind();
            cube->draw();
            cube->unBind();

            screenShotFrame(32, 32, std::string(outputPath) + "face" + std::to_string(i) + ".png");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        convolutionSp.stopUsing();

        delete cube;
        // TODO Free the rest of unused textures, buffers etc..

        return true;
	}

};

#endif
