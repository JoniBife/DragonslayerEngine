#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "textures/FrameBuffer.h"

namespace DragonslayerEngine {

	struct RenderTexture {
		GLuint textureId;
	};

	class RenderTarget {

	friend class DeferredRenderPipeline;

	private:
		FrameBuffer frameBuffer;

	public:
		RenderTarget();
		~RenderTarget();

		RenderTexture getRenderTexture();

	};

}

#endif
