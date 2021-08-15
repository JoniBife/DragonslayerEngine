#version 330 core

in vec4 position;
in vec3 normal;
in vec4 color;
in vec2 textCoord;

out vec4 exPosition;
out vec3 exNormal;
out vec4 exColor;
out vec2 exTextCoord;
out vec4 fragPos;

uniform mat3 normal;
uniform mat4 model;

uniform sharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
	exPosition = inPosition;
	exColor = inColor;
	exTextCoord = inTextCoord;
	exNormal = normal * inNormal;
	fragPos = model * inPosition;

	gl_Position =  projection* view * model * inPosition;
}