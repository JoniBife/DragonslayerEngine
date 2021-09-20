#version 330 core
layout (location = 0) out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass

// ACES Tonemapping found here: https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
// Note: Oversaturates brights. For more realistic rendering use: https://github.com/TheRealMJP/BakingLab/blob/master/BakingLab/ACES.hlsl
vec3 acesFilmTonemapping(vec3 x)
{
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    
    vec3 color = (x*(a*x+b))/(x*(c*x+d)+e); 
    return clamp(color,0.0,1.0);
}

vec3 reinhardTonemapping(vec3 color) {
    return color = color / (color + vec3(1.0));
}

void main()
{
    vec3 color = texture(previousRenderTexture, fragTextCoords).xyz;

    color = acesFilmTonemapping(color);
       
    // Gamma correction
    fragmentColor = vec4(pow(color, vec3(1.0/2.2)),1.0);
} 
