#ifndef OPEN_GL_UTILS_H
#define OPEN_GL_UTILS_H

#include <iostream>
#include <GL/glew.h>

const std::string errorString(GLenum error);

/*
* Checks for any OpenGL errors and prints an
* error message for each error.
* 
* After all errors it prints the error parameter.
* 
* Closes the program if an error occurs.
*/
void checkForOpenGLErrors(std::string error);

// TODO Create an ASSERT macro

/*
* Original solution by The Cherno:
* https://www.youtube.com/watch?v=FBbPWSOQ0-w&t=847s&ab_channel=TheCherno
* 
* Checks if the specified called OpenGL function resulted in an error
* if thats the case then place a breakpoint in that function. It also
* prints the error, the line, the file and the function in which the
* error occurred.
*/
#if _DEBUG
#define GL_CALL(x) clearError();\
	x;\
	if(hasOpenGLError(#x,__FILE__, __LINE__)) __debugbreak() // WARNING This function (__debugbreak) is compiler specific 

static void clearError() { while (glGetError() != GL_NO_ERROR); }
static bool hasOpenGLError(const char* function, const char* file, int line) {
	while (GLenum errCode = glGetError()) {
		std::cerr << "OpenGL ERROR [" << errorString(errCode) 
			<< " line: " << line << " file: " << file << " function: " << function << "]." << std::endl;
		return true;
	}
	return false;
}
#else
#define GL_CALL(x) x;
#endif

#endif
