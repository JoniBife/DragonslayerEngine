#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <assert.h>
#include "../utils/OpenGLUtils.h"
#include "../math/Vec2.h"
#include "../math/Vec3.h"

Texture2D::Texture2D(const std::string& textureFilePath) {
	// Loading image data from textureFilePath
	unsigned char* data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);

	// If there is an error while reading the data from the image file this will be NULL
	assert(data != NULL);

	// Generating the texture
	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	// texture wrapping/filtering options
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	//free image data, no longer necessary
	stbi_image_free(data);
}

Texture2D::Texture2D(const std::string& textureFilePath, GLint param)
{
	// Loading image data from textureFilePath
	unsigned char* data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);

	// If there is an error while reading the data from the image file this will be NULL
	assert(data != NULL);

	// Generating the texture
	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	// texture wrapping/filtering options
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	//free image data, no longer necessary
	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	GL_CALL(glDeleteTextures(1, &id));
}

void Texture2D::bind(unsigned int unitNumber) {
	GL_CALL(glActiveTexture(unitNumber));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture2D::unBind(unsigned int unitNumber) {
	GL_CALL(glActiveTexture(unitNumber));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}


