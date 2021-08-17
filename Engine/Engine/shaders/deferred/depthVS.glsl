#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 modelMatrix;

uniform sharedMatrices {
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

void main()
{	
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}