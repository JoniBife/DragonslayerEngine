#ifndef GL_OBJECT
#define GL_OBJECT

/*
* Base class for any OpenGL objects (i.e. shader program, framebuffer, cubemap, texture etc...)
* Indicates whether an object has been deleted
*/
class GLObject {

private:
	bool deleted = false;

protected:
	virtual void _deleteObject() = 0;

	virtual ~GLObject();

	GLObject(); // This class cannot be instanciated by itself

public:

	void deleteObject();
	bool isDeleted() const;

};

#endif
