#include "OpenGLState.h"
#include "../utils/OpenGLUtils.h"

using namespace renderer;

renderer::OpenGLState::OpenGLState()
{
	if (depthTesting) {
		GL_CALL(glEnable(GL_DEPTH_TEST));
	} else {
		GL_CALL(glDisable(GL_DEPTH_TEST));
	}

	GL_CALL(glDepthFunc(depthFunction));
	GL_CALL(glDepthMask(depthMask));
	GL_CALL(glDepthRange(zNear, zFar));

	if (faceCulling) {
		GL_CALL(glEnable(GL_CULL_FACE));
	} else {
		GL_CALL(glDisable(GL_CULL_FACE));
	}
	GL_CALL(glCullFace(cullFace));
	GL_CALL(glFrontFace(frontFace));

	GL_CALL(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
	GL_CALL(glViewport(x, y, width, height));
}

OpenGLState* renderer::OpenGLState::createDefaultState()
{
	return new OpenGLState();
}

renderer::OpenGLState::~OpenGLState()
{
	// Does nothing for now (TODO Check if it should do something)
}

void OpenGLState::setActiveShaderProgram(const ShaderProgram& shader)
{
	if (shader.getProgramId() != activeShaderProgram) {
		activeShaderProgram = shader.getProgramId();
	}
}

void OpenGLState::setDepthTesting(bool enable)
{
	if (enable != depthTesting) {
		
		depthTesting = enable;

		if (depthTesting) {
			GL_CALL(glEnable(GL_DEPTH_TEST));
		} else {
			GL_CALL(glDisable(GL_DEPTH_TEST));
		}
	}
}

void renderer::OpenGLState::setDepthFunction(GLenum depthFunction)
{
	if (this->depthFunction != depthFunction) {

		this->depthFunction = depthFunction;
		GL_CALL(glDepthFunc(depthFunction));
	}
}

void renderer::OpenGLState::setDepthMask(GLboolean enable)
{
	if (this->depthMask != enable) {

		this->depthMask = enable;
		GL_CALL(glDepthMask(depthMask));
	}
}

void renderer::OpenGLState::setDepthRange(GLclampd zNear, GLclampd zFar)
{
	if (this->zNear != zNear || this->zFar != zFar) {
		
		this->zNear = zNear;
		this->zFar = zFar;

		GL_CALL(glDepthRange(zNear, zFar));
	}
}

void OpenGLState::setFaceCulling(bool enable)
{
	if (enable != faceCulling) {

		faceCulling = enable;

		if (faceCulling) {
			GL_CALL(glEnable(GL_CULL_FACE));
		}
		else {
			GL_CALL(glDisable(GL_CULL_FACE));
		}
	}
}

void renderer::OpenGLState::setCullFace(GLenum face)
{
	if (this->cullFace != face) {

		this->cullFace = face;
		GL_CALL(glCullFace(cullFace));
	}
}

void renderer::OpenGLState::setFrontFace(GLenum frontFace)
{
	if (this->frontFace != frontFace) {

		this->frontFace = frontFace;
		GL_CALL(glFrontFace(frontFace));
	}
}

void renderer::OpenGLState::setClearColor(Vec4 clearColor)
{
	if (this->clearColor != clearColor) {
		this->clearColor = clearColor;
		GL_CALL(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
	}
}

void renderer::OpenGLState::setViewPort(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (this->x != x || this->y != y || this->width != width || this->height != height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		GL_CALL(glViewport(x, y, width, height));
	}
}

GLuint renderer::OpenGLState::getActiveShaderProgram() const
{
	return activeShaderProgram;
}

bool renderer::OpenGLState::getDepthTesting() const
{
	return depthTesting;
}

GLenum renderer::OpenGLState::getDepthFunction() const
{
	return depthFunction;
}

GLboolean renderer::OpenGLState::getDepthMask() const
{
	return depthMask;
}

GLclampd renderer::OpenGLState::getDepthNear() const
{
	return zNear;
}

GLclampd renderer::OpenGLState::getDepthFar() const
{
	return zFar;
}

bool renderer::OpenGLState::getFaceCulling() const
{
	return faceCulling;
}

GLenum renderer::OpenGLState::getCullFace() const
{
	return cullFace;
}

GLenum renderer::OpenGLState::getFrontFace() const
{
	return frontFace;
}

Vec4 renderer::OpenGLState::getClearColor() const
{
	return clearColor;
}

void renderer::OpenGLState::getViewPort(GLint& x, GLint& y, GLsizei& width, GLsizei& height) const
{
	x = this->x;
	y = this->y;
	width = this->width;
	height = this->height;
}
