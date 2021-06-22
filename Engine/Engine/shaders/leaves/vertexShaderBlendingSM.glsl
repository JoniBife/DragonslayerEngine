#version 330 core

// INPUT
in vec4 inPosition;
in vec3 inNormal;
in vec2 inTextCoord;

out vec2 exTextCoord;

uniform mat3 normal;
uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

uniform mat4 lightSpaceMatrix;

void main(void)
{
	vec3 exNormal = normal * inNormal;

	exTextCoord = inTextCoord;

	gl_Position =  lightSpaceMatrix * model * inPosition;
}