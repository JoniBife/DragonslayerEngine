#version 330 core
layout (location = 0) out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass

void main()
{
    vec4 color = texture(previousRenderTexture, fragTextCoords);
    // TODO Post processing
    fragmentColor = color;
} 