#version 330 core

in vec4 inPosition;
in vec3 inNormal;
in vec4 inColor;
in vec2 inTextCoord;

out vec4 exPosition;
out vec3 exNormal;
out vec4 exColor;
out vec2 exTextCoord;

uniform mat3 normal;
uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
	exPosition = inPosition;
	exColor = inColor;
	exTextCoord = inTextCoord;
	exNormal = normal * inNormal;

	gl_Position =  projection * view * model * inPosition;
}