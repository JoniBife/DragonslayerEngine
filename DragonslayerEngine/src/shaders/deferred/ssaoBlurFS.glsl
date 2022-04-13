#version 330 core
layout (location = 0) out float fragmentColor;

in vec2 fragTextCoords;

uniform sampler2D dottedSSAO;

const uint blurSize = 4u;

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(dottedSSAO, 0));
    float result = 0.0;
    for (uint x = 0u; x < blurSize; ++x) 
    {
        for (uint y = 0u; y < blurSize; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(dottedSSAO, fragTextCoords + offset).r;
        }
    }
    fragmentColor = result / (blurSize * blurSize);
}  