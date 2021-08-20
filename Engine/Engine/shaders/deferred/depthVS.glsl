#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 lightSpaceProjectionMatrix;

void main()
{	
	gl_Position =  lightSpaceProjectionMatrix * modelMatrix * position;
}