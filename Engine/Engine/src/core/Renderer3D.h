#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include <vector>

#include "MeshRenderer.h"
#include "../textures/Texture2D.h"
#include "../textures/FrameBuffer.h"

namespace core {

	class Renderer3D {

	private:
		Texture2D* frameTexture = nullptr;
		FrameBuffer* frameBuffer = nullptr;
		GLuint rbo;

	public:
		Renderer3D();
		~Renderer3D();

		void setup();

		Texture2D& RenderToTexture(const Camera& camera, Hierarchy& hierarchy);
		void Render(const Camera& camera, Hierarchy& hierarchy);
			
	};

}

#endif
