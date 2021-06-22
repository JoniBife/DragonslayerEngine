#version 330 core

// INPUT
in vec4 inPosition;
in vec3 inNormal;
in vec2 inTextCoord;
in vec3 inTangent;

uniform mat3 normal;
uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

uniform mat4 lightSpaceMatrix;

void main(void)
{	
	gl_Position =  lightSpaceMatrix * model * inPosition;
}