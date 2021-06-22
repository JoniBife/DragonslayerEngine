#version 330 core

// INPUT
in vec2 exTextCoord;

uniform sampler2D diffuseMap;

void main(void)
{
	vec4 texColor = texture(diffuseMap, exTextCoord);

    if(texColor.a < 0.1)
        discard;
}
