#version 330 core

layout (location = 0) in vec4 position;

out vec3 fragTextCoord;

uniform sharedMatrices {
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

void main()
{
    fragTextCoord = position.xyz;

	// Removing translation from view matrix, because the skybox is always centered
    vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * position;
	gl_Position = pos.xyww;
}  