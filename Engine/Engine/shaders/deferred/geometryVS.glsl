#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textCoord;
layout (location = 3) in vec3 tangent;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTextCoords;
out mat3 TBNMatrix;

uniform mat3 normalMatrix;
uniform mat4 modelMatrix;

uniform sharedMatrices {
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

void main(void)
{
	fragPosition = (modelMatrix * position).xyz;
	fragNormal = normalMatrix * normal;
	fragTextCoords = textCoord;

	vec3 N = normalize(mat3(normalMatrix) * normal);
    vec3 T = normalize(mat3(normalMatrix) * tangent);
    T = normalize(T - dot(N, T) * N);
	vec3 B = normalize(cross(T,N));

	// This matrix moves a tangent space vector to world space
	TBNMatrix = mat3(T,B,N);

	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * position;
}