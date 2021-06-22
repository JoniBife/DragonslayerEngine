#include "CubeMap.h"
#include "stb_image.h"
#include <assert.h>
#include "../utils/OpenGLUtils.h"

CubeMap::CubeMap(const std::vector<std::string>& facesFilePath) {

    // Generating the cubemap
    GL_CALL(glGenTextures(1, &id));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, id));

    // texture wrapping/filtering options
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
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

    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

CubeMap::~CubeMap() {
    GL_CALL(glDeleteTextures(1, &id));
}

void CubeMap::bind(unsigned int unitNumber) {
    GL_CALL(glActiveTexture(unitNumber));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
}

void CubeMap::unBind(unsigned int unitNumber) {
    GL_CALL(glActiveTexture(unitNumber));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}