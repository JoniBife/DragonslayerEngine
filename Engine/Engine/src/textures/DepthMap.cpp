#include "DepthMap.h"
#include "../utils/OpenGLUtils.h"

DepthMap::DepthMap(GLsizei shadowWidth, GLsizei shadowHeight) : shadowWidth(shadowWidth), shadowHeight(shadowHeight) {

	// First we'll create a framebuffer object for rendering the depth map
	GL_CALL(glGenFramebuffers(1, &fbo));

	// Next we create a 2D texture that we'll use as the framebuffer's depth buffer
	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GL_CALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

	// With the generated depth texture we can attach it as the framebuffer's depth buffer
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0));
	GL_CALL(glDrawBuffer(GL_NONE));
	GL_CALL(glReadBuffer(GL_NONE));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

DepthMap::~DepthMap() {
	GL_CALL(glDeleteFramebuffers(1, &fbo));
	GL_CALL(glDeleteTextures(1, &id));
}

void DepthMap::setOnRenderToDepthMap(Engine* engine, std::function<void()> preRenderToDepthMap, std::function<void()> postRenderToDepthMap) {
	engine->setPreRender([=](std::function<void()> renderScene) {

		//GL_CALL(glCullFace(GL_FRONT));

		isTextureLoaded = false;

		// We bind the frameBuffer so that the scene is rendered to the depth map
		GL_CALL(glViewport(0, 0, shadowWidth, shadowHeight));
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
		GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

		preRenderToDepthMap();

		renderScene();
		
		postRenderToDepthMap();

		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		isTextureLoaded = true;

		//GL_CALL(glCullFace(GL_BACK));
	});
}


void DepthMap::bind(unsigned int unitNumber) {
	// On the first render of the scene the texture is ignored
	if (isTextureLoaded) {
		GL_CALL(glActiveTexture(unitNumber));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
	}
}

void DepthMap::unBind(unsigned int unitNumber) {
	if (isTextureLoaded) {
		GL_CALL(glActiveTexture(unitNumber));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}
}