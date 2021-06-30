#ifndef MATERIAL_H
#define MATERIAL_H


#include "../shaders/shaderProgram.h"

class Material {

protected:
	ShaderProgram& shaderProgram;

	Material(ShaderProgram& shaderProgram);

public:
	ShaderProgram& getShaderProgram() const;

	/* Should send the material parameters to the shader as uniforms */
	virtual void setUniforms();
};

#endif
