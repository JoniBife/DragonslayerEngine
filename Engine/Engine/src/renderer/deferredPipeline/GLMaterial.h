#ifndef GL_MATERIAL_H
#define GL_MATERIAL_H
#include "../../shaders/shaderProgram.h"

namespace renderer {

	/* Base class for all OpenGL materials */
	class GLMaterial {

		public:
			virtual ShaderProgram& getGeometryShaderProgram() const = 0;
			virtual void sendParametersToGeometryShaderProgram(const Mat4& model, const Mat3& normal) = 0; // Sends parameters to shader used for the geometry pass
	};
}

#endif