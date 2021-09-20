#ifndef OPEN_GL_STATE_H
#define OPEN_GL_STATE_H

#include <GL/glew.h>
#include "shaders/shaderProgram.h"

namespace WarriorRenderer {

    /*
    * Contains most of the active OpenGL state, avoids any unecessary state changes 
    * Could not find confirmation anywhere but switching OpenGL state probably has 
    * some cost
    * 
    * TODO Improve this class by fetching the values using glGet and glIsEnabled
    */
	class OpenGLState {
		
	private:
        int activeShaderProgram = -1;
        bool depthTesting = true;
        GLenum depthFunction = GL_LESS;
        GLboolean depthMask = GL_TRUE;
        GLclampd zNear = 0.0f;
        GLclampd zFar = 1.0f;
        bool faceCulling = true;
        GLenum cullFace = GL_BACK; 
        GLenum frontFace = GL_CCW;
        Vec4 clearColor = { 0.8f, 0.8f, 0.8f, 1.0f }; // Interally uses GLclampf which translates to float so its fine using Vec4
        bool blending = false;
        GLint x = 0;
        GLint y = 0;
        GLsizei width = 1366;
        GLsizei height = 768;

    public:
        OpenGLState();
        ~OpenGLState();

        /* Sets all the state to the default values (currently specified as the default field values) */
        void setToDefaultState();
        
        void setDepthTesting(bool enable);
        void setDepthFunction(GLenum depthFunction);
        void setDepthMask(GLboolean enable);
        void setDepthRange(GLclampd zNear, GLclampd zFar);
        void setFaceCulling(bool enable);
        void setCullFace(GLenum face);
        void setFrontFace(GLenum frontFace);
        void setClearColor(Vec4 clearColor);
        void setViewPort(GLint x, GLint y, GLsizei width, GLsizei height);
        void setBlending(bool enable);
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
        Vec4 getClearColor() const;
        void getViewPort(GLint& x, GLint& y, GLsizei& width, GLsizei& height) const;
        bool getBlending() const;

	};

}

#endif
