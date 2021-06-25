#version 330 core

layout (location = 0) in vec4 inPosition;
layout (location = 3) in vec2 inTextCoord;

out vec2 textCoord;

void main(void)
{
	textCoord = inTextCoord;

	// We draw directly into clip space
	gl_Position =  inPosition;
}