#version 330 core

in vec4 inPosition;
in vec3 inNormal;
in vec2 inTextCoord;

out vec4 fragPosition;
out vec3 fragNormal;
out vec2 fragTextCoord;

uniform mat3 normal;
uniform mat4 model;

uniform sharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
	fragPosition = model * inPosition;
	fragNormal = normal * inNormal;
	fragTextCoord = inTextCoord;

	gl_Position =  projection* view * model * inPosition;
}