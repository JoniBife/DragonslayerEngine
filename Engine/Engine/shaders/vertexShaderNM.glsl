#version 330 core

// INPUT
in vec4 inPosition;
in vec3 inNormal;
in vec2 inTextCoord;
in vec3 inTangent;

// OUTPUT
out vec4 exPosition;
out vec3 exNormal;
out vec2 exTextCoord;
out vec3 tangFragPos;
out vec3 tangLightPos;
out vec3 tangViewPos;

// UNIFORMS
uniform vec4 lightPosition;
uniform vec3 viewPos;

uniform mat3 normal;
uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
	vec3 T = normalize(vec3(model * vec4(inTangent,0.0)));
    vec3 N = normalize(vec3(model * vec4(inNormal,0.0)));
	T = normalize(T - dot(T, N) * N);
	// We calculate the bitangent in the shader, its more efficient because its in parallel
	vec3 B = normalize(cross(N, T));
    mat3 tbn = transpose(mat3(T, B, N)); // Tangent space matrix, converts vertices in world space to tangent space

	vec4 fragPosition = (model * inPosition); 
	// We move everything to tangent space
	tangFragPos = tbn * fragPosition.xyz;
	tangLightPos = tbn * lightPosition.xyz;
	tangViewPos = tbn * viewPos;

	exPosition = inPosition;
	exTextCoord = inTextCoord;
	exNormal = tbn * (normal * inNormal);

	gl_Position =  projection * view * model * inPosition;
}