#ifndef DEPTH_MAP_H
#define DEPTH_MAP_H

#include "ITexture.h"
#include "GL/glew.h"
#include "../Engine.h"

class DepthMap : public ITexture {

private:
	GLsizei shadowWidth;
	GLsizei shadowHeight;
	GLuint fbo;
	bool isTextureLoaded = true;

public:
	DepthMap(GLsizei shadowWidth, GLsizei shadowHeight);

	~DepthMap();

	void setOnRenderToDepthMap(Engine* engine, std::function<void()> preRenderToDepthMap, std::function<void()> postRenderToDepthMap);

	void bind(unsigned int unitNumber) override;

	void unBind(unsigned int unitNumber) override;
};

#endif


