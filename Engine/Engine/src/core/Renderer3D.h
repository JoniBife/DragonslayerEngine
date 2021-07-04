#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include <vector>

#include "MeshRenderer.h"
#include "../textures/FrameBuffer.h"
#include "../core/Hierarchy.h"

namespace core {

	class Renderer3D {

	public:
		Renderer3D();
		~Renderer3D();

		void setup();

		void renderToFrameBuffer(const Camera& camera, Hierarchy& hierarchy, FrameBuffer& frameBuffer);
		void render(const Camera& camera, Hierarchy& hierarchy);
	};

}

#endif
