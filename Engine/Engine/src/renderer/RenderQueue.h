#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <queue>
#include "RenderCommand.h"
#include "PostProcessingCommand.h"

namespace WarriorRenderer {

	class RenderQueue {

	private:
		// TODO Limit the size of each of render queues, consider
		// pre-allocating to avoid any allocations at each frame render
		// I believe unity has a limit of 5000 elements on some queues
		std::queue<RenderCommand*, std::deque<RenderCommand*>> geometryQueue; // stores commands for geometry pass
		std::queue<RenderCommand*, std::deque<RenderCommand*>> shadowMapQueue; // stores commands for the shadow mapping pass
		std::queue<RenderCommand*, std::deque<RenderCommand*>> alphaTestQueue; // stores commands for alpha testing pass
		std::queue<RenderCommand*, std::deque<RenderCommand*>> customShadersQueue; // stores commands for the custom shaders pass
		std::queue<PostProcessingCommand*, std::deque<PostProcessingCommand*>> postProcessingQueue; // stores commands for the post-processing pass

	public:
		bool enqueueRender(RenderCommand* renderCommand);
		bool enqueuePostProcessing(PostProcessingCommand* postProcessingCommand);

		bool isGeometryEmpty();
		RenderCommand& dequeueGeometry();

		bool isShadowMapEmpty();
		RenderCommand& dequeueShadowMap();
		std::queue<RenderCommand*>& getShadowMapQueue();

		bool isAlphaTestEmpty();
		RenderCommand& dequeueAlphaTest();

		bool isCustomShadersEmpty();
		RenderCommand& dequeueCustomShaders();

		bool isPostProcessingEmpty();
		PostProcessingCommand& dequeuePostProcessing();

		void clear();

	};
}

#endif
