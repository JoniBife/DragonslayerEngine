#version 330 core

in vec4 exPosition;
in vec3 exNormal;
in vec4 exColor;
in vec2 exTextCoord;

out vec4 fragmentColor;

void main(void)
{
	fragmentColor = exColor;
}