#version 330 core

in vec4 position;

out vec4 fragPosition;

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