#version 330 core

in vec4 inPosition;

void main(void)
{
	gl_Position = inPosition;
}