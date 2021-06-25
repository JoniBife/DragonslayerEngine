#version 330 core

out vec4 fragmentColor;

in vec2 textCoord;

uniform sampler2D sceneTexture;

void main(void)
{
	fragmentColor = texture(sceneTexture, textCoord);
}

















