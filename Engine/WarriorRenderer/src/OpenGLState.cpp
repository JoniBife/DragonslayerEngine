#include "OpenGLState.h"
#include "utils/OpenGLUtils.h"

using namespace WarriorRenderer;

WarriorRenderer::OpenGLState::OpenGLState()
{
	
}
WarriorRenderer::OpenGLState::~OpenGLState()
{
	// Does nothing for now (TODO Check if it should do something)
}

void WarriorRenderer::OpenGLState::setToDefaultState()
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

void WarriorRenderer::OpenGLState::setDepthFunction(GLenum depthFunction)
{
	if (this->depthFunction != depthFunction) {

		this->depthFunction = depthFunction;
		GL_CALL(glDepthFunc(depthFunction));
	}
}

void WarriorRenderer::OpenGLState::setDepthMask(GLboolean enable)
{
	if (this->depthMask != enable) {

		this->depthMask = enable;
		GL_CALL(glDepthMask(depthMask));
	}
}

void WarriorRenderer::OpenGLState::setDepthRange(GLclampd zNear, GLclampd zFar)
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

void WarriorRenderer::OpenGLState::setCullFace(GLenum face)
{
	if (this->cullFace != face) {

		this->cullFace = face;
		GL_CALL(glCullFace(cullFace));
	}
}

void WarriorRenderer::OpenGLState::setFrontFace(GLenum frontFace)
{
	if (this->frontFace != frontFace) {

		this->frontFace = frontFace;
		GL_CALL(glFrontFace(frontFace));
	}
}

void WarriorRenderer::OpenGLState::setClearColor(Vec4 clearColor)
{
	if (this->clearColor != clearColor) {
		this->clearColor = clearColor;
		GL_CALL(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
	}
}

void WarriorRenderer::OpenGLState::setViewPort(GLint x, GLint y, GLsizei width, GLsizei height)
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

void WarriorRenderer::OpenGLState::setBlending(bool enable)
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

GLuint WarriorRenderer::OpenGLState::getActiveShaderProgram() const
{
	return activeShaderProgram;
}

bool WarriorRenderer::OpenGLState::getDepthTesting() const
{
	return depthTesting;
}

GLenum WarriorRenderer::OpenGLState::getDepthFunction() const
{
	return depthFunction;
}

GLboolean WarriorRenderer::OpenGLState::getDepthMask() const
{
	return depthMask;
}

GLclampd WarriorRenderer::OpenGLState::getDepthNear() const
{
	return zNear;
}

GLclampd WarriorRenderer::OpenGLState::getDepthFar() const
{
	return zFar;
}

bool WarriorRenderer::OpenGLState::getFaceCulling() const
{
	return faceCulling;
}

GLenum WarriorRenderer::OpenGLState::getCullFace() const
{
	return cullFace;
}

GLenum WarriorRenderer::OpenGLState::getFrontFace() const
{
	return frontFace;
}

Vec4 WarriorRenderer::OpenGLState::getClearColor() const
{
	return clearColor;
}

void WarriorRenderer::OpenGLState::getViewPort(GLint& x, GLint& y, GLsizei& width, GLsizei& height) const
{
	x = this->x;
	y = this->y;
	width = this->width;
	height = this->height;
}

bool WarriorRenderer::OpenGLState::getBlending() const
{
	return blending;
}
