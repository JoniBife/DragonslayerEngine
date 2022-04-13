#include "DragonslayerEngine/OpenGLState.h"
#include "DragonslayerEngine/utils/OpenGLUtils.h"

using namespace DragonslayerEngine;

DragonslayerEngine::OpenGLState::OpenGLState()
{
	
}
DragonslayerEngine::OpenGLState::~OpenGLState()
{
	// Does nothing for now (TODO Check if it should do something)
}

void DragonslayerEngine::OpenGLState::setToDefaultState()
{
	if (depthTesting) {
		GL_CALL(glEnable(GL_DEPTH_TEST));
	}
	else {
		GL_CALL(glDisable(GL_DEPTH_TEST));
	}

	GL_CALL(glDepthFunc(depthFunction));
	//GL_CALL(glDepthMask(depthMask));
	GL_CALL(glDepthRange(zNear, zFar));

	if (faceCulling) {
		GL_CALL(glEnable(GL_CULL_FACE));
	}
	else {
		GL_CALL(glDisable(GL_CULL_FACE));
	}
	GL_CALL(glCullFace(cullFace));
	GL_CALL(glFrontFace(frontFace));

	if (blending) {
		GL_CALL(glEnable(GL_BLEND));
	}
	else {
		GL_CALL(glDisable(GL_BLEND));
	}

	GL_CALL(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
	GL_CALL(glViewport(x, y, width, height));
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

void DragonslayerEngine::OpenGLState::setDepthFunction(GLenum depthFunction)
{
	if (this->depthFunction != depthFunction) {

		this->depthFunction = depthFunction;
		GL_CALL(glDepthFunc(depthFunction));
	}
}

void DragonslayerEngine::OpenGLState::setDepthMask(GLboolean enable)
{
	if (this->depthMask != enable) {

		this->depthMask = enable;
		GL_CALL(glDepthMask(depthMask));
	}
}

void DragonslayerEngine::OpenGLState::setDepthRange(GLclampd zNear, GLclampd zFar)
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

void DragonslayerEngine::OpenGLState::setCullFace(GLenum face)
{
	if (this->cullFace != face) {

		this->cullFace = face;
		GL_CALL(glCullFace(cullFace));
	}
}

void DragonslayerEngine::OpenGLState::setFrontFace(GLenum frontFace)
{
	if (this->frontFace != frontFace) {

		this->frontFace = frontFace;
		GL_CALL(glFrontFace(frontFace));
	}
}

void DragonslayerEngine::OpenGLState::setClearColor(Vec4 clearColor)
{
	if (this->clearColor != clearColor) {
		this->clearColor = clearColor;
		GL_CALL(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
	}
}

void DragonslayerEngine::OpenGLState::setViewPort(GLint x, GLint y, GLsizei width, GLsizei height)
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

void DragonslayerEngine::OpenGLState::setBlending(bool enable)
{
	if (blending != enable) {
		blending = enable;
		if (blending) {
			GL_CALL(glEnable(GL_BLEND));
		} else {
			GL_CALL(glDisable(GL_BLEND));
		}
	}
}

GLuint DragonslayerEngine::OpenGLState::getActiveShaderProgram() const
{
	return activeShaderProgram;
}

bool DragonslayerEngine::OpenGLState::getDepthTesting() const
{
	return depthTesting;
}

GLenum DragonslayerEngine::OpenGLState::getDepthFunction() const
{
	return depthFunction;
}

GLboolean DragonslayerEngine::OpenGLState::getDepthMask() const
{
	return depthMask;
}

GLclampd DragonslayerEngine::OpenGLState::getDepthNear() const
{
	return zNear;
}

GLclampd DragonslayerEngine::OpenGLState::getDepthFar() const
{
	return zFar;
}

bool DragonslayerEngine::OpenGLState::getFaceCulling() const
{
	return faceCulling;
}

GLenum DragonslayerEngine::OpenGLState::getCullFace() const
{
	return cullFace;
}

GLenum DragonslayerEngine::OpenGLState::getFrontFace() const
{
	return frontFace;
}

Vec4 DragonslayerEngine::OpenGLState::getClearColor() const
{
	return clearColor;
}

void DragonslayerEngine::OpenGLState::getViewPort(GLint& x, GLint& y, GLsizei& width, GLsizei& height) const
{
	x = this->x;
	y = this->y;
	width = this->width;
	height = this->height;
}

bool DragonslayerEngine::OpenGLState::getBlending() const
{
	return blending;
}
