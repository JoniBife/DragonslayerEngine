#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "../textures/FrameBuffer.h"

namespace renderer {

	struct RenderTexture {
		GLuint textureId;
	};

	class RenderTarget {

	private:
		FrameBuffer* frameBuffer;

	public:
		RenderTarget();
		~RenderTarget();

		RenderTexture getRenderTexture();

	};

}

#endif