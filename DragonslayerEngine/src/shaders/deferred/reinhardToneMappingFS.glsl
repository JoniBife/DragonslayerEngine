#version 330 core
layout (location = 0) out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass

vec3 reinhardTonemapping(vec3 color) {
    return color = color / (color + vec3(1.0));
}

void main()
{
    vec3 color = texture(previousRenderTexture, fragTextCoords).xyz;

    color = reinhardTonemapping(color);
       
    // Gamma correction
    fragmentColor = vec4(pow(color, vec3(1.0/2.2)),1.0);
} 
