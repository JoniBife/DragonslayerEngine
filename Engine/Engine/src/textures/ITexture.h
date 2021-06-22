#ifndef I_TEXTURE_H
#define I_TEXTURE_H

#include <iostream>
#include "GL/glew.h"

class ITexture {

public:
	GLuint id = 0;

public:

	virtual void bind(unsigned int unitNumber) = 0;

	virtual void unBind(unsigned int unitNumber) = 0;
};

#endif
