#ifndef MATERIAL_R_H
#define MATERIAL_R_H

#include "deferredPipeline/GLMaterial.h"

namespace renderer {

	class Material {
	
	friend class RenderPipeline;

	private:
		GLMaterial* glMaterial = nullptr;
		// TODO VKMAterial* vkMaterial = nullptr;
		// TODO DXMaterial* dxMaterial = nulltr;

		Material(GLMaterial* glMaterial);
		// TODO Vulkan & Direct X
		~Material();

	public:
		GLMaterial& getOpenGLMaterial() const;
		// TODO VKMaterialP& getVulkanMaterial() const;
		// TODO DXMaterial& getDirectXMaterial() const;
	};

}

#endif
