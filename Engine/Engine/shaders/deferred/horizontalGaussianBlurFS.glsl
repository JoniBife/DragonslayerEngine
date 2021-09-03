#version 330 core
layout (location = 0) out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass

const float gaussianWeigths[5] = float[]( 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 );
//const float gaussianWeigths[5] = float[]( 0.22, 0.15, 0.11, 0.08, 0.05 );
//const float gaussianWeigths[5] = float[]( 0.02, 0.05, 0.10, 0.10, 0.24 );

void main()
{
    
    vec3 color = texture(previousRenderTexture, fragTextCoords).rgb * gaussianWeigths[0];
    float texelWidth = (1.0 / textureSize(previousRenderTexture,0)).x;
    
    for (int i = 1; i < gaussianWeigths.length(); ++i) 
    {
        color += texture(previousRenderTexture, fragTextCoords + vec2(i * texelWidth, 0.0)).rgb * gaussianWeigths[i];
        color += texture(previousRenderTexture, fragTextCoords - vec2(i * texelWidth, 0.0)).rgb * gaussianWeigths[i];
    }
    
    fragmentColor = vec4(color, 1.0);
}  