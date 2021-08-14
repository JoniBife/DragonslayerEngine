#ifndef OPEN_GL_STATE_H
#define OPEN_GL_STATE_H

#include <GL/glew.h>
#include "../shaders/shaderProgram.h"

namespace renderer {

    /*
    * Contains most of the active OpenGL state, avoids any unecessary state changes 
    */
	class OpenGLState {
		
	private:
        int activeShaderProgram = -1;
        bool depthTesting = true;
        GLenum depthFunction = GL_BACK;
        GLboolean depthMask = GL_TRUE;
        GLclampd zNear = 0.0f;
        GLclampd zFar = 1.0f;
        bool faceCulling = true;
        GLenum cullFace = GL_BACK;
        GLenum frontFace = GL_CCW;

        OpenGLState();

    public:

        /* Creates an OpenGLState object and sets all the state to the default values (currently specified as the default field values) */
        static OpenGLState* createDefaultState();
        ~OpenGLState();
           
        void setActiveShaderProgram(const ShaderProgram& shader);
        void setDepthTesting(bool enable);
        void setDepthFunction(GLenum depthFunction);
        void setDepthMask(GLboolean enable);
        void setDepthRange(GLclampd zNear, GLclampd zFar);
        void setFaceCulling(bool enable);
        void setCullFace(GLenum face);
        void setFrontFace(GLenum frontFace);
        // TODO Add more state

        GLuint getActiveShaderProgram() const;
        bool getDepthTesting() const;
        GLenum getDepthFunction() const;
        GLboolean getDepthMask() const;
        GLclampd getDepthNear() const;
        GLclampd getDepthFar() const;
        bool getFaceCulling() const;
        GLenum getCullFace() const;
        GLenum getFrontFace() const;

	};

}

#endif
