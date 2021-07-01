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
	GLint modelMatrixLocation = -1;
	GLint normalMatrixLocation = -1;

	Material(const std::string& name);
	virtual ~Material();

public:
	ShaderProgram& getShaderProgram() const;

	/*! @brief Should send the material parameters to the shader (e.g. via uniforms)
	and the model and normal matrix.
	*/
	virtual void sendParametersToShader(const Mat4& modelMatrix, const Mat3& normalMatrix = Mat3::IDENTITY) = 0;

	void use();

	void stopUsing();

	std::string getName() const;
};

#endif
