#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "../shaders/shaderProgram.h"

/* Base class for all materials (e.g BlinnPhongMaterial) */
class Material {

private:
	std::string name;

protected:
	ShaderProgram* shaderProgram = nullptr;

	Material(const std::string& name);
	virtual ~Material();

public:
	ShaderProgram& getShaderProgram() const;

	/* Should send the material parameters to the shader (e.g. via uniforms) */
	virtual void sendParametersToShader() = 0;

	std::string getName() const;
};

#endif
