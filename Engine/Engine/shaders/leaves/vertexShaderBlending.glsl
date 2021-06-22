#version 330 core

// INPUT
in vec4 inPosition;
in vec4 inColor;
in vec3 inNormal;
in vec2 inTextCoord;

// OUTPUT
out vec4 exPosition;
out vec4 exColor;
out vec3 exNormal;
out vec2 exTextCoord;
out vec4 fragPos;
out vec4 fragPosLightSpace;

uniform mat3 normal;
uniform mat4 model;

uniform mat4 lightSpaceMatrix;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
	fragPos = (model * inPosition); 
	fragPosLightSpace = lightSpaceMatrix * model *  inPosition;
	exPosition = inPosition;
	exColor = inColor;
	exTextCoord = inTextCoord;
	exNormal = normal * inNormal;

	gl_Position =  projection * view * model * inPosition;
}