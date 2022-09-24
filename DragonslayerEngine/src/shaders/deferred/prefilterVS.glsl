#version 330 core
layout (location = 0) in vec4 position;

out vec3 fragLocalPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    fragLocalPosition = position.xyz;  
    gl_Position =  projectionMatrix * viewMatrix * vec4(fragLocalPosition, 1.0);
}