#version 330 core
in vec4 inPosition; 

uniform mat4 model;
uniform mat4 projection;

void main()
{
    // No model matrix, it would be the same as having a model matrix which is the product identity
    gl_Position = projection * model * inPosition;
}  