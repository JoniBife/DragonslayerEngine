#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <iostream>
#include "GL/glew.h"
#include "ITexture.h"

/*
* Usage example:
*
* Texture2D tex = Texture2D::loadFromFile("image.png");
* 
* tex.bind();
* 
* // Draw stuff with texture
* 
* text.unBind();
* 
* delete tex;
*/
class Texture2D : public ITexture {

private:
	int width = 0, height = 0, nrChannels = 0; // In RGB there are 3 channels while in RGBA there are 4 for example
	GLint internalFormat;

	Texture2D();

public:

	Texture2D(const std::string& textureFilePath);

	Texture2D(const std::string& textureFilePath, GLint param);

	~Texture2D();

	static Texture2D* emptyTexture(unsigned int width, unsigned int height, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE);
	static Texture2D* depthTexture(unsigned int width, unsigned int height, GLenum type = GL_FLOAT);

	static Texture2D* fromFloatArrayFile(const std::string& textureFilePath, unsigned int width, unsigned int height);

	void bind(unsigned int unitNumber) override;

	void unBind(unsigned int unitNumber) override;
	
	int getWidth() const;
	int getHeight() const;

	void resize(unsigned int width, unsigned int height);
};

#endif
