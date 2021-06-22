#version 330 core

in vec4 inPosition;

out vec3 exTextCoord;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main()
{
    exTextCoord = inPosition.xyz;
    gl_Position = projection * mat4(mat3(view)) * inPosition;
}  