#version 330 core
layout (location = 0) out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass

const float offset = 1.0 / 300.0; 

vec3 ACESFilm(vec3 x)
{
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    
    vec3 color = (x*(a*x+b))/(x*(c*x+d)+e); 
    return clamp(color,0.0,1.0);
}

uniform bool toneMapping;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    /*float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );*/

    /*float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );*/

    float kernel[9] = float[](
        1,  1, 1,
        1, -8, 1,
        1,  1, 1 
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(previousRenderTexture, fragTextCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    vec4 color = texture(previousRenderTexture, fragTextCoords);
    // TODO Post processing
    //fragmentColor = vec4(1,1,0,1);
    
    if (toneMapping)
        fragmentColor = vec4(ACESFilm(color.xyz), 1);
    else
        fragmentColor = vec4(color.xyz, 1);
    
    //fragmentColor = vec4(col, 1.0);
}  