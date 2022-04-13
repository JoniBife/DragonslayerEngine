#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textCoords;

out vec2 fragTextCoords;

void main()
{
    fragTextCoords = textCoords;
	gl_Position = vec4(position.xyz, 1.0);
}