#include <math.h>
#include <assert.h>
#include "DragonslayerEngine/textures/CubeMap.h"
#include "stb_image.h"
#include "DragonslayerEngine/utils/OpenGLUtils.h"
#include "DragonslayerEngine/textures/FrameBuffer.h"
#include "DragonslayerEngine/textures/FloatArrayFile.h"

CubeMap::CubeMap()
{
}

CubeMap::CubeMap(const std::vector<std::string>& facesFilePath, bool hasMips) {

    // Generating the cubemap
    GL_CALL(glGenTextures(1, &id));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, id));

    // texture wrapping/filtering options
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, hasMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    int width, height, nrChannels;

    for (unsigned int i = 0; i < facesFilePath.size(); i++)
    {
        // Loading image data from textureFilePath
        unsigned char* data = stbi_load(facesFilePath[i].c_str(), &width, &height, &nrChannels, 0);

        // If there is an error while reading the data from the image file this will be NULL
        assert(data != NULL);

        // By adding i we are advancing to the next cubemap faces GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y etc... 
        GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        ));

        // Free current face, no longer necessary
        stbi_image_free(data);
    }

    this->width = width;
    this->height = height;

    if (hasMips) {
        this->hasMips = true;
        GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    }

    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

CubeMap CubeMap::fromFloatArrayFiles(const std::vector<std::string>& facesFilePath, unsigned int width, unsigned int height, bool hasMips)
{
    CubeMap cubeMap;
    
    // Generating the cubemap
    GL_CALL(glGenTextures(1, &cubeMap.id));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.id));

    // texture wrapping/filtering options
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, hasMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    cubeMap.width = width;
    cubeMap.height = height;

    for (unsigned int i = 0; i < facesFilePath.size(); i++)
    {
        // Loading image data from textureFilePath
        float* data = fa::loadFromFile(facesFilePath[i].c_str(), 3 * width * height);

        // If there is an error while reading the data from the image file this will be NULL
        assert(data != NULL);

        // By adding i we are advancing to the next cubemap faces GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y etc... 
        GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data
        ));

        delete[] data;
    }

    if (hasMips) {
        cubeMap.hasMips = true;
        GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    }

    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

    return cubeMap;
}

void CubeMap::_deleteObject()
{
    GL_CALL(glDeleteTextures(1, &id));
}

CubeMap::~CubeMap() {
    id = 0u;
}

void CubeMap::addMip(const std::vector<std::string>& facesFilePath, GLint level)
{
    assert(hasMips);

    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, id));

    int width, height, nrChannels;

    for (unsigned int i = 0; i < facesFilePath.size(); i++)
    {
        // Loading image data from textureFilePath
        unsigned char* data = stbi_load(facesFilePath[i].c_str(), &width, &height, &nrChannels, 0);

        // If there is an error while reading the data from the image file this will be NULL
        assert(data != NULL);

        // By adding i we are advancing to the next cubemap faces GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y etc... 
        GL_CALL(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            level, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data
        ));

        // Free current face, no longer necessary
        stbi_image_free(data);
    }

    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void CubeMap::addMipFromFloatArray(const std::vector<std::string>& facesFilePath, GLint level)
{
    assert(hasMips);

    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, id));

    for (unsigned int i = 0; i < facesFilePath.size(); i++)
    {
        unsigned int mipWidth = width * std::pow(0.5, level); // TODO Where did this power came from (documentation ?)
        unsigned int mipHeight = height * std::pow(0.5, level);

        // Loading image data from textureFilePath
        float* data = fa::loadFromFile(facesFilePath[i].c_str(), 3 * mipWidth * mipHeight);

        // If there is an error while reading the data from the image file this will be NULL
        assert(data != NULL);

        // By adding i we are advancing to the next cubemap faces GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y etc... 
        GL_CALL(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            level, 0, 0, mipWidth, mipHeight, GL_RGB, GL_FLOAT, data
        ));

        // Free current face, no longer necessary
        delete[] data;
    }

    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void CubeMap::bind(unsigned int unitNumber) {
    GL_CALL(glActiveTexture(unitNumber));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
}

void CubeMap::unBind(unsigned int unitNumber) {
    GL_CALL(glActiveTexture(unitNumber));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}


