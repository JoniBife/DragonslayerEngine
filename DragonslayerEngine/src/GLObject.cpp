#include "DragonslayerEngine/GLObject.h"

GLObject::GLObject()
{
}

GLObject::~GLObject()
{
}

void GLObject::deleteObject()
{
	if (!deleted) {
		deleted = true;
		_deleteObject();
	}
}

bool GLObject::isDeleted() const
{
	return deleted;
}
