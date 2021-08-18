#ifndef RENDERER_H
#define RENDERER_H

#include "RenderPipeline.h"

namespace renderer {

	class Renderer {

	private:
		RenderPipeline* renderPipeline;

	public:
		Renderer(RenderPipeline* renderPipeline);
		~Renderer();


	};

}

#endif
