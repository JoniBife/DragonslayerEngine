#version 330 core
out vec4 fragmentColor;

in vec3 fragTextCoord;

uniform samplerCube skybox;

float luminance(vec3 v)
{
    return dot(v, vec3(0.2126, 0.7152, 0.0722));
}

vec3 reinhard_jodie(vec3 v)
{
    float l = luminance(v);
    vec3 tv = v / (1.0 + v);
    return mix(v / (1.0 + l), tv, tv);
}

void main()
{    
    fragmentColor = vec4(texture(skybox, fragTextCoord).rgb, 1.0);
}